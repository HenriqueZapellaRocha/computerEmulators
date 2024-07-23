#include "../CPU/cpu6502.h"
#include "testLogic.h"
#include "TestFuncions.h"
#include <stdio.h>
void tests(CPU *cpu, Memory *memory, enum ops operation);

Byte makeLogicOp(enum ops operation, Byte n1, Byte n2) {
    if(operation == AND) {
        return n1 & n2;
    }
    return -100;
}

void ANDTests(CPU *cpu, Memory *memory) {
    tests(cpu,memory,AND);
}


void tests(CPU *cpu, Memory *memory, enum ops operation) {
    //Immediate
    cpu->reset(cpu,memory,0);
    cpu->ACC = 10;
    if(operation == AND) {
        memory->Data[0xFFFC] = InsANDIM; //opdocde
        printf("AND IMEDIATE TEST");
    }
    memory->Data[0xFFFD] = 0x04; // value
    cpu->executeI(cpu, memory, 2);
    assertEqual(cpu->ACC,makeLogicOp(operation,10,0x04));
    printf("\n");

    //Zero Page
    cpu->reset(cpu,memory,0);
    cpu->ACC = 10;
    if(operation == AND) {
        memory->Data[0xFFFC] = InsANDZP; //opdocde
        printf("AND Zero Page TEST");
    }
    memory->Data[0xFFFD] = 0x42; // adress
    memory->Data[0x42] = 20; //value
    cpu->executeI(cpu, memory, 3);
    assertEqual(cpu->ACC,makeLogicOp(operation,10,20));
    printf("\n");
    cpu->reset(cpu,memory,0);

    //Zero Page,X
    cpu->reset(cpu,memory,0);
    cpu->ACC = 10; 
    if(operation == AND) {
        memory->Data[0xFFFC] = InsANDZPX; //opdocde
        printf("AND Zero Page,X TEST");
    }
    memory->Data[0xFFFD] = 0x44; // adress
    memory->Data[0x46] = 20; //value
    cpu->X = 0x2;
    cpu->executeI(cpu, memory, 4);
    assertEqual(cpu->ACC,makeLogicOp(operation,10,20));
    printf("\n");

    //Absolute
    cpu->reset(cpu,memory,0);
    cpu->ACC = 10; 
    if(operation == AND) {
        memory->Data[0xFFFC] = InsANDABS; //opdocde
        printf("AND Absolute TEST");
    }
    memory->Data[0xFFFD] = 0x42; //adress
    memory->Data[0xFFFE] = 0x42; //adress
    memory->Data[0x4242] = 10; //value
    cpu->executeI(cpu, memory, 4);
    assertEqual(cpu->ACC,makeLogicOp(operation,10,10));
    printf("\n");

    //Absolute,X
    cpu->reset(cpu,memory,0);
    cpu->ACC = 10;
    cpu->X = 1;
    if(operation == AND) {
        memory->Data[0xFFFC] = InsANDABSX; //opdocde
        printf("AND Absolute, X TEST");
    }
    memory->Data[0xFFFD] = 0x80; //adress
    memory->Data[0xFFFE] = 0x40; //adress
    memory->Data[0x4081] = 10; //value
    cpu->executeI(cpu, memory, 4);
    assertEqual(cpu->ACC,makeLogicOp(operation,10,10));
    printf("\n");

    //Absolute,X (page boundry)
    cpu->reset(cpu,memory,0);
    cpu->ACC = 10;
    cpu->X = 0xFF;
    if(operation == AND) {
        memory->Data[0xFFFC] = InsANDABSX; //opdocde
        printf("AND Absolute X (with page boundry) TEST");
    }
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0xFFFE] = 0x44; //adress
    memory->Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu->executeI(cpu, memory, 5);
    assertEqual(cpu->ACC,makeLogicOp(operation,10,0x37));
    printf("\n");

    //Absolute,y
    cpu->reset(cpu,memory,0);
    cpu->Y = 1;
    cpu->ACC = 10;
    if(operation == AND) {
        memory->Data[0xFFFC] = InsANDABSY; //opdocde
        printf("AND Absolute Y TEST");
    }
    memory->Data[0xFFFD] = 0x80; //adress
    memory->Data[0xFFFE] = 0x40; //adress
    memory->Data[0x4081] = 10; //value
    cpu->executeI(cpu, memory, 4);
    assertEqual(cpu->ACC,makeLogicOp(operation,10,10));
    printf("\n");

    //Absolute,y (page boundry)
    cpu->reset(cpu,memory,0);
    cpu->Y = 0xFF;
    cpu->ACC = 10;
    if(operation == AND) {
        memory->Data[0xFFFC] = InsANDABSY; //opdocde
        printf("AND Absolute Y (with page boundry) TEST");
    }
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0xFFFE] = 0x44; //adress
    memory->Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu->executeI(cpu, memory, 5);
    assertEqual(cpu->ACC,makeLogicOp(operation,10,0x37));
    printf("\n");

    //Inderect,X 
    cpu->reset(cpu,memory,0);
    cpu->X = 0x04;
    cpu->ACC = 10;
    if(operation == AND) {
        memory->Data[0xFFFC] = InsANDINDX; //opdocde
        printf("AND Indirect,X TEST");
    }
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0x6] = 0x0; //adress
    memory->Data[0x7] = 0x80; //adress
    memory->Data[0x8000] = 10; //value
    cpu->executeI(cpu, memory, 6);
    assertEqual(cpu->ACC,makeLogicOp(operation,10,10));
    printf("\n");

    //Inderect,Y
    cpu->reset(cpu,memory,0);
    cpu->Y = 0x4;
    cpu->ACC = 10;
    if(operation == AND) {
        memory->Data[0xFFFC] = InsANDINDY; //opdocde
        printf("AND Indirect,Y TEST");
    }
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0x2] = 0x0; //adress
    memory->Data[0x3] = 0x80; //adress
    memory->Data[0x8004] = 10; //value cpu->y + 800
    cpu->executeI(cpu, memory, 5);
    assertEqual(cpu->ACC,makeLogicOp(operation,10,10));
    printf("\n");

    //Inderect,Y (Page boundry)
    cpu->reset(cpu,memory,0);
    cpu->Y = 0xFF;
    cpu->ACC = 10;
    if(operation == AND) {
        memory->Data[0xFFFC] = InsANDINDY; //opdocde
        printf("AND Indirect,Y (with page boundry) TEST");
    }
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0x2] = 0x02; //adress
    memory->Data[0x3] = 0x80; //adress
    memory->Data[0x8101] = 10; //value cpu->y + 800
    cpu->executeI(cpu, memory, 6);
    assertEqual(cpu->ACC,makeLogicOp(operation,10,10));
    printf("\n");
}