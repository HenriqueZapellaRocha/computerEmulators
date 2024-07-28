#include "testIncDc.h"
#include "../CPU/cpu6502.h"
#include <stdio.h>
#include "TestFuncions.h"


void incrementTest(CPU *cpu, Memory *memory) {
    //INC zero page
    cpu->reset(cpu,memory,0);
    memory->Data[0xFFFC] = InsINCZP;
    memory->Data[0xFFFD] = 0x02;
    printf("INC zero page TEST");
    cpu->executeI(cpu, memory, 5);
    assertEqual(memory->Data[0x02], 1);
    printf("\n");

    //INC zero page,X
    cpu->reset(cpu,memory,0);
    cpu->X = 1;
    memory->Data[0xFFFC] = InsINCZPX;
    memory->Data[0xFFFD] = 0x02;
    printf("INC zero page,X TEST");
    cpu->executeI(cpu, memory, 6);
    assertEqual(memory->Data[0x03], 1);
    printf("\n");

    //INC Absolute
    cpu->reset(cpu,memory,0);
    memory->Data[0xFFFC] = InsINCABS;
    memory->Data[0xFFFD] = 0x02;
    memory->Data[0XFFFE] = 0x80;
    memory->Data[0X8002] = 2;
    printf("INC Absolute TEST");
    cpu->executeI(cpu, memory, 6);
    assertEqual(memory->Data[0X8002], 3);
    printf("\n");

    //INC Absolute,X
    cpu->reset(cpu,memory,0);
    cpu->X = 1;
    memory->Data[0xFFFC] = InsINCABSX;
    memory->Data[0xFFFD] = 0x02;
    memory->Data[0XFFFE] = 0x80;
    memory->Data[0X8002+1] = 2;
    printf("INC Absolute,X TEST");
    cpu->executeI(cpu, memory, 7);
    assertEqual(memory->Data[0X8002+1], 3);
    printf("\n");

    //INX 
    cpu->reset(cpu,memory,0);
    cpu->X = 0;
    memory->Data[0xFFFC] = InsINX;
    printf("INX TEST");
    cpu->executeI(cpu, memory, 2);
    assertEqual(cpu->X, 1);
    printf("\n");

    //INY
    cpu->reset(cpu,memory,0);
    cpu->Y = 0;
    memory->Data[0xFFFC] = InsINY;
    printf("INY TEST");
    cpu->executeI(cpu, memory, 2);
    assertEqual(cpu->Y, 1);
    printf("\n");
}

void dencrementTest(CPU *cpu, Memory *memory) {
    //DEC zero page
    cpu->reset(cpu,memory,0);
    memory->Data[0xFFFC] = InsDECZP;
    memory->Data[0xFFFD] = 0x02;
    memory->Data[0x02] = 2;
    printf("DEC zero page TEST");
    cpu->executeI(cpu, memory, 5);
    assertEqual(memory->Data[0x02], 1);
    printf("\n");

    //DEC zero page,X
    cpu->reset(cpu,memory,0);
    cpu->X = 1;
    memory->Data[0xFFFC] = InsDECZPX;
    memory->Data[0xFFFD] = 0x02;
    memory->Data[0x03] = 2;
    printf("DEC zero page,X TEST");
    cpu->executeI(cpu, memory, 6);
    assertEqual(memory->Data[0x03], 1);
    printf("\n");

    //DEC Absolute
    cpu->reset(cpu,memory,0);
    memory->Data[0xFFFC] = InsDECABS;
    memory->Data[0xFFFD] = 0x02;
    memory->Data[0XFFFE] = 0x80;
    memory->Data[0X8002] = 2;
    printf("DEC Absolute TEST");
    cpu->executeI(cpu, memory, 6);
    assertEqual(memory->Data[0X8002], 1);
    printf("\n");

    //DEC Absolute,X
    cpu->reset(cpu,memory,0);
    cpu->X = 1;
    memory->Data[0xFFFC] = InsDECABSX;
    memory->Data[0xFFFD] = 0x02;
    memory->Data[0XFFFE] = 0x80;
    memory->Data[0X8002+1] = 2;
    printf("DEC Absolute,X TEST");
    cpu->executeI(cpu, memory, 7);
    assertEqual(memory->Data[0X8002+1], 1);
    printf("\n");

    //DEX 
    cpu->reset(cpu,memory,0);
    cpu->X = 2;
    memory->Data[0xFFFC] = InsDEX;
    printf("DEX TEST");
    cpu->executeI(cpu, memory, 2);
    assertEqual(cpu->X, 1);
    printf("\n");

    //DEY
    cpu->reset(cpu,memory,0);
    cpu->Y = 2;
    memory->Data[0xFFFC] = InsDEY;
    printf("DEY TEST");
    cpu->executeI(cpu, memory, 2);
    assertEqual(cpu->Y, 1);
    printf("\n");
}