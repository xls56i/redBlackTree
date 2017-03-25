#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef enum colorType{
	red = 1,
	black = 0
}nodeColor;

typedef int nodeKey;

typedef struct treeNode
{
	nodeColor color;
	nodeKey key;
	struct treeNode *left;
	struct treeNode *right;
	struct treeNode *parent;
}node;

typedef struct Tree{
	node *nil;
	node *root;
}tree;

tree* creatTree(){
	tree* T = (tree*)malloc(sizeof(node));
	node* nil = (node*)malloc(sizeof(node));
	nil->color = black;
	node* root = nil;
	T->nil = nil;
	T->root = root;
	return T;
}

void left_rotate(tree* T, node* x){
	node* y = (node*)malloc(sizeof(node));
	y = x->right;
	x->right = y->left;
	if(y->left != T->nil)
		y->left->parent = x;
	y->parent = x->parent;
	if(x->parent == T->nil)
		T->root = y;
	else if(x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}

void right_rotate(tree* T, node* y){
	node* x = (node*)malloc(sizeof(node));
	x = y->left;
	y->left = x->right;
	if(x->right != T->nil)
		x->right->parent = y;
	x->parent = y->parent;
	if(y->parent == T->nil)
		T->root = x;
	else if(y == y->parent->right)
		y->parent->right = x;
	else
		y->parent->left = x;
	x->right = y;
	y->parent = x;	
}

void rb_insert_fixup(tree* T, node* z){
	node* y = (node*)malloc(sizeof(node));
	while(z->parent->color == red){
		if(z->parent == z->parent->parent->left){
			y = z->parent->parent->right;
			if(y->color == red){
				z->parent->color = black;
				y->color = black;
				z->parent->parent->color = red;
				z = z->parent->parent;
			} else if(z == z->parent->right){
				z = z->parent;
				left_rotate(T,z);
			} else{
				z->parent->color = black;
				z->parent->parent->color = red;
				right_rotate(T,z->parent->parent);	
			}
		} else{
			y = z->parent->parent->left;
			if(y->color == red){
				z->parent->color = black;
				y->color = black;
				z->parent->parent->color = red;
				z = z->parent->parent;
			} else if(z == z->parent->left){
				z = z->parent;
				right_rotate(T,z);
			} else{
				z->parent->color = black;
				z->parent->parent->color = red;
				left_rotate(T,z->parent->parent);	
			}
		}
	}
	T->root->color = black;
}

void rb_insert(tree* T, nodeKey key){
	node* z = (node*)malloc(sizeof(node));
	z->key = key;
	node* y = T->nil;
	node* x = T->root;
	while(x != T->nil){
		y = x;
		if(z->key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	z->parent = y;
	if (y == T->nil)
		T->root = z;
	else if(z->key < y->key)
		y->left = z;
	else
		y->right = z;
	z->left = T->nil;
	z->right = T->nil;
	z->color = red;
	rb_insert_fixup(T,z);
}

void rb_transplant(tree* T, node* u, node* v){
	if(u->parent == T->nil)
		T->root = v;
	else if(u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	v->parent = u->parent;
}

void rb_delete_fixup(tree* T, node* x){
	node* w = (node*)malloc(sizeof(node));
	while(x != T->root && x->color == black){
		if(x == x->parent->left){
			w = x->parent->right;
			if(w->color == red){
				w->color = black;
				x->parent->color = red;
				left_rotate(T, x->parent);
				w = x->parent->right;
			}
			if(w->left->color == black && w->right->color == black){
				w->color = red;
				x = x->parent;
			}
			else if(w->right->color == black){
				w->left->color = black;
				w->color = red;
				right_rotate(T,w);
				w = x->parent->right;
			}
			else{
				w->color = x->parent->color;
				x->parent->color = black;
				w->right->color = black;
				left_rotate(T,x->parent);
				x = T->root;
			}
		}
		else{
			w = x->parent->left;
			if(w->color == red){
				w->color = black;
				x->parent->color = red;
				right_rotate(T, x->parent);
				w = x->parent->left;
			}
			if(w->right->color == black && w->left->color == black){
				w->color = red;
				x = x->parent;
			}
			else if(w->left->color == black){
				w->right->color = black;
				w->color = red;
				left_rotate(T,w);
				w = x->parent->left;
			}
			else{
				w->color = x->parent->color;
				x->parent->color = black;
				w->left->color = black;
				right_rotate(T,x->parent);
				x = T->root;
			}
		}	
	}
}

node* tree_minimum(tree* T, node* x){
	while(x->left != T->nil)
		x = x->left;
	return x;
}

node* tree_maxmum(tree* T, node* x){
	while(x->right != T->nil)
		x = x->right;
	return x;
}

node* tree_successor(tree* T, node* x){
	node* y = (node*)malloc(sizeof(node));
	if(x->right != T->nil)
		return tree_minimum(T, x->right);
	y = x->parent;
	while(y != T->nil && x == y->right){
		x = y;
		y = y->parent;
	}
	return y;
}

node* tree_predecessor(tree* T, node* x){
	node* y = (node*)malloc(sizeof(node));
	if(x->left != T->nil)
		return tree_maxmum(T, x->left);
	y = x->parent;
	while(y != T->nil && x == y->left){
		x = y;
		y = y->parent;
	}
	return y;
}


void display(int k, node* x){
	int i = 0;
	for(; i < k-1; i++)
		printf("      ");
	if(k != 0)
		printf("|---");
	printf("%d", x->key);
	if(x->color == 1)
		printf("(r)\n");
	else
		printf("(b)\n");
}

void show(tree* T, node* x, int i){
	if(x != T->nil){
		i++;
		show(T,x->right,i);
		i--;
		display(i,x);
		i++;
		show(T,x->left,i);
		i--;
	}
}

void inorder_treeWalk(tree* T, node* x){
	if(x != T->nil){
		inorder_treeWalk(T,x->left);
		printf("%d ",x->key);
		inorder_treeWalk(T,x->right);
	}
}

node* tree_search(tree* T, node* x, int k){
	if(x == T->nil || k == x->key)
		return x;
	if(k < x->key)
		return tree_search(T, x->left, k);
	else
		return tree_search(T, x->right, k);
}

void rb_delete(tree* T, node* z){
	node* y = (node*)malloc(sizeof(node));
	node* x = (node*)malloc(sizeof(node));
	y = z;
	nodeColor y_original_color = y->color;
	if(z->left == T->nil){
		x = z->right;
		rb_transplant(T,z,z->right);
	} else if(z->right == T->nil){
		x == z->left;
		rb_transplant(T,z,z->left);
	} else{
		y = tree_minimum(T,z->right);
		y_original_color = y->color;
		x = y->right;
		if(y->parent == z)
			x->parent = y;
		else{
			rb_transplant(T,y,y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		rb_transplant(T,z,y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	if(y_original_color == black)
		rb_delete_fixup(T,x);
}

int main()
{
//	tree* T = creatTree();
//	rb_insert(T,2);
//	rb_insert(T,6);
//	rb_insert(T,9);
//	rb_insert(T,5);
//	rb_insert(T,1);
//	rb_insert(T,10);
//	rb_insert(T,32);
//	rb_insert(T,14);
//	rb_insert(T,7);
//	rb_insert(T,8);
//	rb_insert(T,0);
//	inorder_treeWalk(T,T->root);
//	printf("root:%d\n", T->root->color);
//	node* mi = tree_minimum(T,T->root);
//	node* ma = tree_maxmum(T,T->root);
//	printf("%d\t%d\n", mi->key,ma->key);
//	node* h1 = tree_successor(T,mi);
//	node* h2 = tree_predecessor(T,ma);
//	printf("%d\t%d\n", h1->key,h2->key);
//	node* h3 = tree_search(T,T->root,9);
//	printf("%d\n", h3->key);
//	
//	rb_delete(T, mi);
//	inorder_treeWalk(T,T->root);
//	
//	printf("root:%d\n", T->root->color);
//	mi = tree_minimum(T,T->root);
//	ma = tree_maxmum(T,T->root);
//	printf("%d\t%d\n", mi->key,ma->key);
//	h1 = tree_successor(T,mi);
//	h2 = tree_predecessor(T,ma);
//	printf("%d\t%d\n", h1->key,h2->key);
//	h3 = tree_search(T,T->root,9);
//	printf("%d\n", h3->key);
//	show(T,T->root,0);
	return 0;
}
