#include "testStack.h"
#include "../CPU/cpu6502.h"
#include <stdio.h>
#include "TestFuncions.h"

void stackOperationsTest(CPU *cpu, Memory *memory) {
    //JSR AND RTS
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

    //TSX
    cpu->reset(cpu,memory,0xFF00);
    memory->Data[0xFF00] = InsTSX;
    printf("TSX TEST");
    cpu->executeI(cpu, memory, 2);
    assertEqual(cpu->X, cpu->SP);
    printf("\n");

    //TXS
    cpu->reset(cpu,memory,0xFF00);
    memory->Data[0xFF00] = InsTXS;
    printf("TXS TEST");
    cpu->executeI(cpu, memory, 2);
    assertEqual(cpu->SP, cpu->X);
    printf("\n");

    //PHA
    cpu->reset(cpu,memory,0xFF00);
    cpu->ACC = 10;
    memory->Data[0xFF00] = InsPHA;
    printf("PHA TEST");
    cpu->executeI(cpu, memory, 3);
    assertEqual(memory->Data[0x1FF], cpu->ACC);
    printf("\n");

    //PHP
    cpu->reset(cpu,memory,0xFF00);
    cpu->status.bits.Z = 1;
    cpu->status.bits.O = 1;
    memory->Data[0xFF00] = InsPHP;
    printf("PHP TEST");
    cpu->executeI(cpu, memory, 3);
    assertEqual(memory->Data[0x1FF], cpu->status.byte);
    printf("\n");

    //PLA
    cpu->reset(cpu,memory,0xFF00);
    memory->Data[0x1FF] = 10;
    cpu->SP -=1;
    memory->Data[0xFF00] = InsPLA;
    printf("PLA TEST");
    cpu->executeI(cpu, memory, 4);
    assertEqual(cpu->ACC, 10);
    printf("\n");

    //PLP
    cpu->reset(cpu,memory,0xFF00);
    cpu->status.bits.Z = 1;
    cpu->status.bits.O = 1;
    Byte testStatus = cpu->status.byte;
    memory->Data[0x1FF] = cpu->status.byte;
    cpu->status.bits.Z = 0;
    cpu->status.bits.O = 0;
    cpu->SP -=1;
    memory->Data[0xFF00] = InsPLP;
    printf("PLP TEST");
    cpu->executeI(cpu, memory, 4);
    assertEqual(cpu->status.byte, testStatus);
    printf("\n");

}
