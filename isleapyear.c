#include <stdio.h>

int main(int argc, char** argv)
{
    if((int)argv % 4 == 0)
    {
        if((int)argv % 100 == 0 && (int)argv % 400 != 0)
        {
            printf("%s was not a leap year\n", argv);
        }
        else
        {
            printf("%s was a leap year\n", argv);
        }
    }
    else
    {
        printf("%s was not a leap year\n", argv);
    }
    return 0;
}
