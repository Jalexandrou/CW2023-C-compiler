#include <stdio.h>

int f();

int main()
{
    printf("%d", f());
    return !( 10==f() );
}
