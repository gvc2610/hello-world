#include <queue>
#include <stack>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

using namespace std;

struct Node {
	int val;
	struct Node *left;
	struct Node *right;
};


struct Node* creatNode(int val) {
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
	temp->val = val;
	temp->left = NULL;
	temp-> right = NULL;
    return temp;
}

struct Node* insert(struct Node *root, int num) {
    
	if(root == NULL) {
		struct Node *temp = creatNode(num);
		return temp;}
     if (num <= root->val)  
	  root->left =  insert(root->left,num);
	 else root->right = insert(root->right,num);   
   return root;
}

void inorder(struct Node * root) {
	if(root->left) inorder(root->left);
	printf("%d\n",root->val);
	if(root->right) inorder(root->right);
}


void preorder(struct Node *root) {

	if(root == NULL) return;
	else printf("%d\n", root->val);

	preorder(root->left);
	preorder(root->right);
}

void level_order(struct Node *root) {


     queue <Node*> Q;

    if(root)
    	Q.push(root);
    else return;
     
     struct Node * temp = root;
    while(!Q.empty()) {
     if(temp->left)
     	Q.push(temp->left);
     if (temp->right)
     	Q.push(temp->right);

    printf("%d\n", Q.front()->val);
    Q.pop();
    temp =  Q.front();
    }

 }


int max_height(struct Node * root ) {

	if(root == NULL) return 0;
	    int l_height = max_height(root->left);
		int r_height = max_height(root->right);

	return (l_height >= r_height? l_height+1:r_height+1);
}


int node_height(struct Node * root,int data,int level) {

	if(root == NULL) return 0;
	if(root->val == data) return level;

	    int downlevel = node_height(root->left,data,level+1);
	    if(downlevel!=0) return downlevel;

		downlevel = node_height(root->right,data,level+1);
		return downlevel;

}

int node_height1(struct Node * root, int data ) {

	if(root == NULL) return -1;
	if(root->val == data) return 0;
	    int l_height = node_height1(root->left,data);
		int r_height = node_height1(root->right,data);

	return (l_height !=-1 ? l_height+1:r_height!=-1? r_height+1:-1);
}


int node_hd(struct Node *root, int data, int hd) {

   if(root==NULL) return 0;
   if(root->val == data) return hd;

   int node_hd_val = node_hd(root->left,data,hd-1);
       if(node_hd_val!=0) return node_hd_val;

       node_hd_val = node_hd(root->right,data,hd+1);
       return node_hd_val;
}

struct Node * mirror(struct Node * root) {
	if(root == NULL) return NULL;

	root->left = mirror(root->left);
	root->right = mirror(root->right);

	struct Node *temp = root->left;
	       root->left = root->right;
	       root->right = temp;

	 return root;      
}
/*
void print_level_order(struct Node *) {

}
*/


struct Node * path(struct Node * root, queue<struct Node *> *q,int data) {


  if(root == NULL) return NULL;

  if(root->val == data) {
  	q->push(root);
  	return root;
  }

  if(data <= root->val) {
  	q->push(root);
  	root->left = path(root->left,q,data);
  }
  
  if (data > root->val) {
  	q->push(root);
  	root->right = path(root->right,q,data);
  } 

  if(root->left==NULL && root->right == NULL) {
  	q->pop();
  } 
 
 return root;

}

void print_stack(std::stack<int> stack) {

for (std::stack<int> dump = stack; !dump.empty(); dump.pop())
std::cout << dump.top() << '\n';

std::cout << "(" << stack.size() << " elements)\n";
}

std::stack<int> bst_stack;

struct Node * print_paths(struct Node *root) {

	if(root == NULL) return NULL;

	bst_stack.push(root->val);
	root->left = print_paths(root->left);
	root->right = print_paths(root->right);

	if(root->left == NULL && root->right == NULL) 
		print_stack(bst_stack);
	bst_stack.pop();

	return root;
}

int main() {

struct Node * root = NULL;
root = insert(root,4);
printf("root_val:%d\n",root->val );
root = insert(root,2);
printf("root_val:%d\n",root->val );
root = insert(root,1);
root = insert(root,3);
root = insert(root,6);
root = insert(root,7);
root = insert(root,5);
root = insert(root,8);
root = insert(root,100);
root = insert(root,90);
root = insert(root,80);
printf("root_val:%d\n",root->val );
//root = insert(root,8);
printf("%s\n","Inorder traversal:" );
inorder(root);

printf("%s\n","preorder traversal:" );
preorder(root);

printf("%s\n","Level order traversal:" );
level_order(root);

printf("height: %d\n", max_height(root));

mirror(root);

printf("%s\n","Mirrored Inorder traversal:" );
inorder(root);

mirror(root);

printf("%s\n","Mirrored-Mirror Inorder traversal:" );
inorder(root);

printf("%s\n","Level order traversal:" );
level_order(root);

printf("Node height:%d\n",node_height(root,5,0) );
printf("Node height1:%d\n",node_height1(root,5) );
printf("Node_hd: %d\n",node_hd(root,6,0) );

queue<struct Node *> q; 
path(root,&q,80);
printf("q.size():%d\n",q.size() );

while(!q.empty()) {
	printf("Queue pop: %d\n",(q.front())->val);
	q.pop();
}
/*
std::stack<int> stack;
    stack.push(1); 
    stack.push(3); 
    stack.push(7); 
    stack.push(19);
    stack.push(20); 

print_stack(stack);
*/
print_paths(root);

}
