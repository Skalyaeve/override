#include <stdio.h>
#include <stdlib.h>

int main(void)
{
        int buffer;
        puts("***********************************");
        puts("* \t     -Level00 -\t\t  *");
        puts("***********************************");
        printf("Password:");
        scanf("%d", &buffer);
        if (buffer != 0x149c)
        {
                puts("\nInvalid Password!");
                return 0x1;
        }
        puts("\nAuthenticated!");
        system("/bin/sh");
        return 0x0;
}
