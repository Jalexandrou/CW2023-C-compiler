#include <stdio.h>

int f();

int g(){
    return 5;
}


int main()
{
    printf("%d", f());
    return !( 5==f() );
}
