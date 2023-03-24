#include <stdio.h>

int f();
int g();

int main()
{
    printf("%d", g());
    return !( 8==g());
}
