#include <stdio.h>
#define FACTORIAL_LIMIT 10

int main()
{
    long double e = 1.0, factorial = 1.0;
    const long double one = 1.0;

    int i;
    for (i = 1; i < FACTORIAL_LIMIT; ++i)
    {
        factorial *= i;
        e += one / factorial;
    }

    printf("e is %.62LF\n", e);

    return 0;
}