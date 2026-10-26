#pragma once
#include "person_store.h"

typedef struct PersonStorageOps{
    bool (* insertPerson)(Person * const p,PersonStore *store);
    bool (* deletePerson)(int id, PersonStore *store);
    Person * (* findPerson)(int id, PersonStore *store);
    void (* printAllPeople)(PersonStore *store);
    void (* deleteAllPeople)(PersonStore *store);
    void (* destroy)(PersonStore *store);
    void (* sortPeople)(PersonStore *store);
} PersonStorageOps;

// Internal definition of PersonStore
struct PersonStore{
    PersonStorageOps *ops;
};
