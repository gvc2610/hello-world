#include <queue>
#include <stack>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;



void sort_descend(stack<int> * s1, stack<int> * s2) {

if(s1->empty()) return;

int temp = s1->top();
s1->pop();
s2->push(temp);

while(!s1->empty()) {
 temp = s1->top();
 s1->pop();
 
int temp1;
	 while(temp >= s2->top()) {
	  temp1 = s2->top();
	  s2->pop();
	  s1->push(temp1);
	  //printf("temp1:%d\n",temp1 );
	  if(s2->empty()) break;
	   }
	 s2->push(temp);
	}


}


void display(stack<int> *s) {
	
	while(!s->empty()) {
	printf("%d\n",s->top()); 
	s->pop();
	}

}

int main() {

stack<int> s1,s2;

s1.push(6);
s1.push(5);
s1.push(4);
s1.push(3);
s1.push(2);
s1.push(1);


//printf("stack1:\n");
// /display(&s1);

sort_descend(&s1,&s2);
printf("stack2:\n");
display(&s2);

return 0;

}