#include "testTranfers.h"
#include <stdio.h>
#include "TestFuncions.h"
#include "../CPU/cpu6502.h"

void testTranfersRegisters(CPU *cpu, Memory *memory) {

    //TAX
    cpu->reset(cpu,memory,0);
    cpu->ACC = 10;
    memory->Data[0xFFFC] = InsTAX; //opdocde
    printf("TAX TEST");
    cpu->executeI(cpu,memory,2);
    assertEqual(cpu->X,10);
    printf("\n");

    //TAY
    cpu->reset(cpu,memory,0);
    cpu->ACC = 10;
    memory->Data[0xFFFC] = InsTAY; //opdocde
    printf("TAY TEST");
    cpu->executeI(cpu,memory,2);
    assertEqual(cpu->Y,10);
    printf("\n");

    //TXA
    cpu->reset(cpu,memory,0);
    cpu->X = 10;
    memory->Data[0xFFFC] = InsTXA; //opdocde
    printf("TXA TEST");
    cpu->executeI(cpu,memory,2);
    assertEqual(cpu->ACC,10);
    printf("\n");


    //TYA
    cpu->reset(cpu,memory,0);
    cpu->Y = 10;
    memory->Data[0xFFFC] = InsTYA; //opdocde
    printf("TYA TEST");
    cpu->executeI(cpu,memory,2);
    assertEqual(cpu->ACC,10);
    printf("\n");
}