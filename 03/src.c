#include <time.h>
#include <stdlib.h>
#include <stdio.h>

typedef char byte;
typedef unsigned int uint;

void decrypt(int input)
{
        // -0xc(%ebp) = canary
        char key[17] = "Q}|u`sfg~sf{}|a3"; // [-0x1d(%ebp) --> -0xd(%ebp)] = 17 bytes
        // gap of 3 bytes for alignment: 0x20 - 0x1d = 3 bytes
        uint size;  // -0x24(%ebp) --> 0x24 - 0x20 = 4 bytes
        uint count; // -0x2c(%ebp) --> 0x2c - 0x28 = 4 bytes
        int index;  // -0x28(%ebp) --> 0x28 - 0x24 = 4 bytes

        count = 0xffffffff;
        for (uint i = 0; count; i++)
        {
                count--;
                if (!key[i])
                        break;
        }
        count = ~count;
        count--;

        size = count;
        index = 0;
        while (index < size)
        {
                key[index] = (char)input ^ key[index];
                index++;
        }
        char *ref = "Congratulations!";
        for (int i = 0; i < 0x11; i++)
        {
                if (key[i] != *(ref + i) || !key[i])
                {
                        if ((key[i] > *(ref + i)) + (key[i] < *(ref + i)))
                                puts("\nInvalid Password");
                        else
                                system("/bin/sh");
                        break;
                }
        }
}

void test(int input, int modifier)
{
        int result; // -0xc(%ebp)

        result = modifier - input;
        switch (result)
        {
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0x10: // 0x10 = 16 et 0x9 = 9
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
                decrypt(result);
                break;
        default:
                decrypt(rand());
        }
}

int main(void)
{
        int number; // 0x1c(%esp)

        srand(time(0x0));
        puts("***********************************");
        puts("*\t\tlevel03\t\t**");
        puts("***********************************");
        printf("Password:");
        scanf("%d", &number);
        test(number, 0x1337d00d);
        return 0x0;
}