#include "testStack.h"
#include "../CPU/cpu6502.h"
#include <stdio.h>
#include "TestFuncions.h"

void stackOperationsTest(CPU *cpu, Memory *memory) {

    cpu->reset(cpu,memory,0xFF00);
    memory->Data[0xFF00] = InsJSRABS;
    memory->Data[0xFF01] = 0x00;
    memory->Data[0xFF02] = 0x80;
    memory->Data[0x8000] = InsRTSIMP;
    memory->Data[0xFF03] = InsLDAIM;
    memory->Data[0xFF04] = 10;
    printf("JSR AND RTS TEST");
    cpu->executeI(cpu, memory, 14);
    assertEqual(cpu->ACC, 10);
    printf("\n");
}
