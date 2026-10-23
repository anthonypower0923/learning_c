#include "person_store.h"
#include "person.h"
#include <stdlib.h>
#define INITIAL_PERSON_CAPACITY 3

typedef struct{
    Person **people;
    size_t capacity; // Number of slots
    size_t count; // Number of occupied slots
} ArrayPersonStore;

// method stubs
ArrayPersonStore *arrayPersonStoreCreateArray(void);
bool arrayPersonStoreInsertPerson(Person * const p,PersonStore *store);
bool arrayPersonStoreDeletePerson(int id, PersonStore *store);
Person *arrayPersonStoreFindPerson(int id, PersonStore *store);
void arrayPersonStorePrintAllPeople(PersonStore *store);
void arrayPersonStoreDeleteAllPeople(PersonStore *store);
void arrayPersonStoreDestroy(PersonStore *store);

// internal method stubs
bool arrayPersonStoreIsFull(ArrayPersonStore *store);
bool arrayPersonStoreGrow(ArrayPersonStore *store);

ArrayPersonStore *arrayPersonStoreCreateArray(void){
    ArrayPersonStore *store = malloc(sizeof(ArrayPersonStore));
    if(store == NULL){
        return NULL;
    }
    store->people = calloc(INITIAL_PERSON_CAPACITY, sizeof(Person *)); // Calloc intialises array elements to null.
    if(store->people == NULL){
        free(store);
        return NULL;
    }
    store->capacity = INITIAL_PERSON_CAPACITY;
    store->count = 0;
    return store;
}

bool arrayPersonStoreIsFull(ArrayPersonStore *store){
    if(store == NULL){
        return false;
    }
    bool result = (store->count >= store->capacity) ? true : false;
    return result;
}

bool arrayPersonStoreGrow(ArrayPersonStore *store){
    size_t new_capacity = store->capacity * 2;
    Person **new_people = realloc(store->people,(new_capacity * sizeof(Person *)));
    if(new_people == NULL){
        return false;
    }
    // Initialise new elements to null.
    for(int i=store->capacity;i<new_capacity;i++){
       new_people[i] = NULL;
    }
    store->people = new_people;
    store->capacity = new_capacity;
    return true;
}
