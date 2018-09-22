#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

void delete(int num) {
 
 struct Node *temp = head;

 if(temp->val == num) {
 	printf("Deleting head\n");

    struct Node * temp1 = temp->next;
 	head = temp1;
 	free(temp);
 	return;
 }

 while ((temp->next)->val != num) {
 	// /printf("itr:%d\n",(temp->next)->val );
 	temp = temp->next;
 	if (temp->next==NULL) {
 		printf("No matching value\n");
 		return; 
 		} 
 }

 struct Node *temp1 = temp->next;
        temp->next = temp1->next;
        printf("Deleting Node:%d\n",temp1->val );
        free(temp1);

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

struct ListNode
{
	int val;
	struct ListNode *next;
};



void reorderList(struct ListNode* head) {

    if(head == NULL) return;
     
    struct ListNode *slow, *fast,*temp2;
    slow = fast = head;
    
    if(slow->next == NULL || slow->next->next == NULL) return;
        
    
    //fast = 2*slow
    while(fast->next!= NULL && fast->next->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    } 
    
    // Val: slow = 3 fast = NULL   {head = 1->2->3->NULL}
    
    //slow points to mid of list
 
    struct ListNode *head1 = slow->next;  //head1 = 4 {4->5->NULL}
    slow->next == NULL; 
    
     printf("head1->val:%d\n",head1->val);
    
            temp2 = head;
    while(temp2!=NULL) 
    {
        printf("temp2->val:%d\n",temp2->val);
        temp2 = temp2->next;
    }
    
    temp2 = head1;
    while(temp2!=NULL) 
    {
        printf("head1 temp2->val:%d\n",temp2->val);
        temp2 = temp2->next;
    }
    
    //reverse the list from slow->next to end of list
   struct ListNode *curr,*next,*prev;
    curr = head1;
    //next = head1;
    prev = NULL;
    
      while(curr!=NULL) {
          next = curr->next;
          curr->next = prev;
          prev = curr;
          curr = next;
      }head1 = prev;  
        
    // head1 = 5 {5->4->NULL}
     
    // head1 is pointing to next of middle element
   
                temp2 = head1;
    while(temp2!=NULL) 
    {
        printf("revserse temp2->val:%d\n",temp2->val);
        temp2 = temp2->next;
    }
    
    /*     
    1->4->2->3 idx:2    5  idx1:5
    1->4->2->5->3 
     */
    struct ListNode *idx = head;
    struct ListNode *idx1 = head1;
    struct ListNode *temp,*temp1 = idx1 ;

    while(idx1 != NULL) {
        printf("idx->val:%d idx1->val:%d\n",idx->val,idx1->val);
        temp = idx->next;
        temp1 = idx1;
        
        idx->next = temp1;
        idx1 = idx1->next;
        
        temp1->next = temp;
        idx =  idx->next->next;
    }
    
    printf("idx->val:%d\n",idx->val);
    
    //idx->next = NULL;
     temp1 = head;
      while(temp1!=NULL) 
    {
        printf("temp1->val:%d\n",temp1->val);
        temp1 = temp1->next;
    }  
}

struct ListNode* removeNthFromEnd(struct ListNode* head, int n) {
    
    assert(head);  
    struct ListNode* pre, *cur;  
    pre = head;cur = head;  
    int step = 0;  
    while(step< n && cur!=NULL)  
    {  
    cur = cur->next;  
    step++;  
    }  
    if(step ==n && cur == NULL)  
    {  
    head = head->next;  
    free(pre);  
    return head;  
    }  
    while(cur->next!=NULL)  
    {  
    pre = pre->next;  
        cur = cur->next;  
    }  
    struct ListNode* temp = pre->next;  
    pre->next = temp->next;  
    free(temp);      
    return head;
    
}

int main()
{
insert(1);
insert(2);
insert(3);
insert(4);
insert(5);
insert(6);
insert(7);
insert(8);
/*
print_list();
reverse();
print_list();

reverse();
delete(1);
print_list();
delete(8);
print_list();
delete(6);
*/
//reverse();
print_list();
printf("HEAD:%d\n",head->val );

//reorderList((struct ListNode*)head);


struct Node *temp;
struct Node *arr = (struct Node *) malloc(sizeof(struct Node)*2);
arr[0].val =  1;
arr[1].val = 2;
temp = arr;

// arr[0].val = 1;
// arr[1].val = 2;

// printf("%d\n",temp. );

}