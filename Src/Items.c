#pragma once
#include "cprocessing.h"
#include <stdlib.h>
#include "Items.h"
#include "Mob.h"
#include "player.h"
/*
@brief		Function that initialise array with empty items
@params		tracker	-> Contains stats for tracking Items
@returns	Item arr with blank items
*/

ItemTrack* ItemTracker;
void CreateItemTracker() {
    ItemTracker = malloc(sizeof(ItemTrack*));
    ItemTracker->arrSize = StartItemQuantity;
    ItemTracker->arr = malloc(sizeof(Item*) * StartItemQuantity);
    InitItemArr(ItemTracker->arr);
    ItemTracker->tree = NULL;
    ItemTracker->itemCount - 0;

}

void InitItemArr(ItemTrack *tracker) {

	for (int w = 0; w < tracker->arrSize;w++) {
		//Allocate memory to it
		tracker->arr[w] = malloc(sizeof(Item));
		//Fill each mob pointers with data

		Item* cI = tracker->arr[w];
		cI->Hitbox = 0;
		cI->Start = w;
		cI->Duration = 0;
		cI->Type = -1;
		cI->Modifier = 0;
		cI->x = 0;
		cI->y = 0;

	}

}


#include <stdio.h>
Item *CreateItemEffect(float x, float y) {
	//Get Random chance generator
	float RNG = CP_Random_RangeFloat(0, 1), DropChance;
	//Get Random Type of effect
	int noEffect = 2, EType = CP_Random_RangeInt(0, noEffect), cSec = (int)CP_System_GetSeconds();
    EType = 0;
    char* StatType;
    Item *newItem = malloc(sizeof(Item));
	switch (EType) {
	case 0: //All Base Stats Upgrade
        newItem->AffectedBaseStat = CP_Random_RangeInt(0, NoBaseStats );
        newItem->Duration = 5; //in secs
        newItem->Modifier = 10;
        newItem->Hitbox = 50;
		break;
	case 1:
		break;
	case 2:
		break;
	}
	newItem->Start = cSec;
	newItem->Type = EType;
	newItem->x = x;
	newItem->y = y;
    return newItem;
};




int ScaledEXP(Player *p) {
    double baserate = 1.1;
    int modrate = 10;
    //More exp give every modrate levels
    //every level increase exp required by baserate;
    int nRate = p->LEVEL.P_EXP % modrate == 0 ? p->LEVEL.P_EXP / 10 * baserate : baserate;
    
	p->LEVEL.EXP_REQ *= nRate;
    printf("Current rate %d\n", p->LEVEL.EXP_REQ);
}


void IAffectPlayer(Item* item, Player* p, int currentSec) {
	
	if (currentSec - item->Start < item->Duration) {
		switch (item->Type) {

		}
	}
}

void GenerateItem(ItemTrack* tracker, int currentSec) {

}

ItemNode* DrawItemTree(ItemNode* node) {
    if (node == NULL)
        return;
    if ((int)CP_System_GetSeconds() - node->key.Start == 10) {
        ItemTracker->tree = deleteItemNode(ItemTracker->tree, node->key);
        return;
    }
    else {
		CP_Graphics_DrawCircle(node->key.x, node->key.y, node->key.Hitbox);
    }
    DrawItemTree(node->left);
    DrawItemTree(node->right);
}


void FreeItemResource(ItemTrack *tracker) {
	for (int w = 0; w < tracker->arrSize;w++) {
		free(tracker->arr[w]);
	}
    freeTree(tracker->tree);
}


/*Tree Implementation*/
int TreeHeight(ItemNode* item) {
	if (NULL == item)
		return 0;
	//return 1 + max(TreeHeight(item->left), TreeHeight(item.right));
	return 1 + max(TreeHeight(item->left), TreeHeight(item->right));
}

int getBalance(ItemNode* item) {
	if (item == NULL)
		return 0;
	return TreeHeight(item->left) - TreeHeight(item->right);
}

ItemNode* newNode(Item item) {
	ItemNode* result = malloc(sizeof(ItemNode));
	result->key = item;
	result->left = NULL;
	result->right = NULL;
	result->h = 0;

}
ItemNode* insertItemNode(ItemNode* node, Item item) {
	if (node == NULL) {
		return newNode(item);
	}
	if (item.x < node->key.x)
		node->left = insertItemNode(node->left, item);
	else if (item.x > node->key.x)
		node->right = insertItemNode(node->right, item);
	else // Equal keys are not allowed in BST
		return NULL;
	//}
    /* 2. Update height of this ancestor node */
    node->h = TreeHeight(node);
 
    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);
 
    // If this node becomes unbalanced, then
    // there are 4 cases
 
    // Left Left Case
    //if (balance > 1 && key.x < node->left->key->x)
    if (balance > 1 && item.x < getX(node->left))
        return rightRotate(node);
 
    // Right Right Case
    if (balance < -1 && item.x > getX(node->right))
        return leftRotate(node);
 
    // Left Right Case
    if (balance > 1 && item.x > getX(node->left))
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
    // Right Left Case
	if (balance < -1 && item.x < getX(node->right))
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}
    /* return the (unchanged) node pointer */
    return node;
}

ItemNode* deleteItemNode(ItemNode* root, Item key)
{
    // STEP 1: PERFORM STANDARD BST DELETE
  
    if (root == NULL)
        return root;
  
    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if ( key.x < root->key.x )
        root->left = deleteItemNode(root->left, key);
  
    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if( key.x > root->key.x )
        root->right = deleteItemNode(root->right, key);
  
    // if key is same as root's key, then This is
    // the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            ItemNode *temp = root->left ? root->left :
                                             root->right;
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp; // Copy the contents of
                            // the non-empty child

            //memcpy(&root->key, &temp->key, sizeof(Item));
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            ItemNode* temp = minValueNode(root->right);
  
            // Copy the inorder successor's data to this node
            root->key = temp->key;
            
            //memcpy(&root->key, &temp->key, sizeof(Item));
  
            // Delete the inorder successor
            root->right = deleteItemNode(root->right, temp->key);
        }
    }
  
    // If the tree had only one node then return
    if (root == NULL)
      return root;
  
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->h = 1 + max(TreeHeight(root->left),
                           TreeHeight(root->right));
  
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);
  
    // If this node becomes unbalanced, then there are 4 cases
  
    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
  
    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }
  
    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
  
    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

ItemNode * minValueNode(ItemNode* node)
{
    ItemNode* current = node;
  
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
  
    return current;
}

ItemNode* rightRotate(ItemNode* y) {
	ItemNode* x = y->left;
	ItemNode* T2 = x->right;
	
	x->right = y;
	y->left = T2;

	y->h = TreeHeight(y);
	x->h = TreeHeight(x);

	return x;
}
ItemNode* leftRotate(ItemNode* y) {
	ItemNode* x = y->right;
	ItemNode* T2 = x->left;

	x->left = y;
	y->right = T2;

	y->h = TreeHeight(y);
	x->h = TreeHeight(x);
	return x;
}

void freeTree(ItemNode* root) {
	if (NULL == root) {
		return NULL;
	}
	freeTree(root->left);
	freeTree(root->right);
	free(root);
}

float getX(ItemNode* current) {
	return current->key.x;
}

ItemNode* FindItem(ItemNode* root, Item item) {
	if (root == NULL)
		return;
	if (item.x < root->key.x)
		FindItem(root->left, item);
	else if (item.x > root->key.x)
		FindItem(root->right, item);
	else
		return root;
}



