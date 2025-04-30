#define _CRT_SECURE_NO_WARNINGS
#include "machinery.h"

int main() {
    Login logins[MAX_USERS];
    int loginCount;
    loadLogins(logins, &loginCount);

    printf("Welcome to the Machinery Management System\n");

    if (!authenticate(logins, loginCount)) {
        printf("Too many failed login attempts. Exiting program.\n");
        return 0;
    }

    Machinery* fleet = restoreMachineryList("fleet.txt");

    int choice;
    char input[10];

    do {
        clearConsole();

        printf("\n--- Machinery Menu ---\n");
        printf("1) Add new machine\n");
        printf("2) Display all machines\n");
        printf("3) Display machine details\n");
        printf("4) Update machine details\n");
        printf("5) Delete machine\n");
        printf("0) Exit\n");

        int valid = 0;
        while (!valid) {
            printf("Enter your choice: ");
            fgets(input, sizeof(input), stdin);
            if (sscanf(input, "%d", &choice) == 1 && choice >= 0 && choice <= 5) {
                valid = 1;
            }
            else {
                printf("Invalid input. Please enter a number between 0 and 5.\n");
            }
        }

        switch (choice) {
        case 1: {
            char chassis[MAX_STR_LENGTH], make[MAX_STR_LENGTH], model[MAX_STR_LENGTH], owner[MAX_STR_LENGTH];
            int year;
            float cost, valuation, mileage;

            getTrimmedInput("Enter chassis number: ", chassis, MAX_STR_LENGTH);
            getTrimmedInput("Enter make: ", make, MAX_STR_LENGTH);
            getTrimmedInput("Enter model: ", model, MAX_STR_LENGTH);
            year = getValidatedYear("Enter year of manufacture: ");
            cost = getValidatedFloat("Enter cost: ");
            valuation = getValidatedFloat("Enter current valuation: ");
            mileage = getValidatedFloat("Enter current mileage: ");
            getTrimmedInput("Enter owner's name: ", owner, MAX_STR_LENGTH);

            Machinery* newMachine = createMachinery(chassis, make, model, year, cost, valuation, mileage, owner);
            addMachinery(&fleet, newMachine);
            printf("Machine added successfully!\n");
            break;
        }

        case 2:
            displayAllMachinery(fleet);
            break;

        case 3: {
            char chassis[MAX_STR_LENGTH];
            getTrimmedInput("Enter chassis number to view: ", chassis, MAX_STR_LENGTH);
            Machinery* machine = retrieveMachinery(fleet, chassis);
            if (machine) {
                printf("Chassis: %s | Make: %s | Model: %s | Year: %d\n",
                    machine->chassisNumber, machine->make, machine->model, machine->yearOfManufacture);
                printf("Cost: %.2f | Valuation: %.2f | Mileage: %.2f | Owner: %s\n",
                    machine->cost, machine->currentValuation, machine->currentMileage, machine->ownerName);
            }
            else {
                printf("Machine not found.\n");
            }
            break;
        }

        case 4: {
            char chassis[MAX_STR_LENGTH];
            getTrimmedInput("Enter chassis number to update: ", chassis, MAX_STR_LENGTH);
            Machinery* machine = retrieveMachinery(fleet, chassis);
            if (machine) {
                updateMachinery(machine);
            }
            else {
                printf("Machine not found.\n");
            }
            break;
        }

        case 5: {
            char chassis[MAX_STR_LENGTH];
            getTrimmedInput("Enter chassis number to delete: ", chassis, MAX_STR_LENGTH);
            deleteMachinery(&fleet, chassis);
            break;
        }

        case 0:
            saveMachineryList(fleet, "fleet.txt");
            printf("Fleet saved. Exiting program.\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
        }

        if (choice != 0) {
            printf("\nPress Enter to return to the menu...");
            getchar();
        }

    } while (choice != 0);

    return 0;
}
