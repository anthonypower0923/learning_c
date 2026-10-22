#include "person.h"
#include <stdio.h>
#define PERSON_CAPACITY 10
#define MAX_BUFFER_SIZE 256
#define MAX_ID_LENGTH 5

char buffer[MAX_BUFFER_SIZE];

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Discard remaining characters
    }
}

int storePerson(Person *p,Person **arr){
    if(p == NULL){
        perror("Can't store person. Person is NULL.");
        return -1;
    }
    for(int i=0;i<PERSON_CAPACITY;i++){
        if(arr[i] == NULL){
            arr[i] = p;
            return i;
        }
    }
    perror("Couldn't store person");
    return -1;
}

void printPersonByIndex(int index, Person **arr){
    if(index < 0 || index >= PERSON_CAPACITY){
        printf("Invalid index \n");
        return;
    }
    printPerson(arr[index]);
    return;
}

bool deletePersonByIndex(int index, Person **arr){
    if(index < 0 || index >= PERSON_CAPACITY){
        printf("Invalid index. Can't delete. \n");
        return false;
    }
    deletePerson(arr[index]);
    arr[index] = NULL;
    return true;
}

void printAllPeople(Person **arr){
    printf("--------- Printing all people ---------- \n");
    for(int i=0;i<PERSON_CAPACITY;i++){
        if(arr[i] != NULL){
            printf("Index: %d \n" , i);
            printPerson(arr[i]);
        }
    }
    return;
}

void deleteAllPeople(Person **arr){
    printf("--------- Deleting all people ---------- \n");
    for(int i=0;i<PERSON_CAPACITY;i++){
        if(arr[i] != NULL){
            // printf("Index: %d \n" , i);
            deletePerson(arr[i]);
            arr[i] = NULL;
        }
    }
    return;
}

void inputName(char *buffer){
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
    printf("Enter you id: \n");
    scanf("%d", &id);
    clearInputBuffer();
    return id;
}

bool inputStatus(char *str) {
    int input;
    printf("Enter %s status (1 = true, 0 = false): ", str);
    scanf("%d", &input);
    clearInputBuffer();
    return input != 0;
}

void createPersonFromUser(Person **arr){
    printf("Enter person details: \n");
    inputName(buffer);
    char *name = buffer;
    int id = inputID();
    bool membership_status = inputStatus("Membership");
    bool adult_status = inputStatus("Adult");
    Person *p1 = createPerson(id, membership_status, adult_status, name);
    int index = storePerson(p1,arr);
    printf("Stored person %s \n", (index == -1) ? "Unsuccessfully" : "Successfully");
}

void deletePersonFromUser(Person **arr){
    int index = inputID();
    deletePersonByIndex(index, arr);
    return;
}

int main(){
    Person **arr = calloc(PERSON_CAPACITY, sizeof(Person *));
    int command;
    while(1){
        printf("------- Menu ------- \n");
        printf("1: Print All \n");
        printf("2: Create Person \n");
        printf("3: Delete Person \n");
        printf("4: Delete All People \n");
        printf("5: Quit \n");
        printf("-------------------- \n");
        printf("Enter your choice: \n");
        command = inputID();
        switch (command){
            case 1:
                printAllPeople(arr);
                break;
            case 2:
                createPersonFromUser(arr);
                break;
            case 3:
                deletePersonFromUser(arr);
                break;
            case 4:
                deleteAllPeople(arr);
                break;
            case 5:
                printf("Quitting... \n");
                deleteAllPeople(arr);
                free(arr);
                return 0;
            default:
               break;
        }
    }
    // char *name = "Zoe";
    // inputName(buffer);
    // char *name = buffer;
    // int id = inputID();
    // bool membership_status = inputStatus("Membership");
    // bool adult_status = inputStatus("Adult");
    // Person *p1 = createPerson(id, membership_status, adult_status, name);
    // inputName(buffer);
    // name = buffer;
    // Person *p2 = createPerson(6, true, true, name);
    // inputName(buffer);
    // name = buffer;
    // Person *p3 = createPerson(8, false, false, name);
    // Person *p2 = createPerson(5, true, false, "Bob");
    // Person *p3 = createPerson(7, false, false, "Sally");
    // // if(p == NULL){
    // //     perror("Person is NULL");
    // //     return 1;
    // // }
    // storePerson(p1, arr);
    // storePerson(p2, arr);
    // storePerson(p3, arr);
    // // if(index != -1){
    // //     printPersonByIndex(index,arr);
    // // }
    // // printPerson(p);
    // // char *new_name = "John";
    // // updateName(p,new_name);
    // // updateStatus(p, true);
    // printAllPeople(arr);
    // // if(index != -1){
    // //     bool deleted = deletePersonByIndex(index,arr);
    // //     printf("Was person deleted: %d \n" , deleted);
    // // }
    // // deletePerson(p);
    // deletePersonByIndex(1,arr);
    // printAllPeople(arr);
    // Person *p4 = createPerson(7, false, false, "Mike");
    // storePerson(p4, arr);
    // printAllPeople(arr);
    return 0;
}
