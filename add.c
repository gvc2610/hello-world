#include "header.h"

int add(int a , int b) {
	return a+b;
}

int x = 5;

int f()
{
 static int x = 0;
 return x;
}
int g()
{
 static int x = 1;
 return x;
}