#include <stdio.h>
#include <stdlib.h>

struct Node
{
	int val;
	struct Node * next;
};

struct Node *head = NULL;

void insert(int num) {
 
 struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
 temp->next = NULL;
 temp->val = num;

	if(head == NULL) {
		head = temp;
	}
	else {
		struct Node* itr = head;
		while(itr->next != NULL)
			itr = itr->next;

		itr->next = temp;
	}
} 

void print_list() {
	struct Node *itr = head;
	printf("%s\n","Start printing:");
	while(itr != NULL) {
		printf("%d\n",itr->val);
		itr = itr->next;
	}

}


void reverse() {
   struct Node *prev,*curr,*ahead;

   prev = NULL;
   curr = head;
    
   while(curr!= NULL) {
   	printf("curr:%d\n",curr->val );
   	ahead = curr->next;
    curr->next = prev;
    prev = curr;
   	curr = ahead;
   }
head = prev;

}

int main()
{
insert(1);
insert(2);
insert(3);
insert(4);
insert(5);

print_list();
reverse();
print_list();


struct Node *temp;
struct Node *arr = (struct Node *) malloc(sizeof(struct Node)*2);
arr[0].val =  1;
arr[1].val = 2;
temp = arr;

// arr[0].val = 1;
// arr[1].val = 2;

// printf("%d\n",temp. );

}