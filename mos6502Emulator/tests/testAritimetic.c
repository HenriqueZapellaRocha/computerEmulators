#include "testAritimetic.h"
#include "../CPU/cpu6502.h"
#include <stdio.h>
#include "TestFuncions.h"

void aritimeticTests(CPU *cpu, Memory *memory) {
    //ADC IMMEDIATE
    cpu->reset(cpu,memory,0);
    cpu->status.bits.Z = 0;
    cpu->ACC = 10;
    memory->Data[0XFFFC] = InsADCIM;
    memory->Data[0XFFFD] = 10;
    printf("ADC IMMEDIATE TEST");
    cpu->executeI(cpu,memory,2);
    assertEqual(cpu->ACC, 20);
    printf("\n");

    //ADC Zero Page
    cpu->reset(cpu,memory,0);
    cpu->status.bits.Z = 0;
    cpu->ACC = 10;
    memory->Data[0XFFFC] = InsADCIM;
    memory->Data[0XFFFD] = 0x2;
    memory->Data[0x2] = 10;
    printf("ADC Zero Page TEST");
    cpu->executeI(cpu,memory,2);
    assertEqual(cpu->ACC, 20);
    printf("\n");
}