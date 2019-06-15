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


 void printLevelOrder(Node *root)  
{  
    // Base Case  
    if (root == NULL) return;  
  
    // Create an empty queue for level order tarversal  
    queue<Node *> q;
    vector <vector<int> > buff;   
  
    // Enqueue Root and initialize height  
    q.push(root);  
  
    while (q.empty() == false)  
    {  
        // nodeCount (queue size) indicates number 
        // of nodes at current lelvel.  
        int node_cnt = q.size();  
        vector<int> vect;
  
        // Dequeue all nodes of current level and  
        // Enqueue all nodes of next level  
        while (node_cnt > 0) 
        {    
            Node *node = q.front();  
            vect.push_back(node->val);

           // cout << node->val << " ";  
            q.pop();  
            if (node->left != NULL)  
                q.push(node->left);  
            if (node->right != NULL)  
                q.push(node->right);  
            node_cnt--;  
        }  
        buff.push_back(vect);
        cout << endl;  
    }  

for(int i = 0; i< buff.size(); i++) {
	for(int j=0; j<buff[i].size(); j++) {
		printf("level:%d  ",buff[i][j]);
	}
	printf("\n");
  }

}  

 void printLevelOrder_msft(Node *root)  
{  
    // Base Case  
    if (root == NULL) return;  
  
    // Create an empty queue for level order tarversal  
    queue<Node *> q;

     printf("level order msft\n");

  
    // Enqueue Root and initialize height  
    q.push(root);  
  
    while (q.empty() == false)  
    {  
        // nodeCount (queue size) indicates number 
        // of nodes at current lelvel.  
        int node_cnt = q.size();  
  
        // Dequeue all nodes of current level and  
        // Enqueue all nodes of next level  
        while (node_cnt > 0) 
        {    
            Node *node = q.front();  

           // cout << node->val << " ";  
            printf("%d ", node->val);
            q.pop();  

            if (node->left != NULL)  
                q.push(node->left);  
            if (node->right != NULL)  
                q.push(node->right);  
            node_cnt--;  
        }  
        cout << endl;  
    }  

}  

void zigzag(struct Node *root) {
	 
	if(!root) return; 

	stack<Node *>s1,s2;
	s1.push(root);
  
    while(!s1.empty() || !s2.empty()) {
      struct Node * p;
    	while(!s1.empty()) {
    		p = s1.top();
    		s1.pop();
    		
    		printf("zigzagL2R:%d\n",p->val);

    		if(p->left) s2.push(p->left);
    		if(p->right) s2.push(p->right);
    	}

	while(!s2.empty()) {
    		p = s2.top();
    	    s2.pop();

    		printf("zigzagR2L:%d\n",p->val);

    		if(p->right) s1.push(p->right);
    		if(p->left) s1.push(p->left);
	    	}    	
	    }
}






void print_queue(queue<Node *> q)
{
  while (!q.empty())
  {
    cout << q.front()->val << " ";
    q.pop();
  }
  std::cout << std::endl;
}
//side : 0 left , side : 1 right
void side_view(struct Node * root, int side)  {

	if(root == NULL) return;
	queue<Node *> Q;
	Q.push(root); int cnt = Q.size(); 
   Node * temp = NULL; int left_idx = cnt;

	while(!Q.empty()) {

		  while(cnt){
		  	temp = 	Q.front(); Q.pop();
		  	
		  	if(side == 0 &&cnt == left_idx) {
		  		printf("left side %d :",temp->val );
		  	}

		  	printf("%d ",temp->val );
			if(temp->left!= NULL) Q.push(temp->left);
			if(temp->right!=NULL) Q.push(temp->right);

		  	if(side == 1 &&cnt == 1) {
		  		printf("right side %d :",temp->val );
		  	}

			cnt--;
		} printf("\n",Q.size());
		// print_queue(Q);

		cnt = Q.size();
		left_idx = cnt;
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

int node_hd1(struct Node *root, int data, int hd) {

   if(root==NULL) return 0;
   if(root->val == data) return hd;

   node_hd(root->left,data,hd-1);
   node_hd(root->right,data,hd+1);

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


bool path(struct Node * root, queue<struct Node *> *q,int data) {


  if(root == NULL) 
  	return false;
  
  q->push(root);
  
  if(root->val == data)
  	return true;

  if(path(root->left,q,data) || path(root->right,q,data)) 
  	 return true;
  
  	q->pop();
 
 return false;

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


struct Node* minValue(struct Node *root) {
    if(root== NULL) return 0;
	while(root->left) {
		root = root->left;
	}
	return root;
}

struct Node * inOrderSuccessor(struct Node *root, struct Node *n) 
{ 
    // step 1 of the above algorithm 
    if( n->right != NULL ) 
        return minValue(n->right); 
  
    struct Node *succ = NULL; 
  
    // Start from root and search for successor down the tree 
    while (root != NULL) 
    { 
        if (n->val < root->val) 
        { 
            succ = root; 
            root = root->left; 
        } 
        else if (n->val > root->val) 
            root = root->right; 
        else
           break; 
    } 
  
    return succ; 
} 


int CountNodes(Node *tree)
{
    int c =  1;             //Node itself should be counted
    if (tree ==NULL)
        return 0;
    else
    {
        c += CountNodes(tree->left);
        c += CountNodes(tree->right);
        return c;
    }
}



int idx = 0;
void serialize_preorder(struct Node *root, int * data, int node_cnt) {
    
    if(!root) return;
    if(idx > node_cnt) return; 
    
    data[idx] = root->val; 

    idx++;
    serialize_preorder(root->left,data,node_cnt);
    serialize_preorder(root->right,data,node_cnt);   
}

int* serialize(struct Node* root) {
    
    if(!root) return NULL;

    int node_cnt = CountNodes(root);

    printf("node_cnt : %d\n",node_cnt );
    
    int *data = (int *) malloc(sizeof(int)*(node_cnt));
    serialize_preorder(root,data,node_cnt);

    return data;  
}


int finddivision(int * preorder, int val, int low,int high) {
	int i;
	for(i =low;i<=high;i++ ) {
		if(val < preorder[i])
           break;
	}
	return i;
}

struct Node * deserialize(int *preorder,int low, int high ){

	if(low > high) return NULL;
    
    struct Node *root = (struct Node*) malloc(sizeof(struct Node));

	root->val = preorder[low];

	int dividx = finddivision(preorder,root->val,low+1,high);

	root->left = deserialize(preorder,low+1,dividx-1);
	root->right = deserialize(preorder,dividx,high);

	return root;
}


int cur_idx = 0;
struct Node* deserialize_new(int *preorder, int num_nodes, int low, int high) {

	if(cur_idx > num_nodes) return NULL;

    struct Node *root = NULL;

    if(preorder[cur_idx] > low && preorder[cur_idx] < high) {

    	 root = (struct Node *)malloc(sizeof(struct Node));
    	 root->val = preorder[cur_idx];
         cur_idx++;
         root->left = deserialize_new(preorder,num_nodes,low,root->val);
         root->right = deserialize_new(preorder,num_nodes,root->val,high);	 
    } 
    
    return root;
}


void serialize_bt(struct Node * root, vector<int> &vect) {

	if(root == NULL) {
		vect.push_back(-1);
		return;
	}

	vect.push_back(root->val);
	serialize_bt(root->left,vect);
	serialize_bt(root->right,vect);
}

int bt_idx = 0;

struct Node * deserialize_bt(std::vector<int> &vect) {

	if(bt_idx == vect.size() || vect[bt_idx] == -1)
		{
			bt_idx+=1;
			return NULL;
		}

	struct Node * root = (struct Node*) malloc (sizeof(struct Node));
	root->val = vect[bt_idx];
	bt_idx+=1;

	root->left = deserialize_bt(vect);
	root->right = deserialize_bt(vect);

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

printf("%s\n","left side view :" );
side_view(root,0);

printf("%s\n","right side view :" );
side_view(root,1);

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
printf("Node_hd: %d\n",node_hd(root,90,0) );
printf("Node_hd1: %d\n",node_hd1(root,90,0) );

queue<struct Node *> q; 
printf("Inorder:\n");
inorder(root);
printf("Path:\n");
int path_exist = path(root,&q,800);
printf("path_exist:%d q.size():%d\n",path_exist,q.size() );

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
struct Node * inOrderSuccessor_ = inOrderSuccessor(root,root->right->right);
printf("inOrderSuccessor: %d\n",inOrderSuccessor_->val);

//n=1;
//printf("Count nodes :%d\n",CountNodes(root));

int *bst_data = serialize(root);

for(int i=0; i < 11; i++) {
	printf("bst_preoder_serial_data: %d\n", bst_data[i]);
}

//struct Node * ds_root = deserialize(bst_data,0,CountNodes(root)-1);

struct Node * ds_root = deserialize_new(bst_data,CountNodes(root),INT_MIN,INT_MAX);

inorder(ds_root);
/*
vector<int> vect;

serialize_bt(root,vect);
for(int i=0; i < vect.size(); i++) {
	printf("BT_preoder_serial_data: %d\n", vect[i]);
}

struct Node *ds_bt = deserialize_bt(vect);
inorder(ds_bt);

//free(bst_data);
//bst_data = NULL;
*/

printLevelOrder_msft(root);

zigzag(root);

}
