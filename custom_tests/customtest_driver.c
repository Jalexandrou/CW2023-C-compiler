#include <stdio.h>

int f(int x);

int main()
{
    printf("%d", f(5));
    return !( 5==f(5) );
}
