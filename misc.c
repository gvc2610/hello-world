#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

#define NUM 3

//#define g_arr[x]  int g_arr[x];              \
                  for(int i = 0; i <x; i++ ) \
                   { grr[i] = 2; }           \

//g_arr[NUM];

#if NUM == 2
int g_arr[NUM] = {1,1};
#elif NUM == 3
int g_arr[NUM] = {1,1,1};
#elif NUM == 1
int g_arr[NUM] = {1}; 
#endif

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


int max_diff_subarray(int A[], int n) {

  int max_diff = 0, min_so_far = A[0];
   
  int left = 0, right = 0; 

   
  for(int i = 0; i < n; i++) {
      
      if(min_so_far > A[i]) { 
      	 min_so_far = A[i];
         left = i;
      	}

      if((A[i]-min_so_far) > max_diff) {
      	max_diff = A[i] - min_so_far;
      	right = i;
      }
  }
 
 printf("max_diff_subarray: left:%d  right:%d,\n",left,right );
 return max_diff;

}

char arr[255];


bool isAnagram(char* s, char* t) {
    
    int len1 = strlen(s);
    int len2 = strlen(t);
    
    if(len1 != len2) return false;
    
    memset(arr,0,255);
    
    int idx1=0,idx2=0;
    
    while(idx1 < len1) {
        printf("Before idx1:%d arr[(int)t[idx1]]:%d\n",idx1,arr[(int)t[idx1]]);
        arr[(int)s[idx1]]++;
        idx1++;
        printf("After idx1:%d arr[(int)t[idx1]]:%d\n",idx1,arr[(int)t[idx1]]);
    }
    
    while(idx2<len2) {
         printf("idx2:%d arr[(int)t[idx2]]:%d\n",idx2,arr[(int)t[idx2]]);
        if(arr[(int)t[idx2]] <= 0) 
            return false;
        
        arr[(int)t[idx2]]--;
        idx2++;       
    }
    
    return true;
    
}

#define GET_CHAIN_NUM(x) (x)==0x1?0:(x)==0x2?1:(x)==0x4?2:(x)==0x8?3:GET_CHAIN_NUM_EXT(x)
#define GET_CHAIN_NUM_EXT(x)  (x)==0x10?0:(x)==0x20?1:(x)==0x40?2:(x)==0x80?3:4


int comparator(void * p, void *q) {
	int a = *(int *)p;
	int b = *(int *)q;
	return a-b;
}

int solution(int A[], int N) {
    // write your code in C99 (gcc 6.2.0)
    
    qsort(A,N,sizeof(A[0]),comparator);
   // int temp = A[0];
    int idx = 0;
    
    while(A[idx] <= 0 && idx < N )
       idx++;
       
    printf("idx:%d\n",idx);   

    if(idx == N) return 1;
    
    if(A[idx] > 1  ) return 1;
    for(int i = 0; i < N; i++) {
        printf("A[%d]:%d\n",i,A[i]);
    }
    while(idx < N-1) {
        if((A[idx+1] - A[idx]) == 0 || (A[idx+1] - A[idx]) == 1)
        {idx++; printf("idx1:%d\n", idx);}
        else {  
            printf("idx2:%d\n", idx);
            return A[idx] + 1;
            
        }
    }
    return A[idx] + 1;

} 


double myPow(double x, int n) {
    double res = 1.0;
    for (int i = n; i != 0; i /= 2) {
        if (i % 2 != 0) res *= x;
        x *= x;
        printf("i:%d res:%f\n",i,res );
    }

    return n < 0 ? 1 / res : res;
}


void diamond(int lines) {
    int i, j, spaces, starts;
    if (lines %2 == 0) lines++;
 
    spaces = (lines-1)/2;
    starts = 1;
    for (i = 1; i <= (lines+1)/2; i++) {
        for (j=spaces; j; j--)
            printf(" ");
        for (j=starts; j; j--)
            printf("*");
        printf("\n");
        spaces--;
        starts+=2;
     }
     spaces = 1;
     starts = lines-2;
     for (i = 1; i <= (lines-1)/2; i++) {
         for (j = spaces; j; j--)
             printf(" ");
         for (j = starts; j; j--)
             printf("*");
         printf("\n");
         spaces++;
         starts-=2;
     }
}


struct test_size {
  long int b;
  char ch;
  int a;
};


int * test_malloc(int n) {

  int * data = (int *)malloc(sizeof(int)*n);

  for(int i =0;i<n;i++) {
    data[i] = i;
  }

  return data;
}


typedef struct 
{
 int a;
 int b;
 int c; 
} int_struct;

typedef struct 
{
  int a;
  char b;
  int c;
}mixed_struct;


int main() {

  char buff_arr[] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
  printf("buff_arr:%x\n",  *(int16_t *)(&buff_arr[3]) );

  int_struct * int_struct_t;
  mixed_struct * mixed_struct_t;

  int_struct_t = (int_struct *) buff_arr;
  mixed_struct_t = (mixed_struct *) buff_arr;

  printf("int_struct a:%x b:%x c:%x \n",int_struct_t->a,int_struct_t->b,int_struct_t->c );
  printf("mixed_struct a:%x b:%x c:%x\n",mixed_struct_t->a,mixed_struct_t->b,mixed_struct_t->c );

   int n =10;

  int * data = test_malloc(n);

  for(int i =0;i<n;i++) {
   printf("test_malloc:%d\n",data[i]);
  }


  diamond(8);
  printf("size of test_size:%d\n", sizeof(struct test_size));

  char *str = NULL;
  char null_char = '\0';

  if(NULL == '\0'){
    printf("NULL = '\0'\n");

    if(str == '\0')
      printf("NULL end of str tested\n");

    if(NULL == (void*)0)
      printf("NULL pointer tested\n");

  } else printf("Not equal\n");


int A[2][3] = {{1,2,3},{4,5,6}};  // *(*(A+x)+y)

for(int i =0; i<2;i++)
  for(int j=0;j<3;j++)
    printf("A[%d][%d]:%d\n",i,j,A[i][j]);

printf("A:%p &A[0][0]:%p &A[0]:%p A[0]:%p A[1]:%p *(*(A+1)+2):%d\n",A,&A[0][0],&A[0],A[0],A[1],*(*(A+1)+2) );
printf("A+1:%p &A[0][0]+1:%p &A[0]+1:%p A[0]+1:%p\n",A+1,&A[0][0]+1,&A[0]+1,A[0]+1 );

int *ptr = &A[0][0];
int *ptr1 = &A[1][0]; 
//ptr[0]= &A[0][0];
printf("ptr:%p ptr+1:%p A[0][0]:%d  A[1][0]:%d  \n",ptr,ptr+1,ptr[2], *(ptr) );
printf("ptr1:%p ptr1+1:%p A[0][0]:%d  A[1][0]:%d \n",ptr1,ptr1+1,ptr1[2], *(ptr1) );



char str4[10] = "abcd";

printf("strlen:%d\n", strlen(str4));

 
/*
 printf("GET_CHAIN_NUM:%d %d %d %d\n",GET_CHAIN_NUM(1),GET_CHAIN_NUM(2),GET_CHAIN_NUM(4),GET_CHAIN_NUM(8));
 printf("GET_CHAIN_NUM_EXT:%d %d %d %d\n",GET_CHAIN_NUM(16),GET_CHAIN_NUM(32),GET_CHAIN_NUM(64),GET_CHAIN_NUM(128));

int a[] = {-2, -3, 4, -1, -2, 1, 5, -3};
    int n = sizeof(a)/sizeof(a[0]);

printf("max_sum_subarray:%d\n",max_sum_subarray(a,n) );

char *ch_arr = "34";
char arr[] = "99";
int iarr[] = {1,2,3,4,5};

printf("%d\n",INT_MAX );

printf("%lu   %lu    %lu    %zu    %zu\n",sizeof(arr),sizeof(iarr),sizeof(ch_arr), strlen(ch_arr), strlen(arr) );
	printf("a2i:%d\n",a2i(arr) );

typedef struct {
    unsigned int    b1:1;
    unsigned int    b2:8;
    unsigned int    b3:7;
    unsigned int    b4:8;
    unsigned int    b5:7;
    unsigned int    b6:1;} BIT_STRUCT;

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

BIT_STRUCT bit_struct;
memset(&bit_struct,0,sizeof(BIT_STRUCT));
bit_struct.b1 = 1;

ch = (char*)(&bit_struct);
if(ch[0])
printf("BIT_STRUCT little endian\n");
else
printf("BIT_STRUCT big endian\n");


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


int arr1[]  = {1,5,10,-2,12,8};//{80,2,6,3,100};

printf("max difference %d:\n",max_diff_subarray(arr1,sizeof(arr1)/sizeof(int)));

char str1[] = "aabbbb",str2[] = "aaaabb";


printf("isAnagram:%d\n",isAnagram(str1,str2) );
int size[3];
printf("%d\n",sizeof(size));


int Array[2][3] = {{1,2,3},{4,5,6}};

for(int i= 0; i < 2; i++) 
	for(int j=0; j<3; j++) {
		printf("Array: %d\n",Array[i][j] );
	}

	memset((void*)&Array[0][0],0,sizeof(Array)); 

	for(int i= 0; i < 2; i++) 
	for(int j=0; j<3; j++) {
		printf("Array: %d\n",Array[i][j] );
	}


   int Arr[] = {-1,0,3,1,2,1,1,5,8,9,3};

  qsort(Arr,10,sizeof(Arr[0]),comparator); 

  for(int i = 0; i < 10; i++) {
  	printf("Arr[%d]%d\n",i,Arr[i] );
  }

  printf("sol:%d\n",solution(Arr,10));


  struct StrucArr {
  	int a;
  	int b;
  };

  struct StrucArr SA[3][4];

  for(int i =0;i<3;i++)
  	for(int j=0;j<4;j++) {
  		SA[i][j].a = 1; 
        SA[i][j].b = 2;
  	}

memset((void *)&SA[1][0],0,sizeof(SA[0]));

  for(int i =0;i<3;i++)
  	for(int j=0;j<4;j++)
  	printf("SA[%d][%d]:%d  %d\n",i,j,SA[i][j].a,SA[i][j].b);

  int hex = 0x08;
  hex >>= 1;
  printf("hex:%x\n", hex);
  printf("%f\n", myPow(3,5));

 for(int i =0; i <NUM; i++)
  printf("%d\n",g_arr[i] );

*/

  
}