#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "header.h"
#include <string.h>

#define A_MAX(_x, _y)       ((_x) > (_y) ? (_x) : (_y))

#define A_MIN(_x, _y)       ((_x) < (_y) ? (_x) : (_y))

#define PRINT_TOKEN(token) printf(#token " is %d", token)

#define CHECK 1

#define A_UINT8 uint8_t


struct ABC
{
  int a;
  int b;	
};

struct ABC abc[2];
struct ABC def;

uint8_t computeSetBits(uint16_t *num)
{
    uint16_t count = 0;
    uint16_t temp = *num;
    while (temp)
    {
      temp &= (temp-1) ;
      count++;
    }
    return count;
}

//uint8_t arrayPopulatorFromBitMap(uint8_t *ByteArray, uint16_t *bitArray)
//{
//   return log2(n&-n)+1;/
//}

void arrayPopulatorFromBitMap(uint8_t *ByteArray, uint16_t bitArray)
{
  A_UINT8 array_idx = -1;
  A_UINT8 num_shifts = -1;

  while (bitArray) 
  {
  	num_shifts++;
	  if((bitArray) & 0x1) 
	  	{   array_idx++;
	  		ByteArray[array_idx] = num_shifts;
	    }
	    bitArray >>=  1 ;
   }
}


unsigned int getFirstSetBitPos(int n)
{
 //  return log2(n&-n)+1;
}

typedef enum {
VAL1 =  0x0010,
VAL2 =  0x000A,

} STATE;

bool test(int a) {

return a;
}


void print_array(int a[][3],int m, int n) {

	for (int i = 0; i<m; i++) {
		for(int j=0;j<n;j++) {
			printf("%d\n",a[i][j] );
		}
	}

}

//int val;
static int val;
void f() {
	static int val = 5; 
	val++;
	printf("Inside f : x:%d\n",val );
}

void g() {
	static int val = 10; 
	val++;
	printf("Inside g : x:%d\n",val );
}

void func_void(void *p) {

	//int a = *(int *)p;
	  int a = (int *)p;
	printf("check void:   p:%p a:%d  \n",p,a );
}


void fun_str(char *str) {
	printf("strlen:%d\n",strlen(str) );
    int len = 0,i=0;
	while(str[i] != '\0') 
		len++,i++;
	printf("strlen1:%d\n", len);
}

void print_arr(int n) {
 
 int array1[n];
 for(int i =0;i<n;i++) {
 	array1[i] = i;
 }

 for(int i= 0;i<n;i++) {
 	printf("arr:%d\n",array1[i] );
 }
}

int main() {

char str1[] = "abcd";
fun_str(str1);

int check_void =  5;
//func_void(&check_void);
func_void((void*)1);

printf("global val1:%d\n",val );
f();
g();
val = 3;

printf("global val1:%d\n",val );
f();
g();


printf("VAL1= %d  VAL2=%d \n",VAL1,VAL2);

printf("%d %d\n",test(0x00008000),test(0x00000000) );

printf("%d %d %d %d\n",A_MIN(1,2),A_MAX(1,2),A_MAX(0,0),A_MIN(0,0) );

printf("CHECK:%d\n",CHECK );
printf("!CHECK:%d\n",!CHECK );

#if !CHECK
printf("inside !CHECK");
#endif

#if CHECK
printf("inside CHECK");
#endif

uint16_t i = 6;
printf("setbits %d: %d \n",i,computeSetBits(&i));

uint16_t j = 0xf0f0;
uint16_t *bitArray1 = &j; 
uint8_t ByteArray1[16];

arrayPopulatorFromBitMap(ByteArray1,0xf0f0);

for(int x = 0; x < sizeof(ByteArray1); x++)
	printf("ByteArray1: %d\n",ByteArray1[x]);


int a = -5;

if(a < 0) a = 0;

PRINT_TOKEN(2+3);


int freq[] = {1,2,3};
void *ptr = NULL;

printf("\n%lu %lu  %lu  %lu\n",sizeof(freq),sizeof(*freq),sizeof(ptr),sizeof(*ptr) );

if(NULL == freq )
	printf("NULL pointer" );
else printf("pointing something");

typedef enum num {ZERO,ONE,TWO,THREE} no_;

no_ one = ONE;
no_ two = TWO;

//printf("add:%d\n", add(one,two));

int arr[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
print_array(arr,3,3);



if(NULL == (void*)0)
printf("*0\n");

if(NULL == (void*)1)
printf("*1");

if(NULL == (void*)-1)
	printf("*-1" );

char *char_ptr;
char ch = 'a';
char_ptr =  &ch;
printf("%lu %lu\n\n\n",sizeof(char_ptr), sizeof(*char_ptr) );


char char_ptr1[] = {'a','b','c'};
char *char_str = "asdf";
char *char_dyna = (char*)malloc(3);
char val_a = 5;
int *ptra = &val_a;

printf("char_ptr arr:%lu %p %p %p\n", sizeof(char_ptr1),char_ptr1,char_ptr1+1, &char_ptr1[0]);


// Dont dare to get the address of char_ptr. It is on the stack. So &char_ptr is of no use 
printf("sizeof(char_str):%d  char_str:%p &char_str:%p char_str[0]:%c\n",sizeof(char_str),char_str, &char_str,char_str[0]);

printf("sizeof(char_dyna):%d char_dyna:%p &char_dyna:%p char_dyna[0]:%c\n",sizeof(char_dyna),char_str, &char_str,char_str[0]);

printf("val_a:%d ptra:%p &ptra:%p &ptra +1:%p &val_a:%p  &val_a+1:%p \n",val_a, ptra, &ptra, &ptra+1, &val_a, &(val_a) +1);

abc[0].a = 1;
abc[0].b =2;
abc[1].a = 3;
abc[1].b =4;
printf("abc[0]: %d  %d \n",abc[0].a, abc[0].b);
printf("abd[1]: %d  %d \n",abc[1].a, abc[1].b);
memset((void*)&abc, 0, sizeof(abc));

printf("%d  %d   %d\n",sizeof(struct ABC), sizeof(abc),sizeof(def) );
printf("%d  %d  %d  %d\n",abc[0].a, abc[0].b, abc[1].a, abc[1].b );



int * ptr1 = malloc(sizeof(int));
*ptr1 = 0xdeadbeef;
int val1 = *ptr1;
int val2 = (int) *((char *) ptr1);


printf("val2:%x\n",val2 );


char *char_ptr_check;
printf("char_ptr_check:%p  %d\n",sizeof(char_ptr_check), sizeof(*char_ptr_check));

uint16_t unsign = -3;
int16_t signd = unsign;

printf("unsign:%d  signd:%d\n", unsign, signd );

char arr1[] = "abc";
char arr2[] = "defghi";

arr1[1] = 'z';

*arr1 = *arr2;


printf("%s  %c\n",arr1,arr1[1] );

int bool_check = 1;
printf("bool_check:  %d  %d  %d\n",bool_check,!bool_check,~bool_check );

printf("ptr:%p\n",NULL );


char *x,*y;
x = "abc";
y=x;
// /y[1] = 'd';

unsigned char aa = 0xFF; //check if signed
int b = (int)aa;
char ch1 = 'a';

printf("b:%d\n",b );

#define size(var) \
        ((size_t)(&(var)+1) - (size_t)(&(var)))

// size of type without sizeof operator
#define sizetype(type) \
       ((size_t)(((type *)0) + 1)) 

printf("size of int:%d  char:%d int:%d\n", size(b),size(ch1),sizetype(int));        

#define EMPTY -1

int A[10][5];
memset(A, EMPTY, sizeof(A));
printf("sizeof A:%d  A[1][1]:%d\n",sizeof(A),A[1][4] );


int Arr[16];
unsigned int a0 = (unsigned int) &Arr[0];
unsigned int a3 = (unsigned int) &Arr[3];
printf("p:%u\n", a3 - a0);
printf("%u\n", &Arr[3] - &Arr[0]);


int input;
printf("Input num\n");
scanf("%d",&input);

int arr_check[input];
print_arr(input);



int a_test =5;

typeof(a_test) b_test = 10;

printf("%d\n",b_test );

return 0;
}



 
/* Write a function f(a, b) which takes two character string 
arguments and returns a string containing only the characters
 found in both strings in the order of a. Write a version which
  is order N-squared and one which is order N.
*/
 
 
int getlenght1(const char s[]) { // same as '(const char * const s)'
    int k = 0;
    while(s[k++]); // same as      while(s[k++] != '\0'); 
                   // and same as  while (*(s + k++) != '\0')
    k--;
    printf("size %d\n", k);
    return k;
}
 
int getlenght2(const char * s) {
    const char * s0 = s;
    while (*s++); // wouldnt work if s defined as const char * const s
    printf("size %s: %d\n", s0, (s-s0-1));
    return (s-s0-1);
}
 
char * solution_nsquare(const char * const a, const char * const b) {
    char *common;
    int i, j, k;
    int na = getlenght2(a);
    int nb = getlenght2(b);
    common = malloc(sizeof(*common) * na);
    k = 0;
    for (i = 0; i < na; i++) {
        for (j = 0; j < nb; j++) {
            if (b[j] == a[i]) {
                common[k] = a[i];
                k++;
            }
        }
    }
    common[k] = '\0';
    return common;
}
 
/* Review of bitwise operations (bit manipulation:
    && is logical operator: result is true (1) or false (0)
    & is bitwise operator, applies && to each bit. result is a number
    set bit x:              vble |= (1<<x)
    set bits x and y:       vble |= (1<<x|1<<y)
    clear bit x:            vble &= ~(1<<x)
    toggle (change) bit x:  vble ^= (1<<x)
    check if bit x set:     if(vble & (1<<x))
    get lower x bits:       v = vble & (2**x-1)  
    get higher x bits:      v = vble & ~(2**(16-x)-1)
 */
char * solution_linear(const char a[], const char b[]) {
    int i, letter, k;
    unsigned long bitarray = 0x0;
    i = 0;
    /* scan b string */
    while (b[i]) {
        letter = b[i] - 'a' + 1;
        bitarray |= 0x1<<letter; /* set bit for that letter */
        i++;
    }
    /* now scan a, so common letters are saved in same order as a */
    char *common = malloc(sizeof(char) * i);
    i = 0; k = 0;
    while (a[i]) {
        letter = a[i] - 'a' + 1; // 'x' for characters, "x" for terminated strings
        if (bitarray & (0x1<<letter)) { /* check if bit set */
            common[k++] = a[i];
        }
        i++;
    }
    common[k] = '\0';
     
    /* 'common' was allocated dynamically, so it won't be erased when function
        returns. But it needs to be freed in main */
    return common;
}


/*     
int main() {
    char *a = "asdfqwer";
    char b[] = "skelrpfa";
    char *common1 = solution_nsquare(a, b);
    char *common2 = solution_linear(a, b);
    printf("a: %s\nb: %s\ncommon square: %s\ncommon linear: %s\n",
            a, b, common1, common2);
    free(common1);
    free(common2);
    return 0;
}     
 
gcc commonchar.c
./a.out 
size asdfqwer: 8
size skelrpfa: 8
a: asdfqwer
b: skelrpfa
common square: asfer
common linear: asfer

*/