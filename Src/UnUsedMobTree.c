// A utility function to get the height of the tree

#include "Mob.h"
#include "player.h"
#include "UnUsedMobTree.h"
#include <stdlib.h>

int treeheight(MobNode *N)
{
    if (N == NULL)
        return 0;
	return 1+max(treeheight(N->left), treeheight(N->right));
}

// A utility function to get maximum of two integers
//int max(int a, int b);
 MobNode* newNode(Mob mob)
{
    MobNode* node = malloc(sizeof(MobNode));
	node->key = mob;
    node->left   = NULL;
    node->right  = NULL;
    node->h = 0;  // new node is initially added at leaf
    return(node);
}

/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
 
// A utility function to right rotate subtree rooted with y
// See the diagram given above.
MobNode *rightRotate(MobNode *y)
{
    MobNode *x = y->left;
    MobNode *T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->h = treeheight(y);
    x->h = treeheight(x);
 
    // Return new root
    return x;
}
 
// A utility function to left rotate subtree rooted with x
// See the diagram given above.
MobNode *leftRotate(MobNode *x)
{
    MobNode *y = x->right;
    MobNode *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->h = treeheight(x);
    y->h = treeheight(y);
 
    // Return new root
    return y;
}
 
// Get Balance factor of node N
int getBalance(MobNode *N)
{
    if (N == NULL)
        return 0;
    return treeheight(N->left) - treeheight(N->right);
}
float getX(MobNode* N) {
    return N->key.x;
}
// Recursive function to insert a key in the subtree rooted
// with node and returns the new root of the subtree.
MobNode* insert(MobNode* node, Mob key)
{
	/* 1.  Perform the normal BST insertion */
	if (node == NULL)
		return(newNode(key));

	if (key.x < node->key.x)
		node->left = insert(node->left, key);
	else if (key.x > node->key.x)
		node->right = insert(node->right, key);
	else // Equal keys are not allowed in BST
		return NULL;
	//}
    /* 2. Update height of this ancestor node */
    node->h = treeheight(node);
 
    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);
 
    // If this node becomes unbalanced, then
    // there are 4 cases
 
    // Left Left Case
    //if (balance > 1 && key.x < node->left->key->x)
    if (balance > 1 && key.x < getX(node->left))
        return rightRotate(node);
 
    // Right Right Case
    if (balance < -1 && key.x > getX(node->right))
        return leftRotate(node);
 
    // Left Right Case
    if (balance > 1 && key.x > getX(node->left))
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
    // Right Left Case
	if (balance < -1 && key.x < getX(node->right))
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}
    /* return the (unchanged) node pointer */
    return node;
}


void freeTree(MobNode* node) {
	if (NULL == node) {
		return;
	}
	freeTree(node->left);
	freeTree(node->right);
	free(node);
}

void traverseTree(CP_Image *Sprites, MobNode *N, Player*p, WaveTrack *tracker) {
	if (NULL == N) {
			return;
	}
	traverseTree(Sprites, N->left, p, tracker);
	traverseTree(Sprites, N->right,p, tracker);
}
