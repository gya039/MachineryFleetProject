#define _CRT_SECURE_NO_WARNINGS
#ifndef MACHINERY_H
#define MACHINERY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USERNAME_LEN 7
#define PASSWORD_LEN 7
#define MAX_USERS 3
#define MAX_STR_LENGTH 100

typedef struct {
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
} Login;

int authenticate(Login logins[], int count);
void loadLogins(Login logins[], int* count);
void getMaskedPassword(char* password);


typedef struct Machinery {
    char chassisNumber[MAX_STR_LENGTH];
    char make[MAX_STR_LENGTH];
    char model[MAX_STR_LENGTH];
    int yearOfManufacture;
    float cost;
    float currentValuation;
    float currentMileage;
    char ownerName[MAX_STR_LENGTH];
    struct Machinery* next;
} Machinery;

// Functions
void clearConsole();
Machinery* createMachinery(char*, char*, char*, int, float, float, float, char*);
void addMachinery(Machinery**, Machinery*);
void displayAllMachinery(Machinery*);
void saveMachineryList(Machinery* head, const char* filename);
Machinery* restoreMachineryList(const char* filename);
float getValidatedFloat(const char* prompt);
int getValidatedYear(const char* prompt);
void getTrimmedInput(const char* prompt, char* buffer, int size);
Machinery* retrieveMachinery(Machinery* head, const char* chassis);
void updateMachinery(Machinery* machine);
void deleteMachinery(Machinery** head, const char* chassis);


#endif
