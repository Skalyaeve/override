#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int secret_backdoor()
{
    char command[128];

    fgets(command, 128, stdin);
    return system(command);
}

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

int main(void)
{
    puts(
        "--------------------------------------------\n"
        "|   ~Welcome to l33t-m$n ~    v1337        |\n"
        "--------------------------------------------");
    handle_msg();
    return 0;
}
