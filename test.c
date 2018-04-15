#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "header.h"

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

int main() {


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

abc[0].a = 1;
abc[0].b =2;
abc[1].a = 3;
abc[1].b =4;
printf("abc[0]: %d  %d \n",abc[0].a, abc[0].b);
printf("abd[1]: %d  %d \n",abc[1].a, abc[1].b);
memset((void*)&abc, 0, sizeof(abc));

printf("%d  %d   %d\n",sizeof(struct ABC), sizeof(abc),sizeof(def) );
printf("%d  %d  %d  %d\n",abc[0].a, abc[0].b, abc[1].a, abc[1].b );

return 0;
}




#include <stdio.h>
#include <stdlib.h>
 
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