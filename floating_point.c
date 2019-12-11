#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
1:sign 8:exponent: 23:mantissa

31: 30-23: 22-0
*/


int main() {

 float a = 263.3;
 int *fptr = (int*)&a;
 int val = *fptr;
 printf("sizeof(float):%d  a_float:%f \n",sizeof(a),a);
 printf("fptr addr: fptr:%p *fptr:%x val:%x\n",fptr,*fptr,val );
 printf("test0:%x test1:%d\n",(val&(0xFF <<23)), (val&(0xFF <<23))>>23 );

 




}