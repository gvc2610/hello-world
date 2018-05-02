#include <stdio.h>
#include <stdlib.h>


/*typedef struct 
{
	char ch[6];
    int i;
}foo;
*/

typedef struct
{ 
    char a[6];
    int b;
} foo;

void main()
{
    printf( "%d\n", ((foo*)0) + 7 );
    printf( "%d\n", &(((foo*)0)->a[4]) );
    printf( "%d\n", &(((foo*)0)->b) );
}



// int main() {

// int a;

// printf("%p\n",(int*)0+1 );

// printf("sizeof(foo):%lu\n", sizeof(foo));
// printf("(*foo)0+1:%p\n",((foo*)0+3) );

// printf("(*foo)0+1:%p\n",&((foo*)0)->ch[4] );
// printf("(*foo)0+1:%p\n",&((foo*)0)->i );

//return 0;
//}