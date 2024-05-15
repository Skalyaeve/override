#include <stdio.h>
#include <string.h>

int clear_stdin(void)
{
    int character = 0;

    while ((char)character != 10 && (char)character != 0xFF)
        character = getchar();
    return character;
}

unsigned int get_unum(void)
{
    unsigned int result = 0;

    fflush(stdout);
    scanf("%u", &result);
    clear_stdin();
    return result;
}

int store_number(int storageAddress)
{
    unsigned int number;
    unsigned int index;

    printf(" Number: ");
    number = get_unum();
    printf(" Index: ");
    index = get_unum();
    if (index % 3 == 0 || HIBYTE(number) == 183)
    {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return 1;
    }
    else
    {
        *(int *)(storageAddress + 4 * index) = number;
        return 0;
    }
}

int read_number(int storageAddress)
{
    int index;

    printf(" Index: ");
    index = get_unum();
    printf(" Number at data[%u] is %u\n", index, *(int *)(storageAddress + 4 * index));
    return 0;
}

int main(int argc, const char **argv, const char **envp)
{
    int storage[100];
    char command[20];
    int number = 0;

    memset(storage, 0, sizeof(storage));
    memset(command, 0, sizeof(command));
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
    while (1)
    {
        printf("Input command: ");
        fgets(command, 20, stdin);
        command[strlen(command) - 1] = 0;

        number = 1;
        if (!memcmp(command, "store", 5))
            number = store_number((int)storage);
        else if (!memcmp(command, "read", 4))
            number = read_number((int)storage);
        else if (!memcmp(command, "quit", 4))
            return 0;

        if (number)
            printf(" Failed to do %s command\n", command);
        else
            printf(" Completed %s command successfully\n", command);
        memset(command, 0, sizeof(command));
    }
}
