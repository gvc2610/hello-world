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


foo f1 = {.a = "abc",.b = 5};
void main()
{
	printf(" %d\n",sizeof(foo) );
    printf( "%p %p\n",&f1, &f1+ 2 );
    printf( "%d\n", ((foo*)0) + 7 );
    printf( "%d\n", &(((foo*)0)->a[4]) );
    printf( "%d\n", &(((foo*)0)->b) );


foo *ptrf1 = malloc(sizeof(foo));
foo *ptrf2 = malloc(2*sizeof(foo));


ptrf1->a[0] = 'a';
ptrf1->a[1] = 'b';
ptrf1->a[2] = '\0';
ptrf1->b = 1;
printf("%s\n",ptrf1->a );

ptrf2[0].a[0] = 'd';
ptrf2[0].a[1] = 'e';
ptrf2[0].a[2] = '\0';
ptrf2[0].b = 2;

printf("%s\n",ptrf2[0].a );

ptrf2[1].a[0] = 'f';
ptrf2[1].a[1] = 'g';
ptrf2[1].a[2] = '\0';
ptrf2[1].b = 3;
printf("%s\n",ptrf2[1].a );


 struct ex_struct {
        char element1;  // 1 byte
        int element2;   // 4 bytes
        long element3;  // 8 bytes
    };
    union ex_union {
        char element1;
        int element2;
        long element3;
    };
 
    struct ex_struct a;
    union ex_union b;
 
    printf("Size of structure: %d\n", sizeof(a));
    printf("Size of union: %d\n", sizeof(b));
 
    a.element1 = 'D';
    a.element2 = 59;
 
    b.element1 = 'D';
    b.element2 = 59;
 
    printf("a.element1 = %c\n", a.element1);
    printf("a.element2 = %d\n", a.element2);
    printf("b.element1 = %c\n", b.element1);
    printf("b.element2 = %d\n\n", b.element2);
 
    b.element3 = 0x333333333;
    b.element1 = 0x11;
    printf("b.element3 = 0x%x\n", b.element3);
    printf("b.element1 = 0x%x\n", b.element1);
 

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