#include <stdio.h>
#include <stdlib.h>


struct Node {
	int val;
	struct Node * next;
	struct Node *prev;
};

struct Node * head = NULL;

struct Node * traverse_end(struct Node * head_pointer, int reverse) {
	struct Node *itr = head_pointer;
	printf("Start printing is_reverse:%d\n",reverse);
	
	if(!reverse) {
	while(itr->next != NULL) {
		printf("%d\n",itr->val);
		itr = itr->next;
		if(itr->next == NULL){
			printf("%d\n",itr->val);
			return itr;
		}
	}
  } else {
  	while(itr->prev != NULL) {
		printf("%d\n",itr->val);
		itr = itr->prev;
		if(itr->prev == NULL){
			printf("%d\n",itr->val);
     		return itr;
       }
   }
 }
}


void delete (int num) {

	
}

void insert(int num) {

struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
      temp->val =  num; 
      temp->prev = NULL;
      temp->next = NULL;

	if(head == NULL) {
       head = temp;
	} else {
		struct Node * temp1 = head;
		
		while(temp1->next  != NULL)
			temp1 = temp1->next;

        temp1->next = temp;
        temp->prev = temp1;
        temp->next = NULL;

	}
}


int main() {

	insert(1);
	insert(2);
	insert(3);
	insert(4);
	insert(5);
	struct Node *last_node = traverse_end(head,0);
	traverse_end(last_node,1);
}