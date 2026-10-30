#include "person.h"
#include "person_store.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "person_store_internal.h"
#define MAX_CAPACITY 15


typedef struct BSTNode BSTNode;
typedef struct BSTPersonStore BSTPersonStore;

struct BSTNode {
    Person *value;
    BSTNode *left;
    BSTNode *right;
};

struct BSTPersonStore{
  PersonStore base;
  BSTNode *root;
  int num_nodes;
  int max_capacity;
};

bool insert(BSTNode **root,Person * p);
void inOrderPrint(BSTNode *root);
void freeBSTNodes(BSTNode *root);
bool delete(BSTNode **root, int val);
BSTNode *findMin(BSTNode *root);
BSTNode *findBST(BSTNode *root, int val);

PersonStore *personStoreCreateBST(void);
bool BSTPersonStoreInsertPerson(Person * const p,PersonStore *store);
void BSTPersonStorePrintAllPeople(PersonStore *store);
void BSTPersonStoreDeleteAllPeople(PersonStore *store);
bool BSTPersonStoreDeletePerson(int val,PersonStore *store);
Person * BSTPersonStoreFindPerson(int id, PersonStore *store);
void BSTPersonStoreDestroy(PersonStore *store);

PersonStorageOps BSTOps = {
    .insertPerson = BSTPersonStoreInsertPerson,
    .deletePerson = BSTPersonStoreDeletePerson,
    .findPerson = BSTPersonStoreFindPerson,
    .printAllPeople = BSTPersonStorePrintAllPeople,
    .deleteAllPeople = BSTPersonStoreDeleteAllPeople,
    .destroy = BSTPersonStoreDestroy,
    // .sortPeople = arrayPersonStoreBubbleSort,
};

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
    deletePerson(root->value);
    free(root);
}

void inOrderPrint(BSTNode *root){
    if(root == NULL){
        return;
    }
    inOrderPrint(root->left);
    printPerson(root->value);
    // printf("========================================\n");
    // printf(" Node Address : %p\n", (void *)root);
    // printf(" Node Value   : %d\n", root->value->id);
    // printf(" Left Child   : %p\n", (void *)root->left);
    // printf(" Right Child  : %p\n", (void *)root->right);
    // printf("========================================\n\n");
    inOrderPrint(root->right);
}

bool insert(BSTNode **root,Person * p){
    if(*root == NULL){
        BSTNode *node = malloc(sizeof(*node));
        node->value = p;
        node->left = NULL;
        node->right = NULL;
        *root = node;
        return true;
    }
    // If root val is == val (duplicate)
    if((*root)->value->id == p->id){
        return false;
    } // If val is less than root val
    else if(p->id < (*root)->value->id){
        return insert(&(*root)->left,p);
    } // If val is greater than root val
    else {
        return insert(&(*root)->right,p);
    }
    return false;
}

bool delete(BSTNode **root, int val){
    if(*root == NULL){
        return false;
    }
    if(val < (*root)->value->id){
        return delete(&(*root)->left,val);
    }
    if (val > (*root)->value->id){
        return delete(&(*root)->right,val);
    }
    // Found the node
    BSTNode *temp = *root;
    // 1: No children
    if(temp->left == NULL && temp->right == NULL){
        deletePerson(temp->value);
        free(temp);
        *root = NULL;
        return true;
    }
    // 2: Only one child left/rght
    if(temp->left == NULL){
        *root = temp->right;
        deletePerson(temp->value);
        free(temp);
        return true;
    }
    if(temp->right == NULL){
        *root = temp->left;
        deletePerson(temp->value);
        free(temp);
        return true;
    }
    // 3: Both children
    BSTNode *succesor = findMin(temp->right);
    Person *succ_person = succesor->value;
    deletePerson(temp->value);
    temp->value = succ_person;
    succesor->value = NULL;
    return delete(&temp->right, succ_person->id);
}

BSTNode *findBST(BSTNode *root, int val){
    if(root == NULL){
        return NULL;
    }
    if(val == root->value->id){
        return root;
    }
    if(val < root->value->id){
        return findBST(root->left,val);
    }
    return findBST(root->right,val);
}

PersonStore *personStoreCreateBST(void){
    BSTPersonStore *store = malloc(sizeof(*store));
    if(store == NULL){
        return NULL;
    }
    store->max_capacity = MAX_CAPACITY;
    store->num_nodes = 0;
    store->root = NULL;
    store->base.ops = &BSTOps;
    return &store->base;
}

bool BSTPersonStoreInsertPerson(Person * const p,PersonStore *store){
    if(store == NULL || p == NULL){
        return false;
    }
    BSTPersonStore *bst_store = (BSTPersonStore *)store;
    if(bst_store->max_capacity == bst_store->num_nodes){
        return false;
    }
    bool return_value = insert(&bst_store->root,p);
    bst_store->num_nodes += (return_value) ? 1 : 0;
    return return_value;
}

void BSTPersonStorePrintAllPeople(PersonStore *store){
    if(store == NULL){
        return;
    }
    BSTPersonStore *bst_store = (BSTPersonStore *)store;
    if(bst_store->root == NULL){
        printf("No people in store.\n");
    }
    inOrderPrint(bst_store->root);
}

void BSTPersonStoreDeleteAllPeople(PersonStore *store){
    if(store == NULL){
        return;
    }
    BSTPersonStore *bst_store = (BSTPersonStore *)store;
    BSTNode *root = bst_store->root;
    freeBSTNodes(root);
    bst_store->root = NULL;
    bst_store->num_nodes = 0;
    return;
}

bool BSTPersonStoreDeletePerson(int val,PersonStore *store){
    if(store == NULL){
        return false;
    }
    BSTPersonStore *bst_store = (BSTPersonStore *)store;
    bool return_value = delete(&bst_store->root,val);
    bst_store->num_nodes -= (return_value) ? 1 : 0;
    return return_value;
}

Person * BSTPersonStoreFindPerson(int id, PersonStore *store){
    if(store == NULL){
        return NULL;
    }
    BSTPersonStore *bst_store = (BSTPersonStore *)store;
    BSTNode *n = findBST(bst_store->root,id);
    if(n == NULL){
            return NULL;
        }
    return n->value;
}

void BSTPersonStoreDestroy(PersonStore *store){
    if(store == NULL){
        return;
    }
    BSTPersonStoreDeleteAllPeople(store);
    free(store);
    return;
}
