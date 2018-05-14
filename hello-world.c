#include <stdio.h>
#include <string.h>


int add(int a,int b) {
	return a+b;
}
int sub(int a,int b) {
	return a-b;
}

int main()
{
	/* code */
	printf("Hello World\n");
    
    int (*funptr)(int,int);
    funptr = add;
    printf("funptr:%d  add:%d\n",funptr(2,3),add(2,3));

   char str[] = "abc";
   printf("%s\n",str ); 

   char str1[6];
   strncpy(str1,str,3);
   printf("%s\n",str1 );
   
   char str2[6];
   str2[0] = 'd';
   str2[1] = 'e';
   str2[2] = '\0';
   printf("%s\n",str2 );

   char str3[] = "check";
   printf("%s\n",str3 );
   str3[2] = 'p';
   printf("%s\n",str3 );

   char *str4 = "check";
   printf("%s\n",str4 );
  
  //Segfault
  // str4[2] = 'p';
  // printf("%s\n",str4 );   

	return 0;
}