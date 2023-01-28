#include <stdio.h>

int main()
{
    int year = 0;
    printf("Input the year to be check: ");
    scanf("%d", &year);
    if(year % 4 == 0)
    {
        printf("Year %d is a leap year.", year);
    }
    else
    {
        printf("Year %d is not a leap year.", year);
    }
    return 0;
}