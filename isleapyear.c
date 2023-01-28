#include <stdio.h>

int main()
{
    int year = 0;
    scanf("%d", &year);
    if(year % 4 == 0)
    {
        printf("%d was a leap year", year);
    }
    else
    {
        printf("%d was not a leap year.", year);
    }
    return 0;
}
