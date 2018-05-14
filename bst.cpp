#include <queue>
#include <stack>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

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


struct Node * mirror(struct Node * root) {
	if(root == NULL) return NULL;

	root->left = mirror(root->left);
	root->right = mirror(root->right);

	struct Node *temp = root->left;
	       root->left = root->right;
	       root->right = temp;

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

}
