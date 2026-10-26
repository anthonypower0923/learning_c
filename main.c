#include "person.h"
#include "person_store.h"
#include <stdio.h>
#define PERSON_CAPACITY 10
#define MAX_BUFFER_SIZE 256
#define MAX_ID_LENGTH 5

char buffer[MAX_BUFFER_SIZE];

typedef enum {
    MENU_PRINT_ALL = 1,
    MENU_CREATE_PERSON,
    MENU_FIND_PERSON,
    MENU_DELETE_PERSON,
    MENU_DELETE_ALL,
    MENU_EXIT
} Menu_Command;

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Discard remaining characters
    }
}

// int storePerson(Person * const p,Person **arr){
//     if(p == NULL){
//         perror("Can't store person. Person is NULL.");
//         return -1;
//     }
//     for(int i=0;i<PERSON_CAPACITY;i++){
//         if(arr[i] == NULL){
//             arr[i] = p;
//             return i;
//         }
//     }
//     perror("Couldn't store person");
//     return -1;
// }

// void printPersonByIndex(int index, Person *const *arr){
//     if(index < 0 || index >= PERSON_CAPACITY){
//         printf("Invalid index \n");
//         return;
//     }
//     printPerson(arr[index]);
//     return;
// }

// bool deletePersonByIndex(int index, Person **arr){
//     if(index < 0 || index >= PERSON_CAPACITY){
//         printf("Invalid index. Can't delete. \n");
//         return false;
//     }
//     deletePerson(arr[index]);
//     arr[index] = NULL;
//     return true;
// }

// void printAllPeople(Person *const *arr){
//     printf("--------- Printing all people ---------- \n");
//     for(int i=0;i<PERSON_CAPACITY;i++){
//         if(arr[i] != NULL){
//             printf("Index: %d \n" , i);
//             printPerson(arr[i]);
//         }
//     }
//     return;
// }

// void deleteAllPeople(Person **arr){
//     printf("--------- Deleting all people ---------- \n");
//     for(int i=0;i<PERSON_CAPACITY;i++){
//         if(arr[i] != NULL){
//             // printf("Index: %d \n" , i);
//             deletePerson(arr[i]);
//             arr[i] = NULL;
//         }
//     }
//     return;
// }

void inputString(char *buffer){
    printf("Enter your name: \n");
    int index = 0;
    int c = getchar();
    while(c != EOF && c != '\n' && index < (MAX_BUFFER_SIZE - 1 )){
        // putchar(c);
        buffer[index] = (char)c;
        index++;
        c = getchar();
    }
    buffer[index] = '\0';
    return;
}

int inputID(){
    int id;
    printf("Enter your id: \n");
    scanf("%d", &id);
    clearInputBuffer();
    return id;
}

Menu_Command inputMenuCommand(){
    int input;
    printf("Enter your menu choice: \n");
    scanf("%d", &input);
    clearInputBuffer();
    return (Menu_Command)input; //Casting int to menu command.
}

bool inputStatus(char *str) {
    int input;
    printf("Enter %s status (1 = true, 0 = false): ", str);
    scanf("%d", &input);
    clearInputBuffer();
    return input != 0;
}

void createPersonFromUser(PersonStore *store){
    printf("------------- Enter person details -------------- \n");
    inputString(buffer);
    char *name = buffer;
    int id = inputID();
    bool membership_status = inputStatus("Membership");
    bool adult_status = inputStatus("Adult");
    Person *p1 = createPerson(id, membership_status, adult_status, name);
    bool res = personStoreInsertPerson(p1,store);
    printf("Stored person %s \n", (res ) ? "Successfully" : "UnSuccessfully");
}

void deletePersonFromUser(PersonStore *store){
    int id = inputID();
    personStoreDeletePerson(id, store);
    return;
}

Person *findPerson(PersonStore *store){
    int id = inputID();
    personStoreBubbleSort(store);
    Person *p = personStoreFindPerson(id,store);
    return p;
}


int main(){
    // Person **arr = calloc(PERSON_CAPACITY, sizeof(Person *));
    PersonStore *store = personStoreCreateArray();
    bool sorted = true;
    Menu_Command command;

    while(1){
        printf("------- Menu ------- \n");
        printf("1: Print All \n");
        printf("2: Create Person \n");
        printf("3: Find Person \n");
        printf("4: Delete Person \n");
        printf("5: Delete All People \n");
        printf("6: Quit \n");
        printf("-------------------- \n");
        // printf("Enter your choice: \n");
        command = inputMenuCommand();
        switch (command){
            case MENU_PRINT_ALL:
                if (!sorted){
                    personStoreBubbleSort(store);
                }
                personStorePrintAllPeople(store);
                break;
            case MENU_CREATE_PERSON:
                createPersonFromUser(store);
                sorted = false;
                break;
            case MENU_FIND_PERSON:
                findPerson(store);
            case MENU_DELETE_PERSON:
                deletePersonFromUser(store);
                sorted = false;
                break;
            case MENU_DELETE_ALL:
                personStoreDeleteAllPeople(store);
                sorted = true;
                break;
            case MENU_EXIT:
                printf("Quitting... \n");
                personStoreDestroy(store);
                // free(store);
                return 0;
            default:
               break;
        }
    }
    return 0;
}
