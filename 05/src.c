#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char string[100];
    unsigned int i;

    i = 0;
    fgets(string, 100, stdin);
    for (i = 0; i < strlen(string); ++i)
    {
        if (string[i] >= 'A' && string[i] <= 'Z')
            string[i] ^= 0b00100000;
    }
    printf(string);
    exit(0);
}
