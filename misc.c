#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

int a2i(char A[]) {

int n = strlen(A);
printf("%d\n", n );

int num = 0;
int delta = 0;

for (int i = n - 1; i >= 0 ; i--) {
    if (A[i] >= '0' && A[i] <= '9') {

        if (i == (n - 1)) {num = (int)A[i] - (int)'0'; 
        	//printf("num:%d\n",num );
          } else { 

          	delta = ((int)A[i] - (int)('0')) *pow(10, n - 2 - i);

            if ((delta) < INT_MAX / 10 && num <  INT_MAX - (delta * 10) && num >=0) {
            	//printf("%d  %d\n",num, delta );
                num = num + delta * 10;
            } else {printf("Out of limits!\n"); return 0;}
     } 

   } else {printf("Invalid string\n"); return 0;}
}

return num;

}


int main() {

char *ch_arr = "34";
char arr[] = "99999999";
int iarr[] = {1,2,3,4,5};

printf("%d\n",INT_MAX );

printf("%lu   %lu    %lu    %zu    %zu\n",sizeof(arr),sizeof(iarr),sizeof(ch_arr), strlen(ch_arr), strlen(arr) );
	printf("a2i:%d\n",a2i(arr) );



int endian = 0x01234567;
int reverse_endian =  ((endian & 0xff) << 24u) | ((endian & 0xff00) << 8u) | ((endian & 0xff000000) >> 24u) | ((endian & 0x00ff0000) >> 8u); 
printf("endian:%x  reverse_endian:%x\n",endian, reverse_endian );


int i = 0x1;
int *ptr = &i;

char *ch = (char*) ptr; 

if(ch[0])
printf("little endian\n");
else
printf("big endian\n");


}