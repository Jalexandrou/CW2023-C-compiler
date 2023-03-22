#include <stdio.h>

int f(int x, int y, int z);

int main()
{
    printf("%d", f(5, 1, 2));
    return !( 5==f(5, 1, 2) );
}
