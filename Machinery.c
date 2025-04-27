#include "machinery.h"

Machinery* createMachinery(char* chassis, char* make, char* model, int year, float cost) {
    Machinery* newMachinery = (Machinery*)malloc(sizeof(Machinery));
    strcpy(newMachinery->chassisNumber, chassis);
    strcpy(newMachinery->make, make);
    strcpy(newMachinery->model, model);
    newMachinery->yearOfManufacture = year;
    newMachinery->cost = cost;
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
        printf("Chassis: %s | Make: %s | Model: %s | Year: %d | Cost: %.2f\n",
            temp->chassisNumber, temp->make, temp->model, temp->yearOfManufacture, temp->cost);
        temp = temp->next;
    }
}
