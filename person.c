#include "person.h"
#include <stdio.h>
#include <string.h>

Person *createPerson(int id,bool adult, bool member, char *name){
    Person *p = malloc(sizeof(Person));
    if(p == NULL){
        perror("Malloc error allocating person");
        return NULL;
    }
    p->id = id;
    p->adult = adult;
    p->member = member;
    int length = strlen(name);
    p->name = malloc(sizeof(char) * (length + 1));
    if(p->name == NULL){
        perror("Malloc error allocating name");
        free(p);
        return NULL;
    }
    memcpy(p->name,name,length+1);
    return p;
}

void printPerson(Person *p){
    if(p == NULL){
        perror("Person is NULL");
        return;
    }
    printf("ID: %d , Name: %s , Adult: %d , Member: %d \n", p->id,p->name,p->adult,p->member);
    return;
}

void deletePerson(Person *p){
    if(p == NULL){
        printf("Person does not exist. Already deleted.");
        return;
    }
    free(p->name);
    free(p);
    return;
}

void updateName(Person *p, char *name){
    int length = strlen(name);
    char *new_name = malloc(sizeof(char) * (length + 1));
    if(new_name == NULL){
        perror("Name change failed \n");
        return;
    }
    free(p->name);
    memcpy(new_name,name,length+1);
    p->name = new_name;
    return;
}

void updateStatus(Person *p, bool member){
    p->member = member;
    return;
}
