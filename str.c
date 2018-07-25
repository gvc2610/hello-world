// C program to check if two given strings are rotations of 
// each other
# include <stdio.h>
# include <string.h>
# include <stdlib.h>



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
char* StrStr(char *str, char *substr)
{
	  while (*str) 
	  {
		    char *Begin = str;
		    char *pattern = substr;
		    
		    // If first character of sub string match, check for whole string
		    while (*str && *pattern && *str == *pattern) 
			{
			      str++;
			      pattern++;
		    }
		    // If complete sub string match, return starting address 
		    if (!*pattern)
		    	  return Begin;
		    	  
		    str = Begin + 1;	// Increament main string 
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

 strcat(str3,str1);
 strcat(str3+str1_len,str1);
 char *found = strstr(str3,str1);
 free(str3);
 found==NULL? 0:1;

}

/* Driver program to test areRotations */
int main()
{
	char *str1 = "AACD";
	char *str2 = "ACDA";

	if (areRotations(str1, str2))
	printf("Strings are rotations of each other\n");
	else
	printf("Strings are not rotations of each other\n");

    char *str3 = "abcdefgab";
    char *str4 = "gab";
    printf("%s\n",strstr1(str3,str4) );

	//getchar();
	return 0;
}
