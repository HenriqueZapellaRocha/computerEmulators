#include "testBrach.h"
#include "../CPU/cpu6502.h"
#include <stdio.h>
#include "TestFuncions.h"

void branchTests(CPU *cpu, Memory *memory) {
    //BEQ 2 cycles
    cpu->reset(cpu,memory,0XFFFA);
    cpu->status.bits.Z = 0;
    memory->Data[0XFFFA] = InsBEQ;
    memory->Data[0XFFFB] = 0x2;
    printf("BEQ 2 CYCLES TEST");
    cpu->executeI(cpu,memory,2);
    assertEqual(cpu->PC, 0xFFFC);
    printf("\n");

    //BEQ 3 cycles
    cpu->reset(cpu,memory,0XFFFA);
    cpu->status.bits.Z = 1;
    memory->Data[0XFFFA] = InsBEQ;
    memory->Data[0XFFFB] = 0x2;
    printf("BEQ 3 CYCLES TEST");
    cpu->executeI(cpu,memory,3);
    assertEqual(cpu->PC, 0xFFFC + 0x2);
    printf("\n");

    //BEQ 5 cycles
    cpu->reset(cpu,memory,0xFEFD);
    cpu->status.bits.Z = 1;
    memory->Data[0xFEFD] = InsBEQ;  // Opcode BEQ
    memory->Data[0xFEFE] = 0x1;    
    printf("BEQ 5 CYCLES TEST");
    cpu->executeI(cpu,memory,5);
    assertEqual(cpu->PC, 0xFEFF + 0x1);
    printf("\n");
}
