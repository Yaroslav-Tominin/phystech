typedef struct Node
{
	struct Node* parents;
	struct Node* left;
	struct Node* right;
	int color;
	int key;
}Node,*pNode;
typedef struct Tree
{
	pNode root;
	pNode nil;
}Tree,*pTree;
typedef struct Iterator
{
	pNode element;
	pNode nil;
}Iterator,*pIterator;

pTree CreateTree();      //+
void DeleteTree(pTree* tree);
void DeleteNodes(pTree tree,pNode node);
pNode FindKey(pTree tree, int key);    //+-
void LeftRotate(pTree tree,pNode x);   //xz
void RightRotate(pTree tree,pNode x);   //xz
void FixedInsert(pNode node,pTree tree); //xz
int Insert(pTree tree,int key);       //+-
pNode TreeMinimum(pTree tree,pNode x);  //xz
void Transplant(pTree tree,pNode u, pNode v);  //xz
void DeleteFixup(pTree tree,pNode x);     //xz
void Delete(pTree tree,pNode z);   //xz
int DeleteKey(pTree tree,int key);

pIterator CreateIterator(pTree tree);   //+
void NextIterator(pIterator* Iter);  //+
int GetIteratorValue(pIterator iter); //+
void DeleteIterator(pIterator* Iter);  //+
void GetAllTreeValue(pTree tree);   //+

