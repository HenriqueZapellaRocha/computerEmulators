#include "testJmp.h"
#include "../CPU/cpu6502.h"
#include <stdio.h>
#include "TestFuncions.h"

void jmpTest(CPU *cpu, Memory *memory) {
    //JMP ABSOLUTE
    cpu->reset(cpu,memory,0);
    memory->Data[0xFFFC] = InsJMPABS;
    memory->Data[0xFFFD] = 0x80;
    memory->Data[0xFFFE] = 0x80;
    memory->Data[0x8080] = InsLDAIM;
    memory->Data[0x8081] = 10;
    printf("JMP ABSOLUTE TEST");
    cpu->executeI(cpu, memory, 5);
    assertEqual(cpu->ACC, 10);
    printf("\n");

    //JMP INDIRECT
    cpu->reset(cpu,memory,0);
    memory->Data[0xFFFC] = InsJMPIND;
    memory->Data[0xFFFD] = 0x80;
    memory->Data[0xFFFE] = 0x80;
    memory->Data[0x8080] = 0x02;
    memory->Data[0x8081] = 0x00;
    memory->Data[0x0002] = InsLDAIM;
    memory->Data[0x0003] = 10;
    printf("JMP INDIRECT TEST");
    cpu->executeI(cpu, memory, 7);
    assertEqual(cpu->ACC, 10);
    printf("\n");
}