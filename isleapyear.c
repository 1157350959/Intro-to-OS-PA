#include <stdio.h>

int main()
{
    int year[5] = [1900,2000,2004,2012,2018];
    for(int i = 0; i < 5; i++)
    {
        if(year[i] % 4 == 0)
        {
            printf("%d was a leap year", year[i]);
        }
        else
        {
            printf("%d was not a leap year.", year[i]);
        }
    }
    return 0;
}
