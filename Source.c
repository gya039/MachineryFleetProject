#define _CRT_SECURE_NO_WARNINGS
#include "machinery.h"

int main() {
    Machinery* fleet = NULL;

    Machinery* m1 = createMachinery("CH1234", "Caterpillar", "ModelX", 2015, 50000.0);
    addMachinery(&fleet, m1);

    displayAllMachinery(fleet);

    return 0;
}
