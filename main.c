#include "person.h"
#include "person_store.h"

#include <stdio.h>
#include <stdbool.h>

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
        // Discard remaining characters.
    }
}

void inputString(char *buffer) {
    printf("Enter person's name: ");
    fflush(stdout);

    int index = 0;
    int c = getchar();

    while (c != EOF && c != '\n' && index < MAX_BUFFER_SIZE - 1) {
        buffer[index++] = (char)c;
        c = getchar();
    }

    buffer[index] = '\0';

    // Discard any remaining characters if the input was too long.
    if (c != '\n' && c != EOF) {
        clearInputBuffer();
    }
}

int inputID(void) {
    int id;

    printf("Enter person ID: ");

    if (scanf("%d", &id) != 1) {
        printf("Invalid input. Please enter a numeric ID.\n");
        clearInputBuffer();
        return -1;
    }

    clearInputBuffer();
    return id;
}

Menu_Command inputMenuCommand(void) {
    int input;

    printf("Select an option (1-6): ");

    if (scanf("%d", &input) != 1) {
        clearInputBuffer();
        return (Menu_Command)0;
    }

    clearInputBuffer();
    return (Menu_Command)input;
}

bool inputStatus(char *str) {
    int input;

    while (1) {
        printf("%s status (1 = Yes, 0 = No): ", str);

        if (scanf("%d", &input) == 1 && (input == 0 || input == 1)) {
            clearInputBuffer();
            return input == 1;
        }

        printf("Invalid input. Please enter 1 for Yes or 0 for No.\n");
        clearInputBuffer();
    }
}

void createPersonFromUser(PersonStore *store) {
    printf("\n========== CREATE PERSON ==========\n");

    inputString(buffer);

    int id = inputID();
    if (id < 0) {
        printf("Person creation cancelled: invalid ID.\n");
        return;
    }

    bool membership_status = inputStatus("Membership");
    bool adult_status = inputStatus("Adult");

    Person *person = createPerson(
        id,
        membership_status,
        adult_status,
        buffer
    );

    if (person == NULL) {
        printf("Error: Could not create person.\n");
        return;
    }

    bool result = personStoreInsertPerson(person, store);

    if (result) {
        printf("Success: Person '%s' (ID: %d) was added.\n", buffer, id);
    } else {
        printf("Error: Could not add person with ID %d.\n", id);

        /*
         * Free person here if the store does not take ownership
         * of the object when insertion fails.
         * Use the appropriate destructor from your person API.
         */
    }
}

void deletePersonFromUser(PersonStore *store) {
    printf("\n========== DELETE PERSON ==========\n");

    int id = inputID();

    if (id < 0) {
        printf("Deletion cancelled: invalid ID.\n");
        return;
    }

    if (personStoreDeletePerson(id, store)) {
        printf("Success: Person with ID %d was deleted.\n", id);
    } else {
        printf("Error: No person found with ID %d. Nothing was deleted.\n", id);
    }
}

void findPerson(PersonStore *store) {
    printf("\n========== FIND PERSON ==========\n");

    int id = inputID();

    if (id < 0) {
        printf("Search cancelled: invalid ID.\n");
        return;
    }

    Person *person = personStoreFindPerson(id, store);

    if (person == NULL) {
        printf("No person found with ID %d.\n", id);
        return;
    }

    printf("\nPerson found:\n");
    printPerson(person);
}

int main(void) {
    PersonStore *store = personStoreCreateHashtable();

    if (store == NULL) {
        printf("Fatal error: Could not initialize the person store.\n");
        return 1;
    }

    Menu_Command command;

    while (1) {
        printf("\n========== PERSON MANAGEMENT ==========\n");
        printf("  1. Display all people\n");
        printf("  2. Add a new person\n");
        printf("  3. Find a person by ID\n");
        printf("  4. Delete a person by ID\n");
        printf("  5. Delete all people\n");
        printf("  6. Exit\n");
        printf("=======================================\n");

        command = inputMenuCommand();

        switch (command) {
            case MENU_PRINT_ALL:
                printf("\n========== ALL PEOPLE ==========\n");
                personStorePrintAllPeople(store);
                break;

            case MENU_CREATE_PERSON:
                createPersonFromUser(store);
                break;

            case MENU_FIND_PERSON:
                findPerson(store);
                break;

            case MENU_DELETE_PERSON:
                deletePersonFromUser(store);
                break;

            case MENU_DELETE_ALL:
                printf("\n========== DELETE ALL PEOPLE ==========\n");
                personStoreDeleteAllPeople(store);
                printf("All people have been deleted.\n");
                break;

            case MENU_EXIT:
                printf("\nExiting Person Management. Goodbye!\n");
                personStoreDestroy(store);
                return 0;

            default:
                printf("Invalid option. Please select a number from 1 to 6.\n");
                break;
        }
    }

    return 0;
}
