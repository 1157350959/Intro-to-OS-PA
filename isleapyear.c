#include <stdio.h>
int main(int argc, char* argv[])
{
    if(atoi(argv[1]) % 4 == 0)
    {
        if(atoi(argv[1]) % 100 == 0 && atoi(argv[1]) % 400 != 0)
        {
            printf("%s was not a leap year\n", argv[1]);
        }
        else
        {
            printf("%s was a leap year\n", argv[1]);
        }
    }
    else
    {
        printf("%s was not a leap year\n", argv[1]);
    }
    return 0;
}
