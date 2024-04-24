#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
        FILE *var_file_ptr; // (rbp - 0x8) 0x8 -> 0x0 = 0x8
        int var_int;        // (rbp - 0xc) 0xc -> 0x8 = 0x4
        char buffer1[0x64]; // (rbp - 0x70) 0x70 -> 0xc = 0x64
        char buffer2[0x30]; // (rbp - 0xa0) 0xa0 -> 0x70 = 0x30
        char buffer3[0x70]; // (rbp - 0x110) 0x110 -> 0xa0 = 0x70

        int i;
        for (i = 0x0; i < 0x64; i++) // QWORD * 0xc + DWORD = 0x64
                buffer1[i] = 0x0;
        for (i = 0x0; i < 0x29; i++) // QWORD * 0x5 + BYTE = 0x29 -> sizes differ
                buffer2[i] = 0x0;
        for (i = 0x0; i < 0x64; i++) // QWORD * 0xc + DWORD = 0x64 -> sizes differ
                buffer3[i] = 0x0;
        var_file_ptr = 0x0;
        var_int = 0x0;
        var_file_ptr = fopen("/home/users/level03/.pass", "r");
        if (!var_file_ptr)
        {
                fwrite("ERROR: failed to open password file\n", 0x1, 0x24, stderr);
                exit(0x1);
        }
        var_int = fread(buffer2, 0x1, 0x29, var_file_ptr);
        buffer2[strcspn(buffer2, "\n")] = 0x0;
        if (var_int != 0x29)
        {
                fwrite("ERROR: failed to read password file\n", 0x1, 0x24, stderr); //
                fwrite("ERROR: failed to read password file\n", 0x1, 0x24, stderr); // wtf ?
                exit(0x1);
        }
        puts("===== [ Secure Access System v1.0 ] =====");
        puts("/***************************************\\");
        puts("| You must login to access this system. |");
        puts("\\**************************************/");
        printf("--[ Username: ");
        fgets(buffer1, 0x64, stdin);
        buffer1[strcspn(buffer1, "\n")] = 0x0;
        printf("--[ Password: ");
        fgets(buffer3, 0x64, stdin);
        buffer3[strcspn(buffer3, "\n")] = 0x0;
        puts("*****************************************");
        if (!strncmp(buffer2, buffer3, 0x29))
        {
                printf("Greetings, %s!\n", buffer1);
                system("/bin/sh");
                return 0x0;
        }
        printf(buffer1); // gg
        puts(" does not have access!");
        exit(0x1);
}
