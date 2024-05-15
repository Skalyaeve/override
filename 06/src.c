#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/ptrace.h>
#include <unistd.h>

bool auth(char *login, int value)
{
    int loginValue;

    login[strcspn(login, "\n")] = 0;
    if (strnlen(login, 32) <= 5)
        return 1;

    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
    {
        puts("\x1B[32m.---------------------------.");
        puts("\x1B[31m| !! TAMPERING DETECTED !!  |");
        puts("\x1B[32m'---------------------------'");
        return 1;
    }
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
}

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

    if (auth(login, value))
        return 1;

    puts("Authenticated!");
    system("/bin/sh");
    return 0;
}
