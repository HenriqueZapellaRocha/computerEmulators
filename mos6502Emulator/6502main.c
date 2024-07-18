#include "cpu6502.h"
#include <stdio.h>
#include "TestFuncions.h"

void LoadAInstructionsTest(CPU cpu, Memory memory); 

int main(void) {
    CPU cpu;
    Memory memory;
    
    startCPUMEMORY(&cpu, &memory);
    
    LoadAInstructionsTest(cpu,memory);

    //JSR 
    memory.Data[0xFFFC] = InsJSRABS; //opdocde
    memory.Data[0xFFFD] = 0x42; //adress
    memory.Data[0xFFFE] = 0x42; //adress
    memory.Data[0x4242] = InsLDAIM; //immediate lda
    memory.Data[0x4243] = 10;
    cpu.executeI(&cpu, &memory, 8);
    printf("JSR TEST, EXEXPECTED 10, RESULT: %d", cpu.ACC);
    printf("\n\n");;
    cpu.reset(&cpu,&memory);

    
    return 0; 
}

void LoadAInstructionsTest(CPU cpu, Memory memory) {
    //LDA Immediate
    memory.Data[0xFFFC] = InsLDAIM; //opdocde
    memory.Data[0xFFFD] = 10; // value
    cpu.executeI(&cpu, &memory, 2);
    printf("LDA IMEDIATE TEST");
    assertEqual(cpu.ACC,10);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Zero Page
    memory.Data[0xFFFC] = InsLDAZP; //opdocde
    memory.Data[0xFFFD] = 0x42; // adress
    memory.Data[0x42] = 20; //value
    cpu.executeI(&cpu, &memory, 3);
    printf("LDA Zero Page TEST");
    assertEqual(cpu.ACC,20);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Zero Page,X
    memory.Data[0xFFFC] = InsLDAZPX; //opdocde
    memory.Data[0xFFFD] = 0x44; // adress
    memory.Data[0x46] = 20; //value
    cpu.X = 0x2;
    cpu.executeI(&cpu, &memory, 3);
    printf("LDA Zero Page,X");
    assertEqual(cpu.ACC,20);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Absolute
    memory.Data[0xFFFC] = InsLDAABS; //opdocde
    memory.Data[0xFFFD] = 0x42; //adress
    memory.Data[0xFFFE] = 0x42; //adress
    memory.Data[0x4242] = 10; //value
    cpu.executeI(&cpu, &memory, 4);
    printf("LDA Absolute TEST");
    assertEqual(cpu.ACC,10);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Absolute,X
    cpu.X = 1;
    memory.Data[0xFFFC] = InsLDAABSX; //opdocde
    memory.Data[0xFFFD] = 0x80; //adress
    memory.Data[0xFFFE] = 0x40; //adress
    memory.Data[0x4081] = 10; //value
    cpu.executeI(&cpu, &memory, 4);
    printf("LDA Absolute X TEST");
    assertEqual(cpu.ACC,10);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Absolute,X (page boundry)
    cpu.X = 0xFF;
    memory.Data[0xFFFC] = InsLDAABSX; //opdocde
    memory.Data[0xFFFD] = 0x02; //adress
    memory.Data[0xFFFE] = 0x44; //adress
    memory.Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu.executeI(&cpu, &memory, 5);
    printf("LDA Absolute X (with page boundry) TEST");
    assertEqual(cpu.ACC,0x37);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Absolute,y
    cpu.Y = 1;
    memory.Data[0xFFFC] = InsLDAABSY; //opdocde
    memory.Data[0xFFFD] = 0x80; //adress
    memory.Data[0xFFFE] = 0x40; //adress
    memory.Data[0x4081] = 10; //value
    cpu.executeI(&cpu, &memory, 4);
    printf("LDA Absolute Y TEST");
    assertEqual(cpu.ACC,10);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Absolute,y (page boundry)
    cpu.Y = 0xFF;
    memory.Data[0xFFFC] = InsLDAABSY; //opdocde
    memory.Data[0xFFFD] = 0x02; //adress
    memory.Data[0xFFFE] = 0x44; //adress
    memory.Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu.executeI(&cpu, &memory, 5);
    printf("LDA Absolute Y (with page boundry) TEST, EXEXPECTED 0x37, RESULT: 0x%x", cpu.ACC);
    assertEqual(cpu.ACC,0x37);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Inderect,X 
    cpu.X = 0x04;
    memory.Data[0xFFFC] = InsLDAINDX; //opdocde
    memory.Data[0xFFFD] = 0x02; //adress
    memory.Data[0x6] = 0x0; //adress
    memory.Data[0x7] = 0x80; //adress
    memory.Data[0x8000] = 10; //value
    cpu.executeI(&cpu, &memory, 6);
    printf("LDA Inderect,X TEST");
    assertEqual(cpu.ACC,10);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Inderect,Y
    cpu.Y = 0x4;
    memory.Data[0xFFFC] = InsLDAINDY; //opdocde
    memory.Data[0xFFFD] = 0x02; //adress
    memory.Data[0x2] = 0x0; //adress
    memory.Data[0x3] = 0x80; //adress
    memory.Data[0x8004] = 10; //value cpu.y + 800
    cpu.executeI(&cpu, &memory, 5);
    printf("LDA Inderect,Y TEST");
    assertEqual(cpu.ACC,10);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Inderect,Y (Page boundry)
    cpu.Y = 0xFF;
    memory.Data[0xFFFC] = InsLDAINDY; //opdocde
    memory.Data[0xFFFD] = 0x02; //adress
    memory.Data[0x2] = 0x02; //adress
    memory.Data[0x3] = 0x80; //adress
    memory.Data[0x8101] = 10; //value cpu.y + 800
    cpu.executeI(&cpu, &memory, 6);
    printf("LDA Inderect,Y (Page Boundry) TEST");
    assertEqual(cpu.ACC,10);
    printf("\n\n");;
    cpu.reset(&cpu,&memory);
}

