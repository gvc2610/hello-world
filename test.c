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

return 0;
}
