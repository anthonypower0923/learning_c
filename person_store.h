#pragma once
#include "person.h"

// Define a forward declaration Struct for storing people. Doesn't need to know what it is storing.
// Method that defines how to create storage implementaion.
typedef struct PersonStore PersonStore;

PersonStore *personStoreCreateArray(void);
//
// Common operations
// - Insert person method
bool personStoreInsertPerson(Person * const p,PersonStore *store);
// - Deletion of person method
bool personStoreDeletePerson(int id, PersonStore *store);
// - Find by index method
Person *personStoreFindPerson(int id, PersonStore *store);
// - Print all method
void personStorePrintAllPeople(PersonStore *store);
// - Clear method
void personStoreDeleteAllPeople(PersonStore *store);

// Destroy person store. Free all people and free storage.
void personStoreDestroy(PersonStore *store);
