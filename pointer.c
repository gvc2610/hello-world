#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "header.h"
#include <string.h>

struct ABC
{
  int a;
  int b;	
};

struct ABC abc[2];
struct ABC def;

int main() {

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
char_dyna[0] = 'a';

printf("char_ptr arr sizeof(char_ptr1):%lu char_ptr1:%p char_ptr1+1:%p &char_ptr1[0]:%p &char_ptr1:%p &char_ptr1+1:%p\n", sizeof(char_ptr1),char_ptr1,char_ptr1+1,&char_ptr1[0],&char_ptr1,(&char_ptr1)+1);


// Dont dare to get the address of char_ptr. It is on the stack. So &char_ptr is of no use 
printf("sizeof(char_str):%d  char_str:%p &char_str:%p &char_str+1:%p &char_str[0]:%p char_str[0]:%c\n",sizeof(char_str),char_str, &char_str,&char_str+1,&char_str[0],char_str[0]);

printf("sizeof(char_dyna):%d char_dyna:%p &char_dyna:%p &char_dyna[0]:%p char_dyna[0]:%c\n",sizeof(char_dyna),char_dyna, &char_dyna,&char_dyna[0],char_dyna[0]);

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


int R = 3, C = 3;
int A[R][C]; 
char cnt = 0;
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			A[i][j] = cnt;cnt++;
		} 
	}


	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			printf("%3d ", *(*(A+i)+j));
		} printf("\n");
	}

// A[R][C] == *(*(A+R)+C)
// A[n] == *(A+n)   
// Think of A as a pointers to 1D As with each row having the address to the 1D A


printf("sizeof(A):%d  A:%p &A:%p &A+2:%p A+1:%p &A[1][0]:%p\n",sizeof(A),A,&A,&A+2,A+1,&A[1][0]);



	char **B;
	B = malloc(R * sizeof(char *));
	if(B == NULL)
		{
		fprintf(stderr, "out of memory\n");
		return -1;
		}
	for(int i = 0; i < R; i++)
		{
		B[i] = malloc(C * sizeof(char));
		if(B[i] == NULL)
			{
			fprintf(stderr, "out of memory\n");
		 return-1;
			}
		}

	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			B[i][j] = cnt;cnt++;
		} 
	}


	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			//printf("%3d ", *(*(B+i)+j));
			printf("%3d  %10p ", B[i][j],&B[i][j]);
		} printf("\n");
	}
//sizeof() doesnt work on dynamically allocated memory
printf("sizeof(B):%d  B:%p *B:%p &B:%p B+1:%p &B[1][0]:%p\n",sizeof(B),B,*B,&B,B+1,&B[0][0]);


int a = 5;
int c;
c = a==5?TRUE:FALSE;
printf("c:%d\n",c );


}