#include <stdio.h>
#include <stdlib.h>
#include <queue.h>
#include <stack.h>

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
		return temp;
	} else if (num <= root->val) 
	  root->left = insert(root->left,num);
	  else root->right = insert(root->right,num);   
   return root;
}

void inorder(struct Node * root) {
	if(root->left) inorder(root->left);
	printf("%d\n",root->val);
	if(root->right) inorder(root->right);
}

void inorder_nr(struct Node *root) {
     Queue<struct Node *> Q;
     
    while(!Q.empty()) {
     if(root)
     	Q.push(root);
     if(root->left)
     	Q.push(root->left);
     if (root->right)
     	Q.push(root->right);

     printf("%d\n", Q.front());
     Q.pop();
    }

}


int main() {

struct Node * root = NULL;
root = insert(root,4);
root = insert(root,2);
root = insert(root,1);
root = insert(root,3);
root = insert(root,6);
root = insert(root,7);
root = insert(root,5);
inorder(root);

void inorder_nr(root);


}
