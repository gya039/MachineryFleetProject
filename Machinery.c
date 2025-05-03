#include "machinery.h"

const char* machineTypeStrings[] = {
    "Tractor",
    "Excavator",
    "Roller",
    "Crane",
    "Mixer"
};

const char* breakdownExperienceStrings[] = {
    "Never",
    "Less than three times",
    "Less than five times",
    "More than five times"
};

void clearConsole() {
#ifdef _WIN32
    system("cls");  // For Windows :)
#else
    printf("\033[H\033[J");  // For Linux/Mac
#endif
}

void validateStringInput(char* input, int maxLength) {
    char tempInput[256];  // buffer to handle input

    while (1) {
        fgets(tempInput, sizeof(tempInput), stdin);

        // Remove newline character from fgets
        tempInput[strcspn(tempInput, "\n")] = 0;

        if (strlen(tempInput) > maxLength) {
            printf("Input too long! Please try again: ");
        }
        else {
            strncpy(input, tempInput, maxLength);  // makes the input the original variable
            break;
        }
    }
}

void getFullNameInput(const char* prompt, char* buffer, int size) {
    int valid = 0;
    while (!valid) {
        printf("%s", prompt);
        fgets(buffer, size, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // remove newline

        char* trimmed = buffer;
        while (*trimmed == ' ') trimmed++; // skips spaces so doesn't show as ( John) but as (John)

        // Check for at least one space between words
        if (strlen(trimmed) == 0 || strchr(trimmed, ' ') == NULL) {
            printf("Please enter a full name (at least two words).\n");
        }
        else {
            valid = 1;
            strcpy(buffer, trimmed); //saves the trimmed version
        }
    }
}

void getNonEmptyInput(const char* prompt, char* buffer, int size) {
    do {
        printf("%s", prompt);
        fgets(buffer, size, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // remove newline
        if (strlen(buffer) == 0) {
            printf("Input cannot be empty. Please try again.\n");
        }
    } while (strlen(buffer) == 0);
}

void validateMileage(float* mileage, float* nextService) {
    while (*nextService <= *mileage) {
        printf("Next service mileage must be greater than current mileage.\nPlease enter next service mileage: ");
        validateFloatInput(nextService);
    }
}

int validatePhoneNumber(const char* phone) {
    // Check length first (12 characters total including 2 dashes)
    if (strlen(phone) != 12) return 0;

    // Check the structure: XXX-XXX-XXXX
    for (int i = 0; i < 12; i++) {
        if (i == 3 || i == 7) {
            if (phone[i] != '-') return 0; // must be dashes
        }
        else {
            if (!isdigit(phone[i])) return 0; // all other chars must be digits
        }
    }
    return 1;
}

void loadLogins(Login logins[], int* count) {
    FILE* fp = fopen("login.txt", "r");
    if (fp == NULL) {
        printf("Error opening login file.\n");
        exit(1);
    }

    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
    *count = 0;

    while (fscanf(fp, "%6s %6s", username, password) == 2 && *count < MAX_USERS) {
        strcpy(logins[*count].username, username);
        strcpy(logins[*count].password, password);
        (*count)++;
    }

    fclose(fp);
}

void displayChassisNumbers(Machinery* head) {
    printf("\nAvailable Chassis Numbers:\n");
    while (head) {
        printf("%s\n", head->chassisNumber);
        head = head->next;
    }
}

int chassisExists(Machinery* head, const char* chassis) {
    while (head != NULL) {
        if (strcmp(head->chassisNumber, chassis) == 0) {
            return 1; // Found a match
        }
        head = head->next;
    }
    return 0; // No match
}

void getMaskedPassword(char* password) {
    int i = 0;
    char ch;

    while ((ch = _getch()) != '\r' && i < PASSWORD_LEN - 1) {  // '\r' is Enter
        if (ch == '\b' && i > 0) {  // Backspace
            printf("\b \b");
            i--;
        }
        else if (ch != '\b') {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

int authenticate(Login logins[], int count) {
    char enteredUser[USERNAME_LEN];
    char enteredPass[PASSWORD_LEN];
    int attempts = 0;
    const int maxAttempts = 5;

    while (attempts < maxAttempts) {
        // Get Username (must be exactly 6 characters)
        while (1) {
            printf("\nUsername: ");
            int i = 0;
            char ch;
            while (1) {
                ch = _getch();
                if (ch == '\r') break;

                if (ch == '\b' && i > 0) {
                    printf("\b \b");
                    i--;
                }
                else if (ch != '\b') {
                    if (i < 6) {
                        enteredUser[i++] = ch;
                        printf("%c", ch);
                    }
                    else {
                        printf("\a");
                    }
                }
            }
            enteredUser[i] = '\0';

            if (i != 6) {
                printf("\nUsername must be 6 characters long. Please try again.\n");
            }
            else {
                break; // valid username
            }
        }

        // Get Password (must be 6 characters)
        while (1) {
            printf("\nPassword: ");
            getMaskedPassword(enteredPass);

            if (strlen(enteredPass) < 6) {
                printf("\nPassword must be 6 characters long. Please try again.");
            }
            else {
                break; // valid password
            }
        }

        // Check login credentials
        for (int i = 0; i < count; i++) {
            if (strcmp(enteredUser, logins[i].username) == 0 &&
                strcmp(enteredPass, logins[i].password) == 0) {
                printf("Login successful.\n");
                return 1;
            }
        }

        attempts++;
        if (attempts < maxAttempts) {
            printf("Invalid login. You have %d attempt%s left.\n",
                maxAttempts - attempts,
                maxAttempts - attempts == 1 ? "" : "s");
        }
    }

    return 0; // All attempts used ( we have 5 here)
}

int validateFloatInput(float* input) {
    char buffer[50];
    char extra;
    int valid = 0;

    while (!valid) {
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // Remove newline

        if (sscanf(buffer, "%f %c", input, &extra) != 1) {
            printf("Invalid input! Please enter a valid number: ");
        }
        else if (*input < 0) {
            printf("Input cannot be negative! Please enter a valid number: ");
        }
        else {
            valid = 1;
        }
    }
    return 1;
}

int validateEmail(char* email) {
    int atCount = 0, dotCount = 0;
    int i = 0;

    while (email[i] != '\0') {
        if (email[i] == '@') atCount++;
        if (email[i] == '.') dotCount++;
        i++;
    }

    return (atCount == 1 && dotCount >= 1 && strstr(email, ".com") != NULL);
}

int validateYear(int* year) {
    char buffer[50];
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", year) == 1 && *year >= 1000 && *year <= 9999) {
            return 1;  // valid year
        }
        else {
            printf("Invalid year! Please enter a valid 4-digit year: ");
        }
    }
}

int validateNonBlankInput(char* input) {
    while (1) {
        fgets(input, MAX_STR_LENGTH, stdin);
        input[strcspn(input, "\n")] = 0; // Remove trailing newline
        if (strlen(input) == 0) {
            printf("Input cannot be blank! Please enter a valid value: ");
        }
        else {
            return 1;
        }
    }
}

void displayAllMachinery(Machinery* head) {
    Machinery* temp = head;
    while (temp != NULL) {
        printf("Chassis: %s | Make: %s | Model: %s | Year: %d | Valuation: %.2f\n",
            temp->chassisNumber, temp->make, temp->model, temp->yearOfManufacture, temp->currentValuation);
        temp = temp->next;
    }
}

void displayMachineDetails(Machinery* machine) {
    if (machine == NULL) return;

    printf("\n--- Machinery Details ---\n");
    printf("Chassis Number: %s\n", machine->chassisNumber);
    printf("Make: %s\n", machine->make);
    printf("Model: %s\n", machine->model);
    printf("Year: %d\n", machine->yearOfManufacture);
    printf("Cost: %.2f\n", machine->cost);
    printf("Valuation: %.2f\n", machine->currentValuation);
    printf("Mileage: %.2f\n", machine->currentMileage);
    printf("Next Service: %.2f\n", machine->nextServiceMileage);
    printf("Owner: %s | Email: %s | Phone: %s\n", machine->ownerName, machine->ownerEmail, machine->ownerPhoneNumber);

    // Machine Type and Breakdowns
    printf("Machine Type: %s | ", machineTypeStrings[machine->machineType]);

    if (machine->breakdownsThisYear >= 0 && machine->breakdownsThisYear < 4) {
        printf("Breakdowns: %s\n", breakdownExperienceStrings[machine->breakdownsThisYear]);
    }
    else {
        printf("Breakdowns: Invalid value\n");
    }

    // Pause before returning to the main menu
    getchar();  // Wait for user input
}

void generateStatistics(Machinery* head) {
    int typeCounts[5] = { 0 }, noBreakdowns[5] = { 0 };
    Machinery* temp = head;

    while (temp) {
        typeCounts[temp->machineType]++;
        if (temp->breakdownsThisYear == NEVER)
            noBreakdowns[temp->machineType]++;
        temp = temp->next;
    }

    for (int i = 0; i < 5; i++) {
        if (typeCounts[i] > 0) {
            float percent = (float)noBreakdowns[i] / typeCounts[i] * 100;
            printf("%.2f%% of %s had no breakdowns.\n", percent, machineTypeStrings[i]);
        }
        else {
            printf("No %s found.\n", machineTypeStrings[i]);
        }
    }
}

void printReportToFile(Machinery* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error writing report file.\n");
        return;
    }

    int totalMachines = 0;
    int noBreakdowns = 0, oneToTwoBreakdowns = 0, threeToFiveBreakdowns = 0, moreThanFiveBreakdowns = 0;
    int machineTypeCounts[5] = { 0 };  //  5 machine types

    Machinery* temp = head;
    while (temp != NULL) {
        totalMachines++;

        // Print machine details
        fprintf(file, "Chassis: %s | Make: %s | Model: %s | Year: %d | Cost: %.2f | Valuation: %.2f\n",
            temp->chassisNumber, temp->make, temp->model, temp->yearOfManufacture, temp->cost, temp->currentValuation);
        fprintf(file, "Mileage: %.2f | Next Service Mileage: %.2f\n", temp->currentMileage, temp->nextServiceMileage);
        fprintf(file, "Owner: %s | Email: %s | Phone: %s\n", temp->ownerName, temp->ownerEmail, temp->ownerPhoneNumber);
        fprintf(file, "Machine Type: %s | Breakdown Experience: %s\n\n",
            machineTypeStrings[temp->machineType], breakdownExperienceStrings[temp->breakdownsThisYear]);

        // Count breakdown experience for statistics
        switch (temp->breakdownsThisYear) {
        case 0: noBreakdowns++; break;
        case 1: oneToTwoBreakdowns++; break;
        case 2: threeToFiveBreakdowns++; break;
        case 3: moreThanFiveBreakdowns++; break;
        }

        // Count machine types for statistics
        if (temp->machineType >= 0 && temp->machineType < 5) {
            machineTypeCounts[temp->machineType]++;
        }

        temp = temp->next;
    }

    // Print the statistics section
    fprintf(file, "\n----- Machine Performance Statistics -----\n");

    if (totalMachines > 0) {
        // Updated machine type stats
        for (int i = 0; i < 5; i++) {
            fprintf(file, "%.2f%% of machines are %s\n",
                (machineTypeCounts[i] * 100.0) / totalMachines, machineTypeStrings[i]);
        }
    }
    else {
        fprintf(file, "No machines to report on.\n");
    }

    // Print the breakdown levels section at the bottom 
    fprintf(file, "\n----- Breakdown Levels -----\n");

    if (totalMachines > 0) {
        fprintf(file, "%.2f%% of machines have no breakdowns.\n", (noBreakdowns * 100.0) / totalMachines);
        fprintf(file, "%.2f%% of machines have 1-2 breakdowns.\n", (oneToTwoBreakdowns * 100.0) / totalMachines);
        fprintf(file, "%.2f%% of machines have 3-5 breakdowns.\n", (threeToFiveBreakdowns * 100.0) / totalMachines);
        fprintf(file, "%.2f%% of machines have more than 5 breakdowns.\n", (moreThanFiveBreakdowns * 100.0) / totalMachines);
    }

    fclose(file);
    printf("Report saved to %s\n", filename);
}

Machinery* sortByValuation(Machinery* head) {
    if (!head || !head->next) return head;

    for (Machinery* i = head; i != NULL; i = i->next) {
        for (Machinery* j = i->next; j != NULL; j = j->next) {
            if (i->currentValuation < j->currentValuation) {
                Machinery temp = *i;
                *i = *j;
                *j = temp;

                Machinery* tempNext = i->next;
                i->next = j->next;
                j->next = tempNext;
            }
        }
    }
    return head;
}

Machinery* createMachinery(char* chassis, char* make, char* model, int year, float cost, float valuation,
    float mileage, float nextService, char* ownerName, char* ownerEmail,
    char* ownerPhone, MachineType type, BreakdownExperience breakdowns) {
    Machinery* newMachinery = (Machinery*)malloc(sizeof(Machinery));
    strcpy(newMachinery->chassisNumber, chassis);
    strcpy(newMachinery->make, make);
    strcpy(newMachinery->model, model);
    newMachinery->yearOfManufacture = year;
    newMachinery->cost = cost;
    newMachinery->currentValuation = valuation;
    newMachinery->currentMileage = mileage;
    newMachinery->nextServiceMileage = nextService;
    strcpy(newMachinery->ownerName, ownerName);
    strcpy(newMachinery->ownerEmail, ownerEmail);
    strcpy(newMachinery->ownerPhoneNumber, ownerPhone);
    newMachinery->machineType = type;
    newMachinery->breakdownsThisYear = breakdowns;
    newMachinery->next = NULL;
    return newMachinery;
}

void addMachinery(Machinery** head, Machinery* newMachine) {
    newMachine->next = *head;
    *head = newMachine;
}

Machinery* retrieveMachinery(Machinery* head, const char* chassis) {
    while (head != NULL) {
        if (strcmp(head->chassisNumber, chassis) == 0) return head;
        head = head->next;
    }
    return NULL;
}

void deleteMachinery(Machinery** head, const char* chassis) {
    Machinery* current = *head;
    Machinery* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->chassisNumber, chassis) == 0) {
            if (prev == NULL) *head = current->next;
            else prev->next = current->next;
            free(current);
            printf("Machine deleted.\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Machine not found.\n");
}

void saveMachineryList(Machinery* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;

    while (head != NULL) {
        fprintf(file, "%s,%s,%s,%d,%.2f,%.2f,%.2f,%.2f,%s,%s,%s,%d,%d\n",
            head->chassisNumber, head->make, head->model, head->yearOfManufacture,
            head->cost, head->currentValuation, head->currentMileage, head->nextServiceMileage,
            head->ownerName, head->ownerEmail, head->ownerPhoneNumber,
            head->machineType, head->breakdownsThisYear);
        head = head->next;
    }
    fclose(file);
}

Machinery* restoreMachineryList(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    Machinery* head = NULL;
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), file)) {
        char chassis[MAX_STR_LENGTH], make[MAX_STR_LENGTH], model[MAX_STR_LENGTH];
        int year, type, breakdowns;
        float cost, valuation, mileage, nextService;
        char name[MAX_STR_LENGTH], email[MAX_STR_LENGTH], phone[MAX_STR_LENGTH];

        sscanf(buffer, "%[^,],%[^,],%[^,],%d,%f,%f,%f,%f,%[^,],%[^,],%[^,],%d,%d",
            chassis, make, model, &year, &cost, &valuation, &mileage, &nextService,
            name, email, phone, &type, &breakdowns);

        Machinery* newMachine = createMachinery(chassis, make, model, year, cost, valuation,
            mileage, nextService, name, email, phone,
            (MachineType)type, (BreakdownExperience)breakdowns);
        addMachinery(&head, newMachine);
    }
    fclose(file);
    return head;
}