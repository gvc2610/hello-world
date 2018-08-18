#include <stdio.h>

int Q[5];
int N = 5;
int front = -1;
int rear = -1;

/*
Queue
-------------------------
front->1->2->3-4...->rear 
-------------------------
Array indices:
0 1 2 3 4 5 ...

*/
int isFull() {
	if ((rear + 1 + N)%N == front)
        return 1;
    else return 0;
}

int isEmpty() {
	if(front== -1 && rear == -1 )
		return 1;
	else return 0;
}

int enQueue(int q) {

if(isFull()) {
    printf("%s\n","Q is full" );
	return -1;
 }

else {
	if(isEmpty()) front += 1;     
	  rear = (rear+N+1)%N;
	  Q[rear] = q;
	  }

 }

void printQ() {
	if(isEmpty())
		printf("%s\n", "Q is Empty, nothing to print");
	for(int i = front; i<=rear; i++) {
		printf("printing Q[%d]:%d\n",i,Q[i] );
	}
}

int deQueue() {
	if(isEmpty()) {
		printf("%s\n","Q is Empty" );
	}

   else if(front == rear) {
        int temp = front;
   		front = -1,rear =-1;
   	    return Q[temp]; 
       	}

    else {
    	int temp1 = front; 
    	front = (front+1+N)%N;
    	return Q[temp1]; 
    }

}
int qFront();
int isEmpty();
int isFull();

int main() 
{
enQueue(100);
enQueue(200);
enQueue(300);
enQueue(400);
printf("front:%d rear:%d\n",front,rear );
enQueue(500);
printQ();

enQueue(600);

printf("deQueue():%d\n",deQueue());
printf("deQueue():%d\n",deQueue());
printf("deQueue():%d\n",deQueue());

printQ();
printf("front:%d rear:%d\n",front,rear );
printf("deQueue():%d\n",deQueue());

printQ();

deQueue();
deQueue();

}