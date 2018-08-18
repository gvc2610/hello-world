#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

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

void i2a(int num, char A[]) {

	int idx =0;
	while(num) {
		A[idx] = (num%10) + (int)'0';
		num = num/10; idx++;
	}

   A[idx] = '\0';
   int i= 0,j=strlen(A)-1;
   char temp;
   while(i!=j) {
    temp = A[i];
    A[i]=A[j];
    A[j]=temp;
    i++;j--;
   }

}

unsigned int bit_reverse(unsigned int num) { 

  num = ((num & 0xaaaaaaaa) >> 1 ) | ((num & 0x55555555) << 1);
  num = ((num & 0xcccccccc) >> 2 ) | ((num & 0x33333333) << 2);
  num = ((num & 0xf0f0f0f0) >> 4 ) | ((num & 0x0f0f0f0f) << 4);
  num = ((num & 0xff00ff00) >> 8 ) | ((num & 0x00ff00ff) << 8);
  num = ((num & 0xffff0000) >> 16 ) | ((num & 0x0000ffff) << 16);

  return num;
}

unsigned int bit_reverse1(unsigned int num) {


}

void print_powerset(char *str) {
	int n = strlen(str);
	printf("strlen: %d\n",n );


	for(int i=0; i< pow(2,n) ; i++) {
         int n = i, cnt = 0;
		while(n) {
	      if(n&0x1) printf("%c",str[cnt]);
	      n = n>>1; cnt++;
		}
      printf("\n");
		
	}
}

int set_bits(unsigned int n) {
int cnt = 0;
	while(n) {
		cnt++;
		n = n&(n-1);
	}
return cnt;
}

int even_bits(uint32_t num) {

 num = num ^ (num >> 16);  printf("num1:%d\n",num );
 num ^= num >> 8;   printf("num2:%d\n",num );
 num ^= num >> 4;   printf("num3:%d\n",num );
 num ^= num >> 2;   printf("num4:%d\n",num );
 num ^= num >> 1;   printf("num5:%d\n",num );

 printf("num:%d\n",num );

 return (num&1)?0:1;

}

uint32_t rightrotate(uint32_t n, int d) {
	return ((n >> d)|(n<<(32-d)));
}

// 1 1 2 3
uint8_t fibo(int n) {

	if(n==0) return 0;
	if(n==1 || n==2) return 1;

    uint8_t f0=1,f1=1,sum = 0;

    n = n-2;
    while(n){
        //check for overflow  f1 < UCHAR_MAX - f0
        if(f1 < UCHAR_MAX-f0 && f0 < UCHAR_MAX) {
        sum = f0+f1;
        f0  = f1;
        f1  = sum;
        --n;
       } else return 255;
    } 

    return sum;


} 

int max_sum_subarray(int A[], int n) {
	
    if(n==1) return A[0];
    if(n==0) return -1;

	int max_sum=A[0],sum_so_far=A[0],s = 0,end=0,start = 0;

    for(int i=1;i<n;i++) {
          
          sum_so_far = sum_so_far+A[i];

    	if(sum_so_far > max_sum) {
    		max_sum = sum_so_far;
    		end = i;
    		start = s;
    	} 
    	
    	if(sum_so_far < 0) {
          sum_so_far = 0;
          s = i+1;
    	}
   }

    printf("n:%d start:%d end:%d\n",n,start,end);

    return max_sum;
}


int main() {

int a[] = {-2, -3, 4, -1, -2, 1, 5, -3};
    int n = sizeof(a)/sizeof(a[0]);

printf("max_sum_subarray:%d\n",max_sum_subarray(a,n) );

char *ch_arr = "34";
char arr[] = "99";
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

unsigned int bits = 0xdeadbeef;
printf(" bits:%x rev_bits:%x  \n",bits,bit_reverse(bits) );

char comb[] = {'a','b','c'}; 

print_powerset(comb);

printf("set_bit_cnt:%d\n",set_bits(15) );

printf("check even_bits:%d\n",even_bits(7) );

printf("char:%c\n",9 +(int)'0' );

char A[10];
i2a(123,A);
printf("i2a:%s\n",A);

char x = '5';
char y = 2 + (int)'0';
int z = x - (int)'0';
printf("%c  %d  %c %d\n",x,x,y,z );
printf("rightrotate:%x\n",rightrotate(0x7,3) );

i = 0;
printf("i:%d\n", i==0 ?0: i==1 ? 1: i==2? 2:-1);

i = -1;
printf("i:%d\n", i==0 ?0: i==1 ? 1: i==2? 2:-1);


i = 1;
printf("i:%d\n", i==0 ?0: i==1 ? 1: i==2? 2:-1);

i = 2;
printf("i:%d\n", i==0 ?0: i==1 ? 1: i==2? 2:-1);

for(int i=0;i<20;i++){
	printf("%d\n",fibo(i) );
}

}