#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>



int main(int argc, char const *argv[])
{
	/* code */

 uint8_t x1, x2, y1, y2, z1, z2; 
/* set x1, x2, y1, y2 here */ 

x2 = 0x00, x1 = 0xC8; // MSB,LSB
y2=  0x00, y1 = 0x64; 

z1 = z2 = 0; 

z1 = x1 + y1; 
if (z1 < x1 || z1 < y1) { 
/* overflow */ 
z2++; 
} 
z2 += x2 + y2; 

printf("z2:%x z1:%x \n",z2,z1 );

	return 0;
}