#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "header.h"
#include <string.h>

char hash_map[256] = {0};

int check_unique(char A[],int len) {
int n = 0;
	while(n < len) {
		if(hash_map[A[n]] == 0)
		   hash_map[A[n]] = 1;
		else {
			printf("Duplicate char: %c\n",A[n] );
			return true;
		}
        n++;
	}
	return false;
}

char * run_len_enc(char A[], int len) {

	char *res = (char*)malloc(sizeof(2*len+1));

	int cnt = 0;
	char prev = A[0];
	int res_idx = 0;
	int i =0;

    while( i< len) {
       cnt = 1;
       res[res_idx] = A[i];
       
       while(A[i]==A[i+1]){
         cnt++; i++;         
       }
       i++;
       res_idx++;
       res[res_idx] = (int)'0'+cnt;
       res_idx++; 
    }

res[res_idx] = '\0';
return res;

}


void remove_dup(char A[], int n) {

memset(hash_map,0,256);

for(int i = 0; i< n; i++) {
	hash_map[A[i]]++;
}

int idx = 0;
int len = 0;

while(len < n) {

    if(hash_map[A[len]]>0) {
	A[idx] = A[len];
	hash_map[A[len]] = 0;
	idx++;
   } else len++; 

} 

 A[idx] = '\0';

}

int main()  {

char str[] = "chaitu";
char str1[] = "chaitanya";

remove_dup(str,strlen(str));
printf("%s\n",str );

//printf("check dup:%d\n",check_unique(str,strlen(str)));
printf("check dup:%d\n",check_unique(str1,strlen(str1)));

char rle[] = "aaaabbdddd";
printf("%s\n",run_len_enc(rle,strlen(rle)) );



}