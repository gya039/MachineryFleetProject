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
