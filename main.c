#include "person.h"
#include "person_store.h"
#include "person_store_bst.h"

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



    printf("\n========== BST TESTING ==========\n");

    // 1. CREATE BST
    printf("\n--- CREATE BST ---\n");

    BSTPersonStore *bst_store = personStoreCreateBST();

    if(bst_store == NULL){
        printf("Failed to create BST.\n");
        return EXIT_FAILURE;
    }

    printf("BST successfully created.\n");

    // 2. INSERT PEOPLE
    printf("\n--- INSERT PEOPLE ---\n");

    int values[] = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65};

    int size = sizeof(values) / sizeof(values[0]);

    for(int i = 0; i < size; i++){
        printf("Insert %d: %s\n",
            values[i],
            BSTPersonStoreInsertPerson(values[i], bst_store)
                ? "SUCCESS" : "FAILED"
        );
    }

    // Test duplicate insertion
    printf("\n--- DUPLICATE INSERTION ---\n");

    printf("Insert 50 again: %s\n",
        BSTPersonStoreInsertPerson(50, bst_store)
            ? "SUCCESS" : "REJECTED"
    );

    // 3. PRINT ALL PEOPLE
    printf("\n--- PRINT ALL PEOPLE ---\n");

    BSTPersonStorePrintAllPeople(bst_store);

    // 4. FIND PEOPLE
    printf("\n--- FIND PEOPLE ---\n");

    int search_values[] = {50, 20, 65, 100};
    int search_size = sizeof(search_values) / sizeof(search_values[0]);

    for(int i = 0; i < search_size; i++){

        BSTNode *found = BSTPersonStoreFindPerson(
            search_values[i], bst_store
        );

        printf("Find %d: %s\n",
            search_values[i],
            found != NULL ? "FOUND" : "NOT FOUND"
        );
    }

    // 5. DELETE LEAF NODE
    printf("\n--- DELETE LEAF NODE ---\n");

    printf("Delete 20: %s\n",
        BSTPersonStoreDeletePerson(20, bst_store)
            ? "SUCCESS" : "FAILED"
    );

    BSTPersonStorePrintAllPeople(bst_store);

    // 6. DELETE NODE WITH ONE CHILD
    printf("\n--- DELETE NODE WITH ONE CHILD ---\n");

    BSTPersonStoreDeletePerson(35, bst_store);

    printf("Delete 40: %s\n",
        BSTPersonStoreDeletePerson(40, bst_store)
            ? "SUCCESS" : "FAILED"
    );

    BSTPersonStorePrintAllPeople(bst_store);

    // 7. DELETE NODE WITH TWO CHILDREN
    printf("\n--- DELETE NODE WITH TWO CHILDREN ---\n");

    printf("Delete 70: %s\n",
        BSTPersonStoreDeletePerson(70, bst_store)
            ? "SUCCESS" : "FAILED"
    );

    BSTPersonStorePrintAllPeople(bst_store);

    // 8. DELETE ROOT NODE
    printf("\n--- DELETE ROOT NODE ---\n");

    printf("Delete 50: %s\n",
        BSTPersonStoreDeletePerson(50, bst_store)
            ? "SUCCESS" : "FAILED"
    );

    BSTPersonStorePrintAllPeople(bst_store);

    // 9. DELETE NONEXISTENT NODE
    printf("\n--- DELETE NONEXISTENT NODE ---\n");

    printf("Delete 999: %s\n",
        BSTPersonStoreDeletePerson(999, bst_store)
            ? "SUCCESS" : "CORRECTLY REJECTED"
    );

    // 10. FIND DELETED NODE
    printf("\n--- FIND DELETED NODE ---\n");

    printf("Find 50: %s\n",
        BSTPersonStoreFindPerson(50, bst_store) != NULL
            ? "FOUND" : "CORRECTLY NOT FOUND"
    );

    // 11. DELETE ALL PEOPLE
    printf("\n--- DELETE ALL PEOPLE ---\n");

    printf("Delete all: %s\n",
        BSTPersonStoreDeleteAllPeople(bst_store)
            ? "SUCCESS" : "FAILED"
    );

    BSTPersonStorePrintAllPeople(bst_store);

    // 12. INSERT AFTER DELETE ALL
    printf("\n--- INSERT AFTER CLEARING ---\n");

    printf("Insert 100: %s\n",
        BSTPersonStoreInsertPerson(100, bst_store)
            ? "SUCCESS" : "FAILED"
    );

    BSTPersonStorePrintAllPeople(bst_store);

    // 13. TEST MAXIMUM CAPACITY
    printf("\n--- TEST MAXIMUM CAPACITY ---\n");

    BSTPersonStoreDeleteAllPeople(bst_store);

    bool all_inserted = true;

    for(int i = 1; i <= 15; i++){
        if(!BSTPersonStoreInsertPerson(i, bst_store)){
            all_inserted = false;
            printf("Failed to insert %d.\n", i);
        }
    }

    printf("Insert 15 nodes: %s\n",
        all_inserted ? "SUCCESS" : "FAILED"
    );

    printf("Insert 16th node: %s\n",
        BSTPersonStoreInsertPerson(16, bst_store)
            ? "UNEXPECTED SUCCESS" : "CORRECTLY REJECTED"
    );

    // 14. DELETE ALL AGAIN
    printf("\n--- DELETE ALL AGAIN ---\n");

    BSTPersonStoreDeleteAllPeople(bst_store);

    BSTPersonStorePrintAllPeople(bst_store);

    // 15. DESTROY BST
    printf("\n--- DESTROY BST ---\n");

    BSTPersonStoreDestroy(bst_store);
    bst_store = NULL;

    printf("BST successfully destroyed.\n");

    printf("\n========== TESTING COMPLETE ==========\n");

    return EXIT_SUCCESS;

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
