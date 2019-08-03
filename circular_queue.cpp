//C++ program for insertion and 
// deletion in Circular Queue 
#include <bits/stdc++.h> 
using namespace std; 

// Structure of a Node 
struct Node 
{ 
	int val; 
	struct Node* next; 
}; 

struct Queue 
{ 
	struct Node *front;
	struct Node *rear; 
	int max_size, curr_size;
}; 

// Function to create Circular queue 
void enQueue(Queue *q, int value) 
{ 
	if(q->curr_size >0 && q->curr_size >= q->max_size ) {
		printf("Queue Full!! cannot enque:%d\n",value);
		return;
	}
	struct Node *temp = (struct Node*)malloc(sizeof(Node)); 
	temp->val = value; 
	if (q->front == NULL) 
		q->front = temp; 
	else
		q->rear->next = temp; 

	q->rear = temp; 
	q->rear->next = q->front;
	(q->curr_size)++; 
} 

// Function to delete element from Circular Queue 
int deQueue(Queue *q) 
{ 
	if (q->front == NULL) 
	{ 
		printf ("Queue is empty"); 
		return INT_MIN; 
	} 

	// If this is the last node to be deleted 
	int value; // Value to be dequeued 
	if (q->front == q->rear) 
	{ 

		printf ("Queue last node being deleted:%d",q->front->val);
		value = q->front->val; 
		free(q->front); 
		q->front = NULL; 
		q->rear = NULL;
		q->curr_size = 0; 
	} 
	else // There are more than one nodes 
	{ 
		struct Node *temp = q->front; 
		value = temp->val; 
		q->front = q->front->next; 
		q->rear->next= q->front; 
		free(temp); 
	} 
    
    --(q->curr_size);
	return value ; 
} 

// Function displaying the elements of Circular Queue 
void displayQueue(struct Queue *q) 
{ 
	struct Node *temp = q->front; 
	printf("\nElements in Circular Queue of size %d are: ",q->curr_size); 
	while (temp->next != q->front) 
	{ 
		printf("%d ", temp->val); 
		temp = temp->next; 
	} 
	printf("%d", temp->val); 
} 

Queue * Qinit(int size) {
	Queue *q  = (struct Queue *) malloc(sizeof(Queue));
	q->max_size = size;
	q->curr_size = 0;
	q->front = NULL;
	q->rear = NULL;
}

void Qfree(struct Queue *q) {
  
  if(q->front == NULL) return;

  struct Node *temp = q->front;
  struct Node *temp1 =NULL;
  while(temp != q->rear) {
         temp1 = temp;
         temp= temp->next;
         printf("freeing Node:%d\n ",temp1->val );
         free(temp1);
  }
    printf("freeing Node:%d\n",temp->val );
   free(temp);
   free(q);
   q= NULL;
  // q->front = q->rear = NULL;

} 

/* Driver of the program */
int main() 
{ 
	// Create a queue and initialize front and rear 
	Queue *q = Qinit(3); 
	q->front = q->rear = NULL; 

	// Inserting elements in Circular Queue 
	enQueue(q, 14); 
	enQueue(q, 22); 
	enQueue(q, 6); 
	enQueue(q, 4);

	// Display elements present in Circular Queue 
	displayQueue(q); 

	// Deleting elements from Circular Queue 
	printf("\nDeleted value = %d", deQueue(q)); 
	printf("\nDeleted value = %d", deQueue(q)); 

	// Remaining elements in Circular Queue 
	displayQueue(q); 

	enQueue(q, 9); 
	enQueue(q, 20); 
	displayQueue(q); 
	Qfree(q);

	return 0; 
} 
