#ifndef MACHINERY_H
#define MACHINERY_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h> // Used this for getch password input (Exclusive for windows hope thats fine)

#define MAX_USERS 3
#define USERNAME_LEN 7
#define PASSWORD_LEN 7
#define MAX_STR_LENGTH 100

//Enums
typedef enum { TRACTOR, EXCAVATOR, ROLLER, CRANE, MIXER } MachineType;
typedef enum { NEVER, LESS_THAN_THREE, LESS_THAN_FIVE, MORE_THAN_FIVE } BreakdownExperience;

extern const char* machineTypeStrings[];
extern const char* breakdownExperienceStrings[];

//Struct Definitions
typedef struct {
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
} Login;

typedef struct Machinery {
    char chassisNumber[MAX_STR_LENGTH];
    char make[MAX_STR_LENGTH];
    char model[MAX_STR_LENGTH];
    int yearOfManufacture;
    float cost;
    float currentValuation;
    float currentMileage;
    float nextServiceMileage;
    char ownerName[MAX_STR_LENGTH];
    char ownerEmail[MAX_STR_LENGTH];
    char ownerPhoneNumber[MAX_STR_LENGTH];
    MachineType machineType;
    BreakdownExperience breakdownsThisYear;
    struct Machinery* next;
} Machinery;

// Machinery Operations
Machinery* createMachinery(char*, char*, char*, int, float, float, float, float, char*, char*, char*, MachineType, BreakdownExperience);
void addMachinery(Machinery** head, Machinery* newMachine);
Machinery* retrieveMachinery(Machinery* head, const char* chassisNumber);
void deleteMachinery(Machinery** head, const char* chassisNumber);
void displayAllMachinery(Machinery* head);
void displayMachineDetails(Machinery* machine);

// File Functions
void saveMachineryList(Machinery* head, const char* filename);
Machinery* restoreMachineryList(const char* filename);
void printReportToFile(Machinery* head, const char* filename);

// Valuation & Statistics
Machinery* sortByValuation(Machinery* head);
void generateStatistics(Machinery* head);

// Login stuff
void loadLogins(Login logins[], int* count);
int authenticate(Login logins[], int count);
void getMaskedPassword(char* password);

// Input & validatinoas.
void validateStringInput(char* input, int maxLength);
void getFullNameInput(const char* prompt, char* buffer, int size);
void getNonEmptyInput(const char* prompt, char* buffer, int size);
int validateNonBlankInput(char* input);
int validatePhoneNumber(const char* phone);
int validateEmail(char* email);
int validateYear(int* year);
int validateFloatInput(float* input);
void validateMileage(float* mileage, float* nextService);

// UI for better reading/flow
void clearConsole();
void displayChassisNumbers(Machinery* head);
int chassisExists(Machinery* head, const char* chassisNumber);

#endif
