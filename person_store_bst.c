#include "person.h"
#include "person_store_bst.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "person_store_bst.h"
#define MAX_CAPACITY 15

bool insert(BSTNode **root,int val);
void inOrderPrint(BSTNode *root);
void freeBSTNodes(BSTNode *root);
bool delete(BSTNode **root, int val);
BSTNode *findMin(BSTNode *root);
BSTNode *findBST(BSTNode *root, int val);

BSTNode *findMin(BSTNode *root){
    if(root == NULL){
        return NULL;
    }
    while(root->left != NULL){
        root = root->left;
    }
    return root;
}

void freeBSTNodes(BSTNode *root){
    if(root == NULL){
        return;
    }
    freeBSTNodes(root->left);
    freeBSTNodes(root->right);
    free(root);
}

void inOrderPrint(BSTNode *root){
    if(root == NULL){
        return;
    }
    inOrderPrint(root->left);
    printf("========================================\n");
    printf(" Node Address : %p\n", (void *)root);
    printf(" Node Value   : %d\n", root->value);
    printf(" Left Child   : %p\n", (void *)root->left);
    printf(" Right Child  : %p\n", (void *)root->right);
    printf("========================================\n\n");
    inOrderPrint(root->right);
}

bool insert(BSTNode **root,int val){
    if(*root == NULL){
        BSTNode *node = malloc(sizeof(*node));
        node->value = val;
        node->left = NULL;
        node->right = NULL;
        *root = node;
        return true;
    }
    // If root val is == val (duplicate)
    if((*root)->value == val){
        return false;
    } // If val is less than root val
    else if(val < (*root)->value){
        return insert(&(*root)->left,val);
    } // If val is greater than root val
    else {
        return insert(&(*root)->right,val);
    }
    return false;
}

bool delete(BSTNode **root, int val){
    if(*root == NULL){
        return false;
    }
    if(val < (*root)->value){
        return delete(&(*root)->left,val);
    }
    if (val > (*root)->value){
        return delete(&(*root)->right,val);
    }
    // Found the node
    BSTNode *temp = *root;
    // 1: No children
    if(temp->left == NULL && temp->right == NULL){
        free(temp);
        *root = NULL;
        return true;
    }
    // 2: Only one child left/rght
    if(temp->left == NULL){
        *root = temp->right;
        free(temp);
        return true;
    }
    if(temp->right == NULL){
        *root = temp->left;
        free(temp);
        return true;
    }
    // 3: Both children
    BSTNode *succesor = findMin(temp->right);
    temp->value = succesor->value;
    return delete(&temp->right, succesor->value);
}

BSTNode *findBST(BSTNode *root, int val){
    if(root == NULL){
        return NULL;
    }
    if(val == root->value){
        return root;
    }
    if(val < root->value){
        return findBST(root->left,val);
    }
    return findBST(root->right,val);
}

BSTPersonStore *personStoreCreateBST(void){
    BSTPersonStore *store = malloc(sizeof(*store));
    if(store == NULL){
        return NULL;
    }
    store->max_capacity = MAX_CAPACITY;
    store->num_nodes = 0;
    store->root = NULL;
    return store;
}

bool BSTPersonStoreInsertPerson(int val,BSTPersonStore *store){
    if(store->max_capacity == store->num_nodes){
        return false;
    }
    bool return_value = insert(&store->root,val);
    store->num_nodes += (return_value) ? 1 : 0;
    return return_value;
}

void BSTPersonStorePrintAllPeople(BSTPersonStore *store){
    if(store == NULL){
        return;
    }
    if(store->root == NULL){
        printf("No people in store.\n");
    }
    inOrderPrint(store->root);
}

bool BSTPersonStoreDeleteAllPeople(BSTPersonStore *store){
    BSTNode *root = store->root;
    freeBSTNodes(root);
    store->root = NULL;
    store->num_nodes = 0;
    return true;
}

bool BSTPersonStoreDeletePerson(int val,BSTPersonStore *store){
    bool return_value = delete(&store->root,val);
    store->num_nodes -= (return_value) ? 1 : 0;
    return return_value;
}

BSTNode * BSTPersonStoreFindPerson(int id, BSTPersonStore *store){
    BSTNode *n = findBST(store->root,id);
    return n;
}

void BSTPersonStoreDestroy(BSTPersonStore *store){
    if(store == NULL){
        return;
    }
    BSTPersonStoreDeleteAllPeople(store);
    free(store);
    return;
}
