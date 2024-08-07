#include "testLoad.h"
#include <stdio.h>
#include "TestFuncions.h"


void LoadAInstructionsTest(CPU *cpu, Memory *memory) {
    //LDA Immediate
    memory->Data[0xFFFC] = InsLDAIM; //opdocde
    memory->Data[0xFFFD] = 10; // value
    cpu->executeI(cpu, memory, 2);
    printf("LDA IMEDIATE TEST");
    assertEqual(cpu->ACC,10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDA Zero Page
    memory->Data[0xFFFC] = InsLDAZP; //opdocde
    memory->Data[0xFFFD] = 0x42; // adress
    memory->Data[0x42] = 20; //value
    cpu->executeI(cpu, memory, 3);
    printf("LDA Zero Page TEST");
    assertEqual(cpu->ACC,20);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDA Zero Page,X
    memory->Data[0xFFFC] = InsLDAZPX; //opdocde
    memory->Data[0xFFFD] = 0x44; // adress
    memory->Data[0x46] = 20; //value
    cpu->X = 0x2;
    cpu->executeI(cpu, memory, 4);
    printf("LDA Zero Page,X");
    assertEqual(cpu->ACC,20);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDA Absolute
    memory->Data[0xFFFC] = InsLDAABS; //opdocde
    memory->Data[0xFFFD] = 0x42; //adress
    memory->Data[0xFFFE] = 0x42; //adress
    memory->Data[0x4242] = 10; //value
    cpu->executeI(cpu, memory, 4);
    printf("LDA Absolute TEST");
    assertEqual(cpu->ACC,10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDA Absolute,X
    cpu->X = 1;
    memory->Data[0xFFFC] = InsLDAABSX; //opdocde
    memory->Data[0xFFFD] = 0x80; //adress
    memory->Data[0xFFFE] = 0x40; //adress
    memory->Data[0x4081] = 10; //value
    cpu->executeI(cpu, memory, 4);
    printf("LDA Absolute X TEST");
    assertEqual(cpu->ACC,10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDA Absolute,X (page boundry)
    cpu->X = 0xFF;
    memory->Data[0xFFFC] = InsLDAABSX; //opdocde
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0xFFFE] = 0x44; //adress
    memory->Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu->executeI(cpu, memory, 5);
    printf("LDA Absolute X (with page boundry) TEST");
    assertEqual(cpu->ACC,0x37);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDA Absolute,y
    cpu->Y = 1;
    memory->Data[0xFFFC] = InsLDAABSY; //opdocde
    memory->Data[0xFFFD] = 0x80; //adress
    memory->Data[0xFFFE] = 0x40; //adress
    memory->Data[0x4081] = 10; //value
    cpu->executeI(cpu, memory, 4);
    printf("LDA Absolute Y TEST");
    assertEqual(cpu->ACC,10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDA Absolute,y (page boundry)
    cpu->Y = 0xFF;
    memory->Data[0xFFFC] = InsLDAABSY; //opdocde
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0xFFFE] = 0x44; //adress
    memory->Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu->executeI(cpu, memory, 5);
    printf("LDA Absolute Y (with page boundry) TEST");
    assertEqual(cpu->ACC,0x37);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDA Inderect,X 
    cpu->X = 0x04;
    memory->Data[0xFFFC] = InsLDAINDX; //opdocde
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0x6] = 0x0; //adress
    memory->Data[0x7] = 0x80; //adress
    memory->Data[0x8000] = 10; //value
    cpu->executeI(cpu, memory, 6);
    printf("LDA Inderect,X TEST");
    assertEqual(cpu->ACC,10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDA Inderect,Y
    cpu->Y = 0x4;
    memory->Data[0xFFFC] = InsLDAINDY; //opdocde
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0x2] = 0x0; //adress
    memory->Data[0x3] = 0x80; //adress
    memory->Data[0x8004] = 10; //value cpu->y + 800
    cpu->executeI(cpu, memory, 5);
    printf("LDA Inderect,Y TEST");
    assertEqual(cpu->ACC,10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDA Inderect,Y (Page boundry)
    cpu->Y = 0xFF;
    memory->Data[0xFFFC] = InsLDAINDY; //opdocde
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0x2] = 0x02; //adress
    memory->Data[0x3] = 0x80; //adress
    memory->Data[0x8101] = 10; //value cpu->y + 800
    cpu->executeI(cpu, memory, 6);
    printf("LDA Inderect,Y (Page Boundry) TEST");
    assertEqual(cpu->ACC,10);
    printf("\n");
    cpu->reset(cpu,memory,0);
}

void LoadXInstructionsTest(CPU *cpu, Memory *memory) {

    //LDX Immediate
    memory->Data[0xFFFC] = InsLDXIM; //opdocde
    memory->Data[0xFFFD] = 10; // value
    cpu->executeI(cpu, memory, 2);
    printf("LDX IMEDIATE TEST");
    assertEqual(cpu->X,10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDX Zero Page
    memory->Data[0xFFFC] = InsLDXZP; //opdocde
    memory->Data[0xFFFD] = 0x42; // adress
    memory->Data[0x42] = 20; //value
    cpu->executeI(cpu, memory, 3);
    printf("LDX Zero Page TEST");
    assertEqual(cpu->X,20);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDX Zero Page,Y
    memory->Data[0xFFFC] = InsLDXZPY; //opdocde
    memory->Data[0xFFFD] = 0x44; // adress
    memory->Data[0x46] = 20; //value
    cpu->Y = 0x2;
    cpu->executeI(cpu, memory, 4);
    printf("LDX Zero Page,Y");
    assertEqual(cpu->X,20);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDX Absolute
    memory->Data[0xFFFC] = InsLDXABS; //opdocde
    memory->Data[0xFFFD] = 0x42; //adress
    memory->Data[0xFFFE] = 0x42; //adress
    memory->Data[0x4242] = 10; //value
    cpu->executeI(cpu, memory, 4);
    printf("LDX Absolute TEST");
    assertEqual(cpu->X,10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDX Absolute,y
    cpu->Y = 1;
    memory->Data[0xFFFC] = InsLDXABSY; //opdocde
    memory->Data[0xFFFD] = 0x80; //adress
    memory->Data[0xFFFE] = 0x40; //adress
    memory->Data[0x4081] = 10; //value
    cpu->executeI(cpu, memory, 4);
    printf("LDX Absolute Y TEST");
    assertEqual(cpu->X,10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDX Absolute,y (page boundry)
    cpu->Y = 0xFF;
    memory->Data[0xFFFC] = InsLDXABSY; //opdocde
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0xFFFE] = 0x44; //adress
    memory->Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu->executeI(cpu, memory, 5);
    printf("LDX Absolute Y (with page boundry) TEST");
    assertEqual(cpu->X,0x37);
    printf("\n");
    cpu->reset(cpu,memory,0);

}

void LoadYInstructionsTest(CPU *cpu, Memory *memory) {

    //LDY Immediate
    memory->Data[0xFFFC] = InsLDYIM; //opdocde
    memory->Data[0xFFFD] = 10; // value
    cpu->executeI(cpu, memory, 2);
    printf("LDY IMEDIATE TEST");
    assertEqual(cpu->Y,10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDY Zero Page
    memory->Data[0xFFFC] = InsLDYZP; //opdocde
    memory->Data[0xFFFD] = 0x42; // adress
    memory->Data[0x42] = 20; //value
    cpu->executeI(cpu, memory, 3);
    printf("LDY Zero Page TEST");
    assertEqual(cpu->Y,20);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDY Zero Page,X
    memory->Data[0xFFFC] = InsLDYZPX; //opdocde
    memory->Data[0xFFFD] = 0x44; // adress
    memory->Data[0x46] = 20; //value
    cpu->X = 0x2;
    cpu->executeI(cpu, memory, 4);
    printf("LDY Zero Page,X");
    assertEqual(cpu->Y,20);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDY Absolute
    memory->Data[0xFFFC] = InsLDYABS; //opdocde
    memory->Data[0xFFFD] = 0x42; //adress
    memory->Data[0xFFFE] = 0x42; //adress
    memory->Data[0x4242] = 10; //value
    cpu->executeI(cpu, memory, 4);
    printf("LDY Absolute TEST");
    assertEqual(cpu->Y,10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDY Absolute,X
    cpu->X = 1;
    memory->Data[0xFFFC] = InsLDYABSX; //opdocde
    memory->Data[0xFFFD] = 0x80; //adress
    memory->Data[0xFFFE] = 0x40; //adress
    memory->Data[0x4081] = 10; //value
    cpu->executeI(cpu, memory, 4);
    printf("LDY Absolute X TEST");
    assertEqual(cpu->Y,10);
    printf("\n");
    cpu->reset(cpu,memory,0);

    //LDY Absolute,X (page boundry)
    cpu->X = 0xFF;
    memory->Data[0xFFFC] = InsLDYABSX; //opdocde
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0xFFFE] = 0x44; //adress
    memory->Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu->executeI(cpu, memory, 5);
    printf("LDY Absolute X (with page boundry) TEST");
    assertEqual(cpu->Y,0x37);
    printf("\n");
    cpu->reset(cpu,memory,0);
}

