#include "machinery.h"

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

void displayAllMachinery(Machinery* head) {
    Machinery* temp = head;
    while (temp != NULL) {
        printf("Chassis: %s | Make: %s | Model: %s | Year: %d\n",
            temp->chassisNumber, temp->make, temp->model, temp->yearOfManufacture);
        printf("Cost: %.2f | Valuation: %.2f | Mileage: %.2f | Owner: %s\n\n",
            temp->cost, temp->currentValuation, temp->currentMileage, temp->ownerName);
        temp = temp->next;
    }
}

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
