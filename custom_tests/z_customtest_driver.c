#include <stdio.h>

int f(int x, int y, int z, int p, int q);

int main()
{
    printf("%d", f(5, 30, 10, 2, 4));
    return !( 30==f(5, 30, 10, 2, 4));
}
