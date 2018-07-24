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


void node_delete(int num) {

 struct Node *temp = head;

  if(temp->val==num) {
  	head = temp->next;
  	head->prev = NULL;
  	free(temp);
    printf("Deleting node pointed by head\n");
  	return;
  }

  while(temp->next->val != num && temp->next != NULL) {
  	    temp = temp->next;
  }
	
	if(temp->next->val != num) {
		printf("%s\n", "Num not found");
		return;
	} else {
    	struct Node *temp1,*temp2;
      	temp1 = temp->next->next;
        temp2 = temp->next;
    	temp->next = temp1;

    	if(temp1 != NULL) // Only if its not last node
    	temp1->prev= temp;

    	free(temp2);
    }
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


void reverse() {
	struct Node *left,*curr,*right;
	left = NULL;
	curr = head;

	while(curr != NULL) {
        right = curr->next;
        
        curr->next = left;
        curr->prev = right;
        
        left = curr;
        curr= right;

	} head = left;
}

int main() {

	insert(1);
	insert(2);
	insert(3);
	insert(4);
	insert(5);
	struct Node *last_node = traverse_end(head,0);
	traverse_end(last_node,1);

    //node_delete(5);
	last_node = traverse_end(head,0);
	traverse_end(last_node,1);
	printf("Reverse\n" );
	reverse();
	last_node = traverse_end(head,0);
	traverse_end(last_node,1);

}