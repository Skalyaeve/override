# 06 - RevMe (3)
> README by [jchene](https://github.com/jchene)

We have a binary named level06 at /home/users/level06/

It is owned by `level07` and has a `setuid bit`

The program is divided in two functions: `main` and `auth`

In `main` the program starts by asking the user for some inputs: a string `login` and an `unsigned int` `value`

```c
int main(void)
{
    int value;
    char login[28];

    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");

    printf("-> Enter Login: ");
    fgets(login, 32, stdin);

    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");

    printf("-> Enter Serial: ");
    scanf("%u", &value);
```

Then we call the `auth` function, if the function returns anything other than `0`, the authentication fails and we return, else the program executes `system('/bin/sh')`

We have to bypass the authentication to get access to the `shell`

The `auth` function starts by removing the newline and checking the length of the login, we return if the len i less than `6`

```c
bool auth(char *login, int value)
{
    int loginValue;

    login[strcspn(login, "\n")] = 0;
    if (strnlen(login, 32) <= 5)
        return 1;
```

Then the program checks for any debugger tampering by using `ptrace` which will prevent us to access to the authentication part is we use gdb on the binary

```c
if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
    {
        puts("\x1B[32m.---------------------------.");
        puts("\x1B[31m| !! TAMPERING DETECTED !!  |");
        puts("\x1B[32m'---------------------------'");
        return 1;
    }
```

If no tampering is detected we can access to the authentication function

We can see the function takes the `login` and process it through some calculus to determine a value for the `login` and then returns the comparison between the `value` passed as parameter and the value calculated from the `login`

```c
else
    {
        loginValue = (login[3] ^ 0x1337) + 6221293;
        for (int i = 0; i < strnlen(login, 32); ++i)
        {
            if (login[i] <= 31)
                return 1;
            loginValue += (loginValue ^ (unsigned int)login[i]) % 0x539;
        }
        return value != loginValue;
    }
```

To get through the authentication we have to first find a string's value and pass it to the program, to simplify this task we can extract the algorithm part of the program and run it to encrypt and get the value of a string

Here is the algorithm implemented in the simplest way
```c
int encode(char *login)
{
    int loginValue;

    loginValue = (login[3] ^ 0x1337) + 6221293;
    for (int i = 0; i < strnlen(login, 7); ++i)
    {
        if (login[i] <= 31)
                return 1;
        loginValue += (loginValue ^ (unsigned int)login[i]) % 1337;
    }
    return loginValue;
}

int main()
{
    char s[7] = "AAAAAA";
    printf("%d\n", encode(s));
}
```

After compiling and running it we get the value `6229050` for the login `AAAAAA`

```
$ gcc ./encode.c
$ ./a.out
6229050
```

Let's try it on the real binary now
```
level06@OverRide:~$ ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: AAAAAA
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6229050
Authenticated!
$ whoami
level07
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```

The flag is
```
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```
