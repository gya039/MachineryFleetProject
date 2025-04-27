#define _CRT_SECURE_NO_WARNINGS
#ifndef MACHINERY_H
#define MACHINERY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LENGTH 100

typedef struct Machinery {
    char chassisNumber[MAX_STR_LENGTH];
    char make[MAX_STR_LENGTH];
    char model[MAX_STR_LENGTH];
    int yearOfManufacture;
    float cost;
    struct Machinery* next;
} Machinery;

// Function prototypes
Machinery* createMachinery(char*, char*, char*, int, float);
void addMachinery(Machinery**, Machinery*);
void displayAllMachinery(Machinery*);

#endif
