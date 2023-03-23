#include <stdio.h>

int f(int x, int y);

int main()
{
    printf("%d", f(5, 5));
    return !( 12==f(5, 5) );
}
