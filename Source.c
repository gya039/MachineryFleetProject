#define _CRT_SECURE_NO_WARNINGS
#include "machinery.h"

int main() {
    Machinery* fleet = restoreMachineryList("fleet.txt");

    char chassis[MAX_STR_LENGTH];
    char make[MAX_STR_LENGTH];
    char model[MAX_STR_LENGTH];
    int year;
    float cost, valuation, mileage;
    char owner[MAX_STR_LENGTH];

    getTrimmedInput("Enter chassis number: ", chassis, MAX_STR_LENGTH);
    getTrimmedInput("Enter make: ", make, MAX_STR_LENGTH);
    getTrimmedInput("Enter model: ", model, MAX_STR_LENGTH);
    year = getValidatedYear("Enter year of manufacture: ");
    cost = getValidatedFloat("Enter original cost: ");
    valuation = getValidatedFloat("Enter current valuation: ");
    mileage = getValidatedFloat("Enter current mileage: ");
    getTrimmedInput("Enter owner's name: ", owner, MAX_STR_LENGTH);

    Machinery* newMachine = createMachinery(chassis, make, model, year, cost, valuation, mileage, owner);
    addMachinery(&fleet, newMachine);

    displayAllMachinery(fleet);
    saveMachineryList(fleet, "fleet.txt");

    return 0;
}



