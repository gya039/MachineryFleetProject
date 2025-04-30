#include "machinery.h"
#include <conio.h> // used for _getch
#include <ctype.h> // for isprint

// === CORE LINKED LIST OPERATIONS ===

Machinery* createMachinery(char* chassis, char* make, char* model, int year, float cost, float valuation, float mileage, char* ownerName) {
    Machinery* newMachinery = (Machinery*)malloc(sizeof(Machinery));
    strcpy(newMachinery->chassisNumber, chassis);
    strcpy(newMachinery->make, make);
    strcpy(newMachinery->model, model);
    newMachinery->yearOfManufacture = year;
    newMachinery->cost = cost;
    newMachinery->currentValuation = valuation;
    newMachinery->currentMileage = mileage;
    strcpy(newMachinery->ownerName, ownerName);
    newMachinery->next = NULL;
    return newMachinery;
}

void addMachinery(Machinery** head, Machinery* newMachine) {
    newMachine->next = *head;
    *head = newMachine;
}

Machinery* retrieveMachinery(Machinery* head, const char* chassis) {
    while (head != NULL) {
        if (strcmp(head->chassisNumber, chassis) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
#endif
}


void updateMachinery(Machinery* machine) {
    if (!machine) return;

    char buffer[MAX_STR_LENGTH];
    printf("Updating machine: %s\n", machine->chassisNumber);

    getTrimmedInput("Enter new make: ", buffer, MAX_STR_LENGTH);
    if (strlen(buffer) > 0) strcpy(machine->make, buffer);

    getTrimmedInput("Enter new model: ", buffer, MAX_STR_LENGTH);
    if (strlen(buffer) > 0) strcpy(machine->model, buffer);

    machine->yearOfManufacture = getValidatedYear("Enter new year of manufacture: ");
    machine->cost = getValidatedFloat("Enter new cost: ");
    machine->currentValuation = getValidatedFloat("Enter new valuation: ");
    machine->currentMileage = getValidatedFloat("Enter new mileage: ");

    getTrimmedInput("Enter new owner name: ", buffer, MAX_STR_LENGTH);
    if (strlen(buffer) > 0) strcpy(machine->ownerName, buffer);

    printf("Machine updated successfully.\n");
}

void deleteMachinery(Machinery** head, const char* chassis) {
    Machinery* current = *head;
    Machinery* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->chassisNumber, chassis) == 0) {
            if (prev == NULL)
                *head = current->next;
            else
                prev->next = current->next;

            free(current);
            printf("Machine with chassis number %s deleted.\n", chassis);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Machine not found.\n");
}

void displayAllMachinery(Machinery* head) {
    if (!head) {
        printf("No machines found.\n");
        return;
    }

    Machinery* temp = head;
    while (temp != NULL) {
        printf("Chassis: %s | Make: %s | Model: %s | Year: %d\n",
            temp->chassisNumber, temp->make, temp->model, temp->yearOfManufacture);
        printf("Cost: %.2f | Valuation: %.2f | Mileage: %.2f | Owner: %s\n\n",
            temp->cost, temp->currentValuation, temp->currentMileage, temp->ownerName);
        temp = temp->next;
    }
}

// === FILE OPERATIONS ===

void saveMachineryList(Machinery* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    while (head != NULL) {
        fprintf(file, "%s,%s,%s,%d,%.2f,%.2f,%.2f,%s\n",
            head->chassisNumber, head->make, head->model, head->yearOfManufacture,
            head->cost, head->currentValuation, head->currentMileage, head->ownerName);
        head = head->next;
    }

    fclose(file);
    printf("Machinery list saved to '%s'.\n", filename);
}

Machinery* restoreMachineryList(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("No existing fleet file found. Starting fresh.\n");
        return NULL;
    }

    Machinery* head = NULL;
    char buffer[512];

    while (fgets(buffer, sizeof(buffer), file)) {
        char chassis[MAX_STR_LENGTH], make[MAX_STR_LENGTH], model[MAX_STR_LENGTH];
        int year;
        float cost, valuation, mileage;
        char ownerName[MAX_STR_LENGTH];

        sscanf(buffer, "%[^,],%[^,],%[^,],%d,%f,%f,%f,%[^\n]",
            chassis, make, model, &year, &cost, &valuation, &mileage, ownerName);

        Machinery* newMachine = createMachinery(chassis, make, model, year, cost, valuation, mileage, ownerName);
        addMachinery(&head, newMachine);
    }

    fclose(file);
    printf("Machinery list restored from '%s'.\n", filename);
    return head;
}

// === VALIDATION + HELPER INPUTS ===

float getValidatedFloat(const char* prompt) {
    float value;
    char input[50];
    while (1) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%f", &value) == 1 && value >= 0)
            return value;
        printf("Invalid input. Please enter a non-negative number.\n");
    }
}

int getValidatedYear(const char* prompt) {
    int year;
    char input[50];
    while (1) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &year) == 1 && year >= 1900 && year <= 2025)
            return year;
        printf("Invalid year. Please enter a value between 1900 and 2025.\n");
    }
}

void getTrimmedInput(const char* prompt, char* buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

// === LOGIN SYSTEM ===

void getMaskedPassword(char* password) {
    int i = 0;
    char ch;
    while ((ch = _getch()) != '\r' && i < PASSWORD_LEN - 1) {
        if (ch == '\b' && i > 0) {
            printf("\b \b");
            i--;
        }
        else if (isprint(ch)) {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

void loadLogins(Login logins[], int* count) {
    FILE* fp = fopen("login.txt", "r");
    if (!fp) {
        printf("Failed to open login.txt\n");
        exit(1);
    }

    char user[USERNAME_LEN], pass[PASSWORD_LEN];
    *count = 0;

    while (fscanf(fp, "%6s %6s", user, pass) == 2 && *count < MAX_USERS) {
        strcpy(logins[*count].username, user);
        strcpy(logins[*count].password, pass);
        (*count)++;
    }

    fclose(fp);
}

int authenticate(Login logins[], int count) {
    char enteredUser[USERNAME_LEN];
    char enteredPass[PASSWORD_LEN];
    int attempts = 0;

    while (attempts < 5) {
        printf("\nUsername: ");
        scanf("%6s", enteredUser);
        getchar(); // flush newline
        printf("Password: ");
        getMaskedPassword(enteredPass);

        for (int i = 0; i < count; i++) {
            if (strcmp(enteredUser, logins[i].username) == 0 &&
                strcmp(enteredPass, logins[i].password) == 0) {
                printf("Login successful.\n");
                return 1;
            }
        }

        printf("Invalid login. %d attempt(s) remaining.\n", 4 - attempts);
        attempts++;
    }

    return 0;
}
