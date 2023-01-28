#include <stdio.h>

int main()
{
    int year;
    scanf("%d", &year);
    if(year % 4 == 0)
    {
        printf("%d was a leap year\n", year);
    }
    else
    {
        printf("%d was not a leap year\n", year);
    }
    return 0;
}
