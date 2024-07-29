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

    //testing just the 3 cycles type beacause the others tested before in beq
    //BCC 
    cpu->reset(cpu,memory,0XFFFA);
    cpu->status.bits.C = 0;
    memory->Data[0XFFFA] = InsBCC;
    memory->Data[0XFFFB] = 0x2;
    printf("BCC TEST");
    cpu->executeI(cpu,memory,3);
    assertEqual(cpu->PC, 0xFFFC + 0x2);
    printf("\n");


    //BCS
    cpu->reset(cpu,memory,0XFFFA);
    cpu->status.bits.C = 1;
    memory->Data[0XFFFA] = InsBCS;
    memory->Data[0XFFFB] = 0x2;
    printf("BCS TEST");
    cpu->executeI(cpu,memory,3);
    assertEqual(cpu->PC, 0xFFFC + 0x2);
    printf("\n");

    //BMI
    cpu->reset(cpu,memory,0XFFFA);
    cpu->status.bits.N = 1;
    memory->Data[0XFFFA] = InsBMI;
    memory->Data[0XFFFB] = 0x2;
    printf("BMI TEST");
    cpu->executeI(cpu,memory,3);
    assertEqual(cpu->PC, 0xFFFC + 0x2);
    printf("\n");

    //BNE
    cpu->reset(cpu,memory,0XFFFA);
    cpu->status.bits.Z = 0;
    memory->Data[0XFFFA] = InsBNE;
    memory->Data[0XFFFB] = 0x2;
    printf("BNE TEST");
    cpu->executeI(cpu,memory,3);
    assertEqual(cpu->PC, 0xFFFC + 0x2);
    printf("\n");

    //BVC
    cpu->reset(cpu,memory,0XFFFA);
    cpu->status.bits.V = 0;
    memory->Data[0XFFFA] = InsBVC;
    memory->Data[0XFFFB] = 0x2;
    printf("BVC TEST");
    cpu->executeI(cpu,memory,3);
    assertEqual(cpu->PC, 0xFFFC + 0x2);
    printf("\n");

    //BPL
    cpu->reset(cpu,memory,0XFFFA);
    cpu->status.bits.N = 0;
    memory->Data[0XFFFA] = InsBPL;
    memory->Data[0XFFFB] = 0x2;
    printf("BPL TEST");
    cpu->executeI(cpu,memory,3);
    assertEqual(cpu->PC, 0xFFFC + 0x2);
    printf("\n");
}
