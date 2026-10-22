#pragma once
#include <stdbool.h>
#include <stdlib.h>

typedef struct Person {
    int id;
    bool adult;
    bool member;
    char *name;
} Person;

Person *createPerson(int id, bool adult, bool member, char *name);

void printPerson(Person *p);

void updateName(Person *p, char *name);

void updateStatus(Person *p, bool member);

void deletePerson(Person *p);
