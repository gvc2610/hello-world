// C program to check if two given strings are rotations of 
// each other
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>



char * strstr1(char *str2,char *str1) {

	int str2_len = strlen(str2);
	int str1_len = strlen(str1);
	int cnt = 0;

	if(str1_len == 0 || str2_len == 0) return NULL;

	while(cnt < str2_len) {
	  int itr = 0;
	  int start_idx = 0;

	  if(str2[cnt] == str1[itr]) {
	  	start_idx = cnt;
	  	while(itr < str1_len) {
	  		if(str2[cnt+itr] == str1[itr]) {
	  			itr++;}  
        }
        if(itr==str1_len) return str2+start_idx;
	  } cnt++;
	} 
 
  return NULL;

}

/* Driver Function */ 
bool StrStr(char *str, char *substr)
{
	  while (*str) 
	  {
		    char *Begin = str;
		    char *pattern = substr;

		    printf("%s   %s\n",Begin,pattern );		    
		    // If first character of sub string match, check for whole string
		    while (*str && *pattern && *str == *pattern) 
			{
			      str++;
			      pattern++;
		    }
		    // If complete sub string match, return starting address 
		    if (!*pattern) {
		    	  return true;
		    	  }
		    str = Begin + 1;	// Increament main string 
		    pattern = substr;
	  }
	  return NULL;
}
 

/* Function checks if passed strings (str1 and str2)
are rotations of each other */
int areRotations(char *str1, char *str2)
{

int str1_len = strlen(str1);
int str2_len = strlen(str2);

 char *str3 = (char*)malloc(str1_len*2 + 1 );

 strcpy(str3,str1);
 strcat(str3,str1);
 printf("%s\n",str3 );
 bool found = StrStr(str3,str2);
 free(str3);
 printf("chk2:%d\n",found );
 
 return found;

}

// Function to implement strncpy() function
char* strncpy(char* destination, const char* source, size_t num)
{
	// return if no memory is allocated to the destination
	if (destination == NULL)
		return NULL;
 
	// take a pointer pointing to the beginning of destination string
	char* ptr = destination;
 
	// copy first num characters of C-string pointed by source
	// into the array pointed by destination
	while (*source && num--)
	{
		*destination = *source;
		destination++;
		source++;
	}
 
	// null terminate destination string
	*destination = '\0';
 
	// destination is returned by standard strncpy()
	return ptr;
}


int compressed_count(char *str) {

 int len = strlen(str);
 if(len == 0) return 0;
  
 int unique = 0;
 char last = str[0];
 int i = 0;

while(str[i]!=0) {
     
    while(last == str[i]) {
     	i++;
     }
     unique++; last = str[i];
    
 }
return unique;

}


char * compressed_str(char *str) {
 
 int len = strlen(str);
 if(len == 0) return NULL;
 int cmprsd_cnt = compressed_count(str)*2;
  if(cmprsd_cnt >= len)
  return str; 

  char * str_buff = malloc(cmprsd_cnt + 1);
  printf("cmprsd_cnt+1:%d\n",cmprsd_cnt +1 );
  
  int count = 0;
  char last = str[0];
  int index = 0;
  int i = 0;
  //str_buff[index] = str[0];

  while(str[i]!='\0') {

  	while(last == str[i]) {
  		i++; count++;
  	  }

  	  str_buff[index] = last;
  	  last = str[i];
  	  index++;
  	  str_buff[index] = (int)'0' + count;
  	  index++; count = 0 ; 
}
  str_buff[index] = '\0';
return str_buff;

}

void reverse_str(char *str, int start, int end) {
   
   if(str == NULL) return;
   char temp;
   while(start <= end) {
      temp = str[start];
      str[start] = str[end];
      str[end] = temp;
      start++,end--;
   }
}

char * rotate_str(char * str,int n) {

	if(str == NULL) return NULL;

	int l = strlen(str);
    
    if(n >= l || n==0) return str;

     
    reverse_str(str,0,l-n-1);
    reverse_str(str,l-n,l-1);
    reverse_str(str,0,l-1);

    return str;

}

/* Driver program to test areRotations */
int main()
{
	char *str1 = "abacd";
	char *str2 = "abda";

	if (areRotations(str1, str2))
	printf("Strings are rotations of each other\n");
	else
	printf("Strings are not rotations of each other\n");
   
    char *str3 = "abcdefgab";
    char *str4 = "gab";
    printf("%s\n",strstr1(str3,str4) );

    char cpr_str[] = "aaaaaab";

    printf("unique:%d\n",compressed_count(cpr_str) );

   printf("compressed string:%s\n",compressed_str(cpr_str));

   printf("%d\n", NULL!='\0' );
    
	//getchar();
  char rt[] = "abcdefg";
   for(int i=0;i<strlen(rt)+2;i++)
   printf("i:%d %s\n",i,rotate_str(rt,i) );
  
   char *null_str = "";
   if(null_str == NULL || null_str == "") printf("null_str1\n");
   //if(null_str == "") printf("null_str1\n");

   //bool_t *t;
   bool *b1;
   bool b = true;

   b1 = &b;
   printf("bool:%d\n",*b1 );

	return 0;
}
