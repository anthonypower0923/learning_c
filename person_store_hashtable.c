#include "person.h"
#include "person_store_internal.h"
#include <stdio.h>
#define INITIAL_PERSON_CAPACITY 10

typedef struct HashtableEntry HashtableEntry;
struct HashtableEntry {
    int key;
    Person *person;
    HashtableEntry *next;
};

typedef struct {
    PersonStore base;
    HashtableEntry **buckets;
    size_t capacity; // Number of slots
    size_t count; // Number of elements
} HashtablePersonStore;

PersonStore *personStoreCreateHashtable(void);
bool hashtablePersonStoreInsertPerson(Person * const p,PersonStore *store);
bool hashtablePersonStoreDeletePerson(int id, PersonStore *store);
Person * hashtablePersonStoreFindPerson(int id, PersonStore *store);
void hashtablePersonStorePrintAllPeople(PersonStore *store);
void hashtablePersonStoreDeleteAllPeople(PersonStore *store);
void hashtablePersonStoreDestroy(PersonStore *store);
// void arrayPersonStoreBubbleSort(PersonStore *store);
//
// Internal functions
size_t hash(int id, size_t capacity);

PersonStorageOps hashOps = {
    .insertPerson = hashtablePersonStoreInsertPerson,
    .deletePerson = hashtablePersonStoreDeletePerson,
    .findPerson = hashtablePersonStoreFindPerson,
    .printAllPeople = hashtablePersonStorePrintAllPeople,
    .deleteAllPeople = hashtablePersonStoreDeleteAllPeople,
    .destroy = hashtablePersonStoreDeleteAllPeople,
    // .sortPeople = arrayPersonStoreBubbleSort,
};

PersonStore *personStoreCreateHashtable(void){
    HashtablePersonStore *store = malloc(sizeof(*store));
    if(store == NULL){
        return NULL;
    }
    store->capacity = INITIAL_PERSON_CAPACITY;
    store->count = 0;
    store->buckets = calloc(store->capacity,(sizeof(*store->buckets)));
    if(store->buckets == NULL){
        free(store);
        return NULL;
    }
    store->base.ops = &hashOps;
    return &store->base;
}

size_t hash(int id, size_t capacity){
    size_t h = (size_t)id % capacity;
    return h;
}

bool hashtablePersonStoreInsertPerson(Person * const p,PersonStore *store){
    if(p == NULL){
        return false;
    }
    if(hashtablePersonStoreFindPerson(p->id,store) != NULL){
        printf("Person already in table.\n");
        return false;
    }
    HashtablePersonStore *hash_store = (HashtablePersonStore *)store;
    size_t hash_index = hash(p->id,hash_store->capacity);
    HashtableEntry *entry = malloc(sizeof(*entry));
    if(entry == NULL){
        return false;
    }
    entry->key = p->id;
    entry->person = p;
    HashtableEntry *curr_head = hash_store->buckets[hash_index];
    entry->next = curr_head;
    hash_store->buckets[hash_index] = entry;
    hash_store->count++;
    return true;
}

Person * hashtablePersonStoreFindPerson(int id, PersonStore *store){
    HashtablePersonStore *hash_store = (HashtablePersonStore *)store;
    size_t hash_index = hash(id,hash_store->capacity);
    HashtableEntry *entry = hash_store->buckets[hash_index];
    while(entry != NULL){
        if(entry->key == id){
            return entry->person;
        }
        entry = entry->next;
    }
    // printf("Person not found\n");
    return NULL;
}

bool hashtablePersonStoreDeletePerson(int id, PersonStore *store){
    HashtablePersonStore *hash_store = (HashtablePersonStore *)store;
    size_t hash_index = hash(id,hash_store->capacity);
    HashtableEntry *current = hash_store->buckets[hash_index];
    HashtableEntry *previous = NULL;
    while(current != NULL){
        if(current->key == id){
            // Found at the head.
            if(previous == NULL){
                hash_store->buckets[hash_index] = current->next;
            } else {
                // Found after head.
                previous->next = current->next; // Skipping current
            }
            deletePerson(current->person);
            free(current);
            hash_store->count--;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

void hashtablePersonStoreDeleteAllPeople(PersonStore *store){
    HashtablePersonStore *hash_store = (HashtablePersonStore *)store;
    for(size_t i = 0;i<hash_store->capacity;i++){
        HashtableEntry *entry = hash_store->buckets[i];
        while(entry != NULL){
            HashtableEntry *next = entry->next;
            deletePerson(entry->person);
            free(entry);
            entry = next;
        }
        hash_store->buckets[i] = NULL;
    }
    hash_store->count = 0;
    return;
}

void hashtablePersonStoreDestroy(PersonStore *store){
    HashtablePersonStore *hash_store = (HashtablePersonStore *)store;
    hashtablePersonStoreDeleteAllPeople(store);
    free(hash_store->buckets);
    free(hash_store);
    return;
}

void hashtablePersonStorePrintAllPeople(PersonStore *store){
    HashtablePersonStore *hash_store = (HashtablePersonStore *)store;
    for(size_t i = 0;i<hash_store->capacity;i++){
        HashtableEntry *entry = hash_store->buckets[i];
        while(entry != NULL){
            printPerson(entry->person);
            entry = entry->next;
        }
    }
    return;
}
