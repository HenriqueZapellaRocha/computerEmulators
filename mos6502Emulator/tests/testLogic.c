#include "../CPU/cpu6502.h"
#include "testLogic.h"
#include "TestFuncions.h"
#include <stdio.h>
void tests(CPU *cpu, Memory *memory, enum ops operation);

Byte makeLogicOp(enum ops operation, Byte n1, Byte n2) {
    if(operation == AND) {
        return n1 & n2;
    } else if (operation == EOR) {
        return n1 ^ n2;    
    } else {
        return n1 | n2;   
    }
    return -100;
}

void logicTests(CPU *cpu, Memory *memory) {
    tests(cpu,memory,AND);
    tests(cpu,memory,EOR);
    tests(cpu,memory,ORA);
    bitTests(cpu,memory);
}


void tests(CPU *cpu, Memory *memory, enum ops operation) {
    //Immediate
    cpu->reset(cpu,memory,0);
    cpu->ACC = 10;
    if(operation == AND) {
        memory->Data[0xFFFC] = InsANDIM; //opdocde
        printf("AND IMEDIATE TEST");
    } else if(operation == EOR) {
        memory->Data[0xFFFC] = InsEORIM; //opdocde
        printf("EOR IMEDIATE TEST");
    } else {
        memory->Data[0xFFFC] = InsORAIM; //opdocde
        printf("ORA IMEDIATE TEST"); 
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
    } else if(operation == EOR) {
        memory->Data[0xFFFC] = InsEORZP; //opdocde
        printf("EOR Zero Page TEST");  
    } else {
        memory->Data[0xFFFC] = InsORAZP; //opdocde
        printf("ORA Zero Page TEST"); 
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
    } else if(operation == EOR) {
        memory->Data[0xFFFC] = InsEORZPX; //opdocde
        printf("EOR Zero Page,X TEST");  
    } else {
        memory->Data[0xFFFC] = InsORAZPX; //opdocde
        printf("ORA Zero Page,X TEST");  
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
    } else if(operation == EOR) {
        memory->Data[0xFFFC] = InsEORABS; //opdocde
        printf("EOR Absolute TEST");
    } else {
        memory->Data[0xFFFC] = InsORAABS; //opdocde
        printf("ORA Absolute TEST");  
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
    } else if(operation == EOR) {
        memory->Data[0xFFFC] = InsEORABSX; //opdocde
        printf("EOR Absolute, X TEST"); 
    } else {
        memory->Data[0xFFFC] = InsORAABSX; //opdocde
        printf("ORA Absolute, X TEST"); 
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
    } else if(operation == EOR) {
        memory->Data[0xFFFC] = InsEORABSX; //opdocde
        printf("EOR Absolute X (with page boundry) TEST");
    } else {
        memory->Data[0xFFFC] = InsORAABSX; //opdocde
        printf("ORA Absolute X (with page boundry) TEST");
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
    } else if(operation == EOR) {
        memory->Data[0xFFFC] = InsEORABSY; //opdocde
        printf("EOR Absolute Y TEST");
    } else {
        memory->Data[0xFFFC] = InsORAABSY; //opdocde
        printf("ORA Absolute Y TEST");
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
    } else if(operation == EOR) {
        memory->Data[0xFFFC] = InsEORABSY; //opdocde
        printf("EOR Absolute Y (with page boundry) TEST");
    } else {
        memory->Data[0xFFFC] = InsORAABSY; //opdocde
        printf("ORA Absolute Y (with page boundry) TEST");
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
    } else if(operation == EOR) {
        memory->Data[0xFFFC] = InsEORINDX; //opdocde
        printf("EOR Indirect,X TEST");  
    } else {
        memory->Data[0xFFFC] = InsORAINDX; //opdocde
        printf("ORA Indirect,X TEST");  
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
    } else if(operation == EOR) {
        memory->Data[0xFFFC] = InsEORINDY; //opdocde
        printf("EOR Indirect,Y TEST");
    } else {
        memory->Data[0xFFFC] = InsORAINDY; //opdocde
        printf("ORA Indirect,Y TEST"); 
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
    } else if(operation == EOR) {
        memory->Data[0xFFFC] = InsEORINDY; //opdocde
        printf("EOR Indirect,Y (with page boundry) TEST"); 
    } else {
        memory->Data[0xFFFC] = InsORAINDY; //opdocde
        printf("ORA Indirect,Y (with page boundry) TEST");    
    }
    memory->Data[0xFFFD] = 0x02; //adress
    memory->Data[0x2] = 0x02; //adress
    memory->Data[0x3] = 0x80; //adress
    memory->Data[0x8101] = 10; //value cpu->y + 800
    cpu->executeI(cpu, memory, 6);
    assertEqual(cpu->ACC,makeLogicOp(operation,10,10));
    printf("\n");
}

void bitTests(CPU *cpu, Memory *memory) {
    //BIT Zero Page
    cpu->reset(cpu,memory,0);
    printf("BIT Zero Page TEST ");
    cpu->ACC = 0b00001111;
    memory->Data[0xFFFC] = InsBITZP;
    memory->Data[0xFFFD] = 0x02;
    memory->Data[0x02] = 0b11110000;
    cpu->executeI(cpu,memory,3);
    printf(" Z flag:");
    assertEqual(cpu->status.bits.Z, 1);
    printf(" N flag:");
    assertEqual(cpu->status.bits.Z, 1);
    printf(" V flag:");
    assertEqual(cpu->status.bits.V, 1);
    printf("\n");
    
    //BIT Absolute
    cpu->reset(cpu,memory,0);
    printf("BIT Absolute TEST ");
    cpu->ACC = 0b00001111;
    memory->Data[0xFFFC] = InsBITABS;
    memory->Data[0xFFFD] = 0x00;
    memory->Data[0xFFFE] = 0x80;
    memory->Data[0x8000] = 0b11110000;
    cpu->executeI(cpu,memory,4);
    printf(" Z flag:");
    assertEqual(cpu->status.bits.Z, 1);
    printf(" N flag:");
    assertEqual(cpu->status.bits.Z, 1);
    printf(" V flag:");
    assertEqual(cpu->status.bits.V, 1);
    printf("\n");
}
