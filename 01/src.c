#include <stdio.h>

char a_user_name[0x100]; // 0x804a040 (.bss)

int verify_user_pass(const char *password)
{
        const char *ptr = a_user_name;
        for (int i = 0x0; i < 0x5; i++)
        {
                if (*password != *ptr)
                        break;
                password++;
                ptr++;
        }
        return (*ptr > *password) - (*ptr < *password);
}

int verify_user_name(void)
{
        puts("verifying username....\n");
        const char *ptr = a_user_name;
        const char *ref = "dat_wil";
        for (int i = 0x0; i < 0x7; i++)
        {
                if (*ref != *ptr)
                        break;
                ref++;
                ptr++;
        }
        return (*ptr > *ref) - (*ptr < *ref);
}

int main(void)
{
        int returned;            // (esp+0x5c) 0x4  -> 0x0 = 0x4 octets
        char passw_buffer[0x40]; // (esp+0x1c) 0x44 -> 0x4 = 0x40 octets

        for (int i = 0x0; i < 0x40; i++) // DWORD * 0x10 = 0x40
                passw_buffer[i] = 0x0;
        returned = 0x0;
        puts("********* ADMIN LOGIN PROMPT *********");
        printf("Enter Username: ");
        fgets(&a_user_name, 0x100, stdin);
        returned = verify_user_name();
        if (returned != 0x0)
        {
                puts("nope, incorrect username...\n");
                return 0x1;
        }
        puts("Enter Password: ");
        fgets(&passw_buffer, 0x64, stdin); // overflow
        returned = verify_user_pass(passw_buffer);
        if (returned == 0x0)
        {
                puts("nope, incorrect password...\n");
                return 0x1;
        }
        if (returned == 0x0) //
                return 0x0;  // wtf
        puts("nope, incorrect password...\n");
        return 0x1;
}
