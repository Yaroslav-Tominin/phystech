#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define RED 1
#define BLACK 0
void* alloc(int size)
{
	if((rand()%10000)>100)
		return malloc(size);
	return NULL;
}
#define malloc alloc
pTree CreateTree()
{
	pTree tree=(pTree)malloc(sizeof(Tree));
	if (tree==NULL)
		return NULL;
	pNode nil=(pNode)malloc(sizeof(Node));
	if (nil==NULL)
	{	
		free(tree);
		return NULL;
	}
	tree->nil=nil;
	tree->root=tree->nil;
	tree->nil->color=BLACK;
	tree->nil->key=-222;
	return tree;
}
void DeleteTree(pTree* tree)
{
	pTree t=*tree;
	DeleteNodes(t,t->root);
	free(t->nil);
	free(t);
	*tree=NULL;
	
}
void DeleteNodes(pTree tree,pNode node)
{
	if(node->left!=tree->nil)
		DeleteNodes(tree, node->left);
	if(node->right!=tree->nil)
		DeleteNodes(tree, node->right);
	free(node);
}
pNode FindKey(pTree tree, int key)
{
	pNode x=tree->root;
	while(x!=tree->nil && x->key!=key)
	{
		if(key<x->key)
			x=x->left;
		else
			x=x->right;
	}
	if(x==tree->nil)
		return NULL;
	return x;
}
void LeftRotate(pTree tree,pNode x)
{
	pNode y=x->right;
	x->right=y->left;
	if (y->left!=tree->nil)
		y->left->parents=x;
	y->parents=x->parents;
	if(x->parents==tree->nil)
		tree->root=y;
	else if(x==x->parents->left)
		x->parents->left=y;
	else
		x->parents->right=y;
	y->left=x;
	x->parents=y;
}
void RightRotate(pTree tree, pNode x)
{

	pNode y=x->left;
	x->left=y->right;
	if (y->right!=tree->nil)
		y->right->parents=x;
	y->parents=x->parents;
	if(x->parents==tree->nil)
		tree->root=y;
	else if(x==x->parents->right)
		x->parents->right=y;
	else
		x->parents->left=y;
	y->right=x;
	x->parents=y;
}
void FixedInsert(pNode z,pTree tree)
{

	pNode y;
	while(z->parents->color==RED)
	{
		if((z->parents)==(z->parents->parents->left))
		{
			y=z->parents->parents->right;
			if(y->color==RED)
			{
				z->parents->color=BLACK;
				y->color=BLACK;
				z->parents->parents->color=RED;
				z=z->parents->parents;
			}
			else 
			{
				//2 случай превращаем в 3
				if(z==z->parents->right)
				{
					z=z->parents;
					LeftRotate(tree,z);
				}
				//3случай
				z->parents->color=BLACK;
				z->parents->parents->color=RED;
				RightRotate(tree,z->parents->parents);
			}
		}
		else
		{
			y=z->parents->parents->left;
			if(y->color==RED)
			{
				z->parents->color=BLACK;
				y->color=BLACK;
				z->parents->parents->color=RED;
				z=z->parents->parents;
			}
			else 
			{
				if(z==z->parents->left)
				{
					z=z->parents;
					RightRotate(tree,z);
				}
				z->parents->color=BLACK;
				z->parents->parents->color=RED;
				LeftRotate(tree,z->parents->parents);
			}
		}
	}
	tree->root->color=BLACK;
}
int Insert(pTree tree,int key)
{
	if(tree==NULL)
		return -1;
	pNode z=(pNode)malloc(sizeof(Node));
	if(z==NULL)
		return -1;
	z->key=key;
	pNode y=tree->nil;
	pNode x=tree->root;
	while(x!=tree->nil)
	{
		y=x;
		if(z->key<x->key)
			x=x->left;
		else
			x=x->right;
	}
	z->parents=y;
	if(y==tree->nil)
		tree->root=z;
	else if(z->key<y->key)
		y->left=z;
	else
		y->right=z;
	z->left=tree->nil;
	z->right=tree->nil;
	z->color=RED;
	FixedInsert(z,tree);
	return 1;
}
pNode TreeMinimum(pTree tree,pNode x)
{
	while(x->left!=tree->nil)
	{
		x=x->left;
	}
	return x;
}
void Transplant(pTree tree,pNode u, pNode v)
{
	if(u->parents==tree->nil)
	{
		tree->root=v;
	}
	else if (u==u->parents->left)
	{
		u->parents->left=v;
	}
	else
		u->parents->right=v;
	v->parents=u->parents;
}
void DeleteFixup(pTree tree,pNode x)
{
	pNode w;
	while(x!=tree->root && x->color==BLACK)
	{
		if(x==x->parents->left)
		{
			w=x->parents->right;
			//когда брат у икса красный
			if(w->color==RED)
			{
				w->color=BLACK;
				x->parents->color=RED;
				LeftRotate(tree,x->parents);
				w=x->parents->right;
			}
			
			//брат черный и все дети w тоже
			if(w->left->color==BLACK && w->right->color==BLACK)
			{
				w->color=RED;
				x=x->parents;
			}
			else 
			{
				//брат черный,и левый ребенок w крассный, а другой черный
				if(w->right->color==BLACK)
				{
					w->left->color=BLACK;
					w->color=RED;
					RightRotate(tree,w);
					w=x->parents->right;
				}
				//брат черный, а правый дочерний w крассный
				w->color=x->parents->color;
				x->parents->color=BLACK;
				w->right->color=BLACK;
				LeftRotate(tree,x->parents);
				x=tree->root;
			}
		}
		else
		{
			w=x->parents->left;
			if(w->color==RED)
			{
				w->color=BLACK;
				x->parents->color=RED;
				RightRotate(tree,x->parents);
				w=x->parents->left;
			}
			if(w->right->color==BLACK && w->left->color==BLACK)
			{
				w->color=RED;
				x=x->parents;
			}
			else 
			{
				if(w->left->color==BLACK)
				{
					w->right->color=BLACK;
					w->color=RED;
					LeftRotate(tree,w);
					w=x->parents->left;
				}
				w->color=x->parents->color;
				x->parents->color=BLACK;
				w->left->color=BLACK;
				RightRotate(tree,x->parents);
				x=tree->root;
			}
		}
	}
	x->color=BLACK;
}
void Delete(pTree tree,pNode z)
{
	if(tree==NULL)
		return ;
	pNode x;
	pNode y=z;
	int y_orig=y->color;
	if(z->left==tree->nil)
	{
		x=z->right;
		Transplant(tree,z,z->right);
	}
	else if(z->right==tree->nil)
	{
		x=z->left;
		Transplant(tree,z,z->left);
	}
	else
	{
		 y=TreeMinimum(tree,z->right);
		 y_orig=y->color;
		 x=y->right;
		if(y->parents==z)
			x->parents=y;
		else
		{
			Transplant(tree,y,y->right);
			y->right=z->right;
			y->right->parents=y;
		}
		Transplant(tree,z,y);
		y->left=z->left;
		y->left->parents=y;
		y->color=z->color;
	}
	//printf("IN DELETE KEY BEFOR DELETEFIXUP, color=%d,z nil?-%d,parent nil?-%d,left nil?-%d,right nil?-%d\n\n\n",z->color,z==tree->nil,z->parents==tree->nil,z->left==tree->nil,z->right==tree->nil);
	
	if(y_orig==BLACK)
		DeleteFixup(tree,x);
	free(z);
}
int DeleteKey(pTree tree,int key)
{
	if(tree==NULL)
		return -1;
	pNode z=FindKey(tree,key);
	if(z==NULL)
		return -1;
	//printf("IN DELETEKEY BEFOR DELETE, color=%d,z nil?-%d,parent nil?-%d,left nil?-%d,right nil?-%d\n\n\n",z->color,z==tree->nil,z->parents==tree->nil,z->left==tree->nil,z->right==tree->nil);
	Delete(tree,z);
	return 1;
}
pIterator CreateIterator(pTree tree)
{
	if(tree->root==tree->nil)
		return NULL;
	pIterator iter=(pIterator)malloc(sizeof(Iterator));
	if(iter==NULL)
		return NULL;
	pNode x=tree->root;
	while(x->left!=tree->nil)
		x=x->left;
	iter->element=x;
	iter->nil=tree->nil;
	return iter;
}
void NextIterator(pIterator* Iter)
{
	pIterator iter=*Iter;
	GetIteratorValue(iter);
	if(iter->element->right==iter->nil)
	{
		//идем к родителю пока находимся в правом ребенке
		//закончим если дойдем до родителя из правого ребенка, тогда выход
		//или дойдем до левого ребенка
		while(iter->element->parents!=iter->nil && iter->element->parents->right==iter->element)
			iter->element=iter->element->parents;
		if(iter->element->parents==iter->nil)
		{
			DeleteIterator(Iter);
			return;
		}
		iter->element=iter->element->parents;
		*Iter=iter;
		return;
	}
	else
	{
		iter->element=iter->element->right;
		while(iter->element->left!=iter->nil)
			iter->element=iter->element->left;
		*Iter=iter;
		return;
	}		
}
int GetIteratorValue(pIterator iter)
{
	return iter->element->key;
}
void DeleteIterator(pIterator* Iter)
{
	free(*Iter);
	*Iter=NULL;
}
void GetAllTreeValue(pTree tree)
{
	if(tree==NULL)
		return;
	pIterator iter=CreateIterator(tree);
	if(iter==NULL)
		return;
	while(iter)
	{
		//printf("%d,color=%d,root=%d\n  ",GetIteratorValue(iter),iter->element->color,tree->root->key);
		NextIterator(&iter);
	}
}
