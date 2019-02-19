#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
	int j,i,q;
	pNode z;
	pTree tree;
	srand(time(NULL));
	
	
	//1 case of deletefixup
	tree=CreateTree();
	CreateIterator(tree);
	Insert(tree,10);
	Insert(tree,6);
	Insert(tree,15);
	GetAllTreeValue(tree);
	Insert(tree,18);
	GetAllTreeValue(tree);
	Insert(tree,12);
	Insert(tree,19);
	GetAllTreeValue(tree);
	DeleteKey(tree,6);
	DeleteTree(&tree);
	
	
	
	
	
	tree=CreateTree();
	Insert(tree,10);
	Insert(tree,15);
	Insert(tree,6);
	GetAllTreeValue(tree);
	Insert(tree,4);
	GetAllTreeValue(tree);
	Insert(tree,7);
	Insert(tree,5);
	GetAllTreeValue(tree);
	DeleteKey(tree,15);
	DeleteTree(&tree);
	
	
	
	
	
	
	
	tree=CreateTree();
	if(tree==NULL)
	{
		return 1;
	}
	Insert(tree,7);
	Insert(tree,3);
	Insert(tree,18);
	Insert(tree,10);
	Insert(tree,22);
	Insert(tree,8);
	Insert(tree,11);
	Insert(tree,26);
	Insert(tree,2);
	Insert(tree,6);
	Insert(tree,13);
	GetAllTreeValue(tree);
	GetAllTreeValue(tree);
	DeleteKey(tree,18);
	DeleteKey(tree,11);
	DeleteKey(tree,3);
	DeleteKey(tree,10);
	DeleteKey(tree,22);
	DeleteTree(&tree);
	
	
	
	
	
	
	
	
	for(j=0;j<1000;++j)
	{
		tree=CreateTree();
		if(tree==NULL)
		{
			printf("GOOD BY\n\n\n\n\n");
			continue;
		}
		Insert(tree,5);
		Insert(tree,5);
		srand(time(NULL));
		for(i=0;i<1000;++i)
			Insert(tree,(rand() %10000));
		for(i=0;i<1000;++i)
		{
			q=(rand() %10000);
			z=FindKey(tree,q);
			if(z!=NULL && (z->right)!=tree->nil)
				Delete(tree,z->right);
		}
		for(i=0;i<1000;++i)
		{
			q=(rand() %10000);
			z=FindKey(tree,q);
			DeleteKey(tree,q);
		}
		GetAllTreeValue(tree);
		DeleteTree(&tree);
	}
	return 0;
}
