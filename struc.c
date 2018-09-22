#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*typedef struct 
{
	char ch[6];
    int i;
}foo;
*/

typedef struct
{ 
    char a[6];
    char c;
    int b;
} foo;

struct check {
int *ptr;
int a;
};


void print_check(struct check chk) {
	printf("inside: chk.a:%d   chk.ptr:%p   *chk.ptr:%d\n",chk.a,chk.ptr,*(chk.ptr));
	*(chk.ptr) = 6;
}

foo f1 = {.a = "abc",.b = 5};

struct test {
	int test_val;
};

void test_fun(struct test *val) {
struct test *val1 = val;
	val1->test_val = 2;
}

void main()
{
	

   struct test *test_struc, test_struc1;
    test_struc1.test_val = 1;
   test_struc = &test_struc1;
   

   test_fun(test_struc);
   
   printf("test_struc->val:%d\n",test_struc->test_val );

	printf(" %d\n",sizeof(foo) );
	printf("sizeof(foo.a) :%d\n",sizeof(f1.c) );
    printf( "%p %p\n",&f1, &f1+ 2 );
    printf( "%d\n", ((foo*)0) + 7 );
    printf( "%d\n", &(((foo*)0)->a[4]) );
    printf( "%d\n", &(((foo*)0)->c) );
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


    int num = 5;
    int *ptr_num = &num;
    struct check struc = {.ptr = &num,.a =1};
    
    printf("%p   %d\n",ptr_num,*ptr_num );
    print_check(struc);
    printf("%p   %d  %d\n",ptr_num,*ptr_num, *(struc.ptr) );

    struct output {
    uint8_t max;
    uint8_t min;
  // uint8_t padding[6];
    uint64_t sum;
};
printf("sizeof output:%d\n",sizeof(struct output));

struct output1 {
    uint8_t max;
   // uint8_t min;
  // uint8_t padding[6];
    uint64_t sum;
};

printf("sizeof output1:%d\n",sizeof(struct output1));

struct output2 {
    uint8_t max;
    int min;
  // uint8_t padding[6];
    uint64_t sum;
};

printf("sizeof output2:%d\n",sizeof(struct output2));

// structure A
typedef struct structa_tag
{
char    c;
short int  s;
} structa_t;

// structure B
typedef struct structb_tag
{
short int  s;
char    c;
int     i;
} structb_t;

// structure C
typedef struct structc_tag
{
char    c;
double   d;
int 	 s;
} structc_t;

// structure D
typedef struct structd_tag
{
double   d;
int 	 s;
char    c;
} structd_t;

printf("sizeof(structa_t) = %d\n", sizeof(structa_t));
printf("sizeof(structb_t) = %d\n", sizeof(structb_t));
printf("sizeof(structc_t) = %d\n", sizeof(structc_t));
printf("sizeof(structd_t) = %d\n", sizeof(structd_t));


typedef struct structure1 
{
int id1;
int id2;
char name;
char c;
float percentage;
} struc1;
 
typedef struct structure2 
{
int id1;
char name;
int id2;
char c;
float percentage;
} struc2;

struc1 A1;
struc2 B1;

printf("size of structure1 in bytes : %d\n",sizeof(struc1));
printf ( "\n   Address of id1        = %u", &A1.id1 );
printf ( "\n   Address of id2        = %u", &A1.id2 );
printf ( "\n   Address of name       = %u", &A1.name );
printf ( "\n   Address of c          = %u", &A1.c );
printf ( "\n   Address of percentage = %u", &A1.percentage );

printf("\n\nsize of structure2 in bytes : %d\n",sizeof(struc2));
printf ( "\n   Address of id1        = %u", &B1.id1 );
printf ( "\n   Address of name       = %u", &B1.name );
printf ( "\n   Address of id2        = %u", &B1.id2 );
printf ( "\n   Address of c          = %u", &B1.c );
printf ( "\n   Address of percentage = %u", &B1.percentage );


struct test1 {
	int a;
	char b;
	int c;
};

struct  test2
{
	int a;
	int b;
	char c;
	
};

printf("\nsize of test1:%d test2:%d\n",sizeof(struct test1),sizeof(struct test2) );

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