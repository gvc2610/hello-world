#include <stdio.h>


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



	return 0;
}