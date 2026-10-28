#include "person_store.h"
#include "person_store_internal.h"

bool personStoreInsertPerson(Person * const p,PersonStore *store){
    return store->ops->insertPerson(p,store);
}
// - Deletion of person method
bool personStoreDeletePerson(int id, PersonStore *store){
    return store->ops->deletePerson(id,store);
}
// - Find by index method
Person *personStoreFindPerson(int id, PersonStore *store){
    return store->ops->findPerson(id,store);
}
// - Print all method
void personStorePrintAllPeople(PersonStore *store){
    return store->ops->printAllPeople(store);
}
// - Clear method
void personStoreDeleteAllPeople(PersonStore *store){
    return store->ops->deleteAllPeople(store);
}

// Destroy person store. Free all people and free storage.
void personStoreDestroy(PersonStore *store){
    return store->ops->destroy(store);
}

// void personStoreBubbleSort(PersonStore *store){
//     return store->ops->sortPeople(store);
// }
