#include "person.h"
#include "person_store_internal.h"
#include <stdlib.h>
#include <stdio.h>
#define INITIAL_PERSON_CAPACITY 3

typedef struct{
    PersonStore base;
    Person **people;
    size_t capacity; // Number of slots
    size_t count; // Number of occupied slots
} ArrayPersonStore;


// method stubs
PersonStore *personStoreCreateArray(void);
bool arrayPersonStoreInsertPerson(Person * const p,PersonStore *store);
bool arrayPersonStoreDeletePerson(int id, PersonStore *store);
Person * arrayPersonStoreFindPerson(int id, PersonStore *store);
void arrayPersonStorePrintAllPeople(PersonStore *store);
void arrayPersonStoreDeleteAllPeople(PersonStore *store);
void arrayPersonStoreDestroy(PersonStore *store);

// internal method stubs
bool arrayPersonStoreIsFull(ArrayPersonStore *store);
bool arrayPersonStoreGrow(ArrayPersonStore *store);

PersonStorageOps arrayOps = {
    .insertPerson = arrayPersonStoreInsertPerson,
    .deletePerson = arrayPersonStoreDeletePerson,
    .findPerson = arrayPersonStoreFindPerson,
    .printAllPeople = arrayPersonStorePrintAllPeople,
    .deleteAllPeople = arrayPersonStoreDeleteAllPeople,
    .destroy = arrayPersonStoreDestroy
};

PersonStore *personStoreCreateArray(void){
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
    store->base.ops = &arrayOps;
    return &store->base;
}

bool arrayPersonStoreIsFull(ArrayPersonStore *store){
    if(store == NULL){
        return false;
    }
    bool result = (store->count >= store->capacity) ? true : false;
    return result;
}

bool arrayPersonStoreGrow(ArrayPersonStore *store){
    if(store == NULL){
        printf("Store is NULL. Can't grow. \n");
        return false;
    }
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

bool arrayPersonStoreInsertPerson(Person * const p,PersonStore *store){
    if(store == NULL){
        printf("Input is NULL. Can't insert.\n");
        return false;
    }
    // Cast genric person store to array person store
    ArrayPersonStore *array_store = (ArrayPersonStore *)store;
    // Check if we have space.
    if(arrayPersonStoreIsFull(array_store)){
        if(!(arrayPersonStoreGrow(array_store))){
            printf("Couldn't resize.\n");
            return false;
        }
    }
    for(size_t i=0;i<array_store->capacity;i++){
        if(array_store->people[i] == NULL){
            array_store->people[i] = p;
            array_store->count++;
            return true;
        }
    }
    return false;
}

void arrayPersonStorePrintAllPeople(PersonStore *store){
    if(store == NULL){
        printf("Input is NULL. Can't print.\n");
        return;
    }
    ArrayPersonStore *array_store = (ArrayPersonStore *)store;

    if(array_store->count == 0){
        printf("Store is empty.\n");
        return;
    }

    for(size_t i=0;i<array_store->capacity;i++){
        if(array_store->people[i] != NULL){
            printPerson(array_store->people[i]);
        }
    }
    return;
}

bool arrayPersonStoreDeletePerson(int id, PersonStore *store){
    if(store == NULL){
        printf("Input is NULL. Can't print.\n");
        return false;
    }
    ArrayPersonStore *array_store = (ArrayPersonStore *)store;

    for(size_t i=0;i<array_store->capacity;i++){
        if(array_store->people[i] != NULL && array_store->people[i]->id == id){
            deletePerson(array_store->people[i]);
            array_store->people[i] = NULL;
            array_store->count--;
            return true;
        }
    }
    return false;
}

void arrayPersonStoreDeleteAllPeople(PersonStore *store){
    if(store == NULL){
        printf("Input is NULL. Can't delete all.\n");
        return;
    }
    ArrayPersonStore *array_store = (ArrayPersonStore *)store;

    for(size_t i=0;i<array_store->capacity;i++){
        if(array_store->people[i] != NULL){
            deletePerson(array_store->people[i]);
            array_store->people[i] = NULL;
        }
    }
    array_store->count = 0;
    return;
}

Person * arrayPersonStoreFindPerson(int id, PersonStore *store){
    if(store == NULL){
        printf("Input is NULL. Can't find.\n");
        return NULL;
    }
    ArrayPersonStore *array_store = (ArrayPersonStore *)store;

    for(size_t i=0;i<array_store->capacity;i++){
        if(array_store->people[i] != NULL && array_store->people[i]->id == id){
            return array_store->people[i];
        }
    }
    return NULL;
}

void arrayPersonStoreDestroy(PersonStore *store){
    if(store == NULL){
        printf("Input is NULL. Can't destroy.\n");
        return;
    }
    arrayPersonStoreDeleteAllPeople(store);
    ArrayPersonStore *array_store = (ArrayPersonStore *)store;
    free(array_store->people);
    free(array_store);
    return;
}
