#include <stdio.h>

int f();

int main()
{
    printf("%d", f());
    return !( 12==f() );
}
