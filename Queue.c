#include <stdio.h>
#include <stdlib.h>
/*
Queue
-------------------------
front->1->2->3-4...->rear 
-------------------------
Array indices:
0 1 2 3 4 5 ...

*/


typedef struct {
    int * buff;
    int front;
    int rear;
    int maxlen;
} Queue_t;


Queue_t * initCircQ(int size) {

	Queue_t * Q = (Queue_t*) malloc(sizeof(Queue_t));
    Q->buff = (int*)malloc(sizeof(int)*size);
    Q->front = -1;
    Q->rear = -1;
    Q->maxlen = size;
    return Q;
}

int isFull(Queue_t *Q) {
	if ((Q->rear + 1 + Q->maxlen)%(Q->maxlen) == Q->front)
        return 1;
    else return 0;
}

int isEmpty(Queue_t *Q) {
	if(Q->front== -1 && Q->rear == -1 )
		return 1;
	else return 0;
}

int enQueue(Queue_t *Q, int q) {

if(isFull(Q)) {
    printf("%s\n","Q is full" );
	return -1;
 }

else {
	if(isEmpty(Q)) Q->front += 1;     
	  
	  Q->rear = (Q->rear+Q->maxlen+1) % (Q->maxlen);
	  printf("rear:%d\n",Q->rear );
	  Q->buff[Q->rear] = q;
	  }

 }

void printQ(Queue_t *Q) {
	if(isEmpty(Q)) {
		printf("%s\n", "Q is Empty, nothing to print");
		return;
	}

	for(int i = Q->front; i<=Q->rear; i++) {
		printf("printing Q[%d]:%d\n",i,Q->buff[i] );
	}
}

int deQueue(Queue_t *Q) {
	if(isEmpty(Q)) {
		printf("%s\n","Q is Empty" );
	}

   else if(Q->front == Q->rear) {
        int temp = Q->front;
   		Q->front = -1, Q->rear =-1;
   	    return Q->buff[temp]; 
       	}

    else {
    	int temp1 = Q->front; 
    	Q->front = (Q->front+1+Q->maxlen)%Q->maxlen;
    	return Q->buff[temp1]; 
    }

}

void deleteQueue(Queue_t *Q) {
	free(Q->buff);
	free(Q);
}

int main() 
{

Queue_t *Q = initCircQ(5);

enQueue(Q,200);
enQueue(Q,300);
enQueue(Q,400);
printf("front:%d rear:%d\n",Q->front,Q->rear );
enQueue(Q,500);
printQ(Q);

enQueue(Q,600);

printf("deQueue():%d\n",deQueue(Q));
printf("deQueue():%d\n",deQueue(Q));
printf("deQueue():%d\n",deQueue(Q));

printQ(Q);
printf("front:%d rear:%d\n",Q->front,Q->rear );
printf("deQueue():%d\n",deQueue(Q));

printQ(Q);

deQueue(Q);
deQueue(Q);

deleteQueue(Q);

}