#include "person.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define MAX_CAPACITY 15

typedef struct BSTNode BSTNode;
typedef struct BSTPersonStore BSTPersonStore;

struct BSTNode {
    int value;
    BSTNode *left;
    BSTNode *right;
};

struct BSTPersonStore{
  BSTNode *root;
  int num_nodes;
  int max_capacity;
};

bool insert(BSTNode **root,int val);
void inOrderPrint(BSTNode *root);
void freeBSTNodes(BSTNode *root);

BSTPersonStore *personStoreCreateBST(void);
bool BSTPersonStoreInsertPerson(int val,BSTPersonStore *store);
void BSTPersonStorePrintAllPeople(BSTPersonStore *store);
bool BSTPersonStoreDeleteAllPeople(BSTPersonStore *store);

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
