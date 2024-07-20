#include "testStore.h"
#include <stdio.h>
#include "TestFuncions.h"
#include "cpu6502.h"

void StoreAInstructionsTest(CPU *cpu, Memory *memory) {
    //STA Zero page
    cpu->ACC = 10;
    memory->Data[0xFFFC] = InsSTAZP; //opdocde
    memory->Data[0xFFFD] = 0x2;
    memory->Data[0x2] = 0;
    printf("STA Zero Page TEST");
    cpu->executeI(cpu,memory,3);
    assertEqual(memory->Data[0x2],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //STA Zero page,X
    cpu->ACC = 10;
    cpu->X = 0x10;
    memory->Data[0xFFFC] = InsSTAZPX; //opdocde
    memory->Data[0xFFFD] = 0x2;
    memory->Data[0x02 + 0x10] = 0;
    printf("STA Zero Page,X TEST");
    cpu->executeI(cpu,memory,4);
    assertEqual(memory->Data[0x02 + 0x10],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //STA Absolute
    cpu->ACC = 10;
    memory->Data[0xFFFC] = InsSTAABS; //opdocde
    memory->Data[0xFFFD] = 0x42; //adress
    memory->Data[0xFFFE] = 0x42; //adress
    memory->Data[0x4242] = 0; //value
    printf("STA Absolute TEST");
    cpu->executeI(cpu, memory, 4);
    assertEqual(memory->Data[0x4242],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //STA Absolute,X
    cpu->ACC = 10;
    cpu->X = 0x40;
    memory->Data[0xFFFC] = InsSTAABSX; //opdocde
    memory->Data[0xFFFD] = 0xFF; //adress
    memory->Data[0x40 + 0XFF] = 0; //value
    printf("STA Absolute,X TEST");
    cpu->executeI(cpu, memory, 5);
    assertEqual(memory->Data[0x40 + 0XFF],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //STA Absolute,Y
    cpu->ACC = 10;
    cpu->Y = 0x40;
    memory->Data[0xFFFC] = InsSTAABSY; //opdocde
    memory->Data[0xFFFD] = 0xFF; //adress
    memory->Data[0x40 + 0xFF] = 0; //value
    printf("STA Absolute,Y TEST");
    cpu->executeI(cpu, memory, 5);
    assertEqual(memory->Data[0x40+0xFF],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //STA Indirect,X
    cpu->ACC = 10;
    cpu->X = 0x0F;
    memory->Data[0xFFFC] = InsSTAINDX; //opdocde
    memory->Data[0xFFFD] = 0x20; //adress
    memory->Data[0x002F] = 0x0; //adress
    memory->Data[0x0030] = 0x80;
    memory->Data[0x8000] = 0; //value
    cpu->executeI(cpu, memory, 6);
    printf("STA Indirect,X TEST");
    assertEqual(memory->Data[0x8000],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //STA Indirect,Y
    cpu->ACC = 10;
    cpu->Y = 0x0F;
    memory->Data[0xFFFC] = InsSTAINDY; //opdocde
    memory->Data[0xFFFD] = 0x20; //adress
    memory->Data[0x0020] = 0x0; //adress
    memory->Data[0x0021] = 0x80;
    memory->Data[0x8000 + 0x0F] = 0; //value
    cpu->executeI(cpu, memory, 6);
    printf("STA Indirect,Y TEST");
    assertEqual(memory->Data[0x8000 + 0xF],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

}

void StoreXInstructionsTest(CPU *cpu, Memory *memory) {
    //STX Zero page
    cpu->X = 10;
    memory->Data[0xFFFC] = InsSTXZP; //opdocde
    memory->Data[0xFFFD] = 0x2;
    memory->Data[0x2] = 0;
    printf("STX Zero Page TEST");
    cpu->executeI(cpu,memory,3);
    assertEqual(memory->Data[0x2],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //STX Zero page,Y
    cpu->Y = 0X10;
    cpu->X = 10;
    memory->Data[0xFFFC] = InsSTXZPY; //opdocde
    memory->Data[0xFFFD] = 0x2;
    memory->Data[0x02 + 0x10] = 0;
    printf("STX Zero Page,Y TEST");
    cpu->executeI(cpu,memory,4);
    assertEqual(memory->Data[0x02 + 0x10],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //STX Absolute
    cpu->X = 10;
    memory->Data[0xFFFC] = InsSTXABS; //opdocde
    memory->Data[0xFFFD] = 0x42; //adress
    memory->Data[0xFFFE] = 0x42; //adress
    memory->Data[0x4242] = 0; //value
    printf("STX Absolute TEST");
    cpu->executeI(cpu, memory, 4);
    assertEqual(memory->Data[0x4242],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

}

void StoreYInstructionsTest(CPU *cpu, Memory *memory) {
    //STY Zero page
    cpu->Y = 10;
    memory->Data[0xFFFC] = InsSTYZP; //opdocde
    memory->Data[0xFFFD] = 0x2;
    memory->Data[0x2] = 0;
    printf("STY Zero Page TEST");
    cpu->executeI(cpu,memory,3);
    assertEqual(memory->Data[0x2],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //STY Zero page,X
    cpu->X = 0X10;
    cpu->Y = 10;
    memory->Data[0xFFFC] = InsSTYZPX; //opdocde
    memory->Data[0xFFFD] = 0x2;
    memory->Data[0x02 + 0x10] = 0;
    printf("STY Zero Page,X TEST");
    cpu->executeI(cpu,memory,4);
    assertEqual(memory->Data[0x02 + 0x10],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //STY Absolute
    cpu->Y = 10;
    memory->Data[0xFFFC] = InsSTYABS; //opdocde
    memory->Data[0xFFFD] = 0x42; //adress
    memory->Data[0xFFFE] = 0x42; //adress
    memory->Data[0x4242] = 0; //value
    printf("STY Absolute TEST");
    cpu->executeI(cpu, memory, 4);
    assertEqual(memory->Data[0x4242],10);
    printf("\n");
    cpu->reset(cpu,memory,0);

}
