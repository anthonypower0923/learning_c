#pragma once
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
