#include "machinery.h"

int main() {
    Login logins[MAX_USERS];
    int loginCount;

    loadLogins(logins, &loginCount);

    printf("Welcome to the Machinery Fleet Management System\n");
    if (!authenticate(logins, loginCount)) {
        printf("Too many failed login attempts. Exiting program.\n");
        return 1;
    }

    Machinery* fleet = restoreMachineryList("fleet.txt");
    int choice;
    char chassisSearch[MAX_STR_LENGTH];

    do {
        clearConsole();  // Clear the screen before showing the menu

        printf("\n--- Machinery Fleet Menu ---\n");
        printf("1) Add machine\n2) Display all machines\n3) Display machine details\n4) Update machine\n5) Delete machine\n");
        printf("6) Generate breakdown statistics\n7) Print report to file\n8) List machines by valuation\n0) Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // flush newline

        switch (choice) {
        case 1: {
            char chassis[MAX_STR_LENGTH], make[MAX_STR_LENGTH], model[MAX_STR_LENGTH];
            int year;
            float cost, valuation, mileage, nextService;
            char name[MAX_STR_LENGTH], email[MAX_STR_LENGTH], phone[MAX_STR_LENGTH];
            int type, breakdowns;

            getNonEmptyInput("Enter chassis number: ", chassis, MAX_STR_LENGTH);

            if (chassisExists(fleet, chassis)) {
                printf("Error: Chassis number '%s' already exists. Machine not added.\n", chassis);
                break;
            }

            getNonEmptyInput("Enter make: ", make, MAX_STR_LENGTH);
            getNonEmptyInput("Enter model: ", model, MAX_STR_LENGTH);
            printf("Enter year of manufacture: ");
            validateYear(&year);
            printf("Enter cost: ");
            validateFloatInput(&cost);
            printf("Enter valuation: ");
            validateFloatInput(&valuation);
            printf("Enter current mileage: ");
            validateFloatInput(&mileage);
            printf("Enter next service mileage: ");
            validateFloatInput(&nextService);
            validateMileage(&mileage, &nextService);
            getFullNameInput("Enter owner's name: ", name, MAX_STR_LENGTH);
            printf("Enter owner's email: "); gets(email);
            while (!validateEmail(email)) {
                printf("Invalid email. Please enter a valid email: ");
                gets(email);
            }
            do {
                printf("Enter owner's phone number (format: 083-155-4646): ");
                fgets(phone, MAX_STR_LENGTH, stdin);
                phone[strcspn(phone, "\n")] = '\0'; // remove newline

                if (!validatePhoneNumber(phone)) {
                    printf("Invalid phone number format. Please enter in 3-3-4 format (e.g., 083-155-4646).\n");
                }
            } while (!validatePhoneNumber(phone));

            do {
                printf("Enter machine type (0=Tractor, 1=Excavator, 2=Roller, 3=Crane, 4=Mixer): ");
                scanf("%d", &type);
                if (type < 0 || type > 4) {
                    printf("Invalid machine type. Please enter a value between 0 and 4.\n");
                }
            } while (type < 0 || type > 4);

            // Validate breakdown level input
            do {
                printf("Enter breakdown level this year (0=Never, 1=Less than three times, 2=Less than five times, 3=More than five times): ");
                scanf("%d", &breakdowns);
                if (breakdowns < 0 || breakdowns > 3) {
                    printf("Invalid breakdown level. Please enter a value between 0 and 3.\n");
                }
            } while (breakdowns < 0 || breakdowns > 3);

            Machinery* newMachine = createMachinery(chassis, make, model, year, cost, valuation,
                mileage, nextService, name, email, phone, (MachineType)type, (BreakdownExperience)breakdowns);

            addMachinery(&fleet, newMachine);
            saveMachineryList(fleet, "fleet.txt");
            break;
        }
        case 2:
            displayAllMachinery(fleet);
            break;
        case 3:
            // Shows all chassis numbers for the user so they dont habe to remeber them
            displayChassisNumbers(fleet);
            printf("Enter chassis number to retrieve: ");
            scanf("%s", chassisSearch);
            Machinery* machine = retrieveMachinery(fleet, chassisSearch);
            if (machine != NULL) {
                displayMachineDetails(machine);  // Display details of the selected machine
            }
            else {
                printf("Machine with chassis number %s not found.\n", chassisSearch);  // where the machine is not found
            }
            break;

        case 4:
        {
            // Shows all chassis numbers for the user so they dont habe to remeber them
            printf("\nAvailable Chassis Numbers:\n");
            displayChassisNumbers(fleet);

            printf("Enter chassis number to update: ");
            scanf("%s", chassisSearch);

            // Retrieve the machine using the chassis number
            Machinery* machine = retrieveMachinery(fleet, chassisSearch);
            if (machine != NULL) {
                // Display details of the selected machine
                displayMachineDetails(machine);

                int updateChoice;
                do {
                    printf("\nSelect the detail to update:\n");
                    printf("1) Update Mileage\n");
                    printf("2) Update Next Service Mileage\n");
                    printf("3) Update Owner Details\n");  // Combined option
                    printf("4) Update Valuation\n");
                    printf("0) Back to Menu\n");
                    printf("Enter your choice: ");
                    scanf("%d", &updateChoice);
                    getchar();

                    switch (updateChoice) {
                    case 1:
                        // Update Mileage
                        printf("Current Mileage: %.2f\n", machine->currentMileage);
                        printf("Enter new mileage: ");
                        validateFloatInput(&machine->currentMileage);  //validates input
                        printf("Mileage updated successfully!\n");
                        break;

                    case 2:
                        // Update Next Service Mileage
                        printf("Current Next Service Mileage: %.2f\n", machine->nextServiceMileage);
                        printf("Enter new next service mileage: ");
                        validateFloatInput(&machine->nextServiceMileage);  //validates input
                        printf("Next Service Mileage updated successfully!\n");
                        break;
                    case 3:
                        // Update Owner Details (Full Name, Phone, Email)
                        printf("Current Owner Name: %s\n", machine->ownerName);
                        printf("Enter new Owner Name: ");
                        validateStringInput(machine->ownerName, MAX_STR_LENGTH); // string input validation

                        printf("Current Owner Phone: %s\n", machine->ownerPhoneNumber);
                        printf("Enter new Owner Phone: ");
                        validateStringInput(machine->ownerPhoneNumber, MAX_STR_LENGTH); // phone number input validation

                        printf("Current Owner Email: %s\n", machine->ownerEmail);
                        printf("Enter new Owner Email: ");
                        validateStringInput(machine->ownerEmail, MAX_STR_LENGTH); // email input validation

                        printf("Owner details updated successfully!\n");
                        break;

                    case 4:
                        // Update Valuation
                        printf("Current Valuation: %.2f\n", machine->currentValuation);
                        printf("Enter new valuation: ");
                        validateFloatInput(&machine->currentValuation); //input validation
                        printf("Valuation updated successfully!\n");
                        break;

                    case 0:
                        break;  // Exit back to the menu

                    default:
                        printf("Invalid choice! Please try again.\n");
                        break;
                    }
                } while (updateChoice != 0);
            }
            else {
                printf("Machine with chassis number %s not found.\n", chassisSearch);  // case where the machine is not found
            }
            break;
        }

        case 5:
            // Shows all chassis numbers for the user so they dont habe to remeber them
            displayChassisNumbers(fleet);

            printf("Enter chassis number to delete: ");
            scanf("%s", chassisSearch);
            machine = retrieveMachinery(fleet, chassisSearch);
            if (machine != NULL) {
                deleteMachinery(&fleet, chassisSearch);
                printf("Machine with chassis number %s has been deleted.\n", chassisSearch);
                saveMachineryList(fleet, "fleet.txt");  // Save the updated list to file
            }
            else {
                printf("Machine with chassis number %s not found.\n", chassisSearch);
            }
            break;
        case 6:
            generateStatistics(fleet);
            break;
        case 7:
            printReportToFile(fleet, "machinery_report.txt");
            break;
        case 8:
            fleet = sortByValuation(fleet);
            displayAllMachinery(fleet);
            break;
        case 0:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice, please try again.\n");
            break;
        }
        printf("\nPress Enter to return to the menu...");
        getchar(); // Wait for enter

    } while (choice != 0);

    saveMachineryList(fleet, "fleet.txt");
    return 0;
}