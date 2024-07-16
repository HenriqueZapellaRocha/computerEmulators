#include "cpu6502.h"
#include <stdio.h>

int main(void) {
    CPU cpu;
    Memory memory;
    
    startCPUMEMORY(&cpu, &memory);
    
    //LDA Immediate
    memory.Data[0xFFFC] = InsLDAIM; //opdocde
    memory.Data[0xFFFD] = 10; // value
    cpu.executeI(&cpu, &memory, 2);
    printf("LDA IMEDIATE TEST, EXEXPECTED 10, RESULT: %d", cpu.ACC);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Zero Page
    memory.Data[0xFFFC] = InsLDAZP; //opdocde
    memory.Data[0xFFFD] = 0x42; // adress
    memory.Data[0x42] = 20; //value
    cpu.executeI(&cpu, &memory, 3);
    printf("LDA Zero Page TEST, EXEXPECTED 20, RESULT: %d", cpu.ACC);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Zero Page,X
    memory.Data[0xFFFC] = InsLDAZPX; //opdocde
    memory.Data[0xFFFD] = 0x44; // adress
    memory.Data[0x46] = 20; //value
    cpu.X = 0x2;
    cpu.executeI(&cpu, &memory, 3);
    printf("LDA Zero Page,X TEST, EXPECTED 20, RESULT: %d", cpu.ACC);
    printf("\n\n");
    cpu.reset(&cpu,&memory);

    //LDA Absolute
    memory.Data[0xFFFC] = InsLDAABS; //opdocde
    memory.Data[0xFFFD] = 0x42; //adress
    memory.Data[0xFFFE] = 0x42; //adress
    memory.Data[0x4242] = 10; //value
    cpu.executeI(&cpu, &memory, 4);
    printf("LDA Absolute TEST, EXEXPECTED 10, RESULT: %d", cpu.ACC);
    printf("\n\n");;
    cpu.reset(&cpu,&memory);

    //LDA Absolute,X
    cpu.X = 1;
    memory.Data[0xFFFC] = InsLDAABSX; //opdocde
    memory.Data[0xFFFD] = 0x80; //adress
    memory.Data[0xFFFE] = 0x40; //adress
    memory.Data[0x4081] = 10; //value
    cpu.executeI(&cpu, &memory, 4);
    printf("LDA Absolute X TEST, EXEXPECTED 10, RESULT: %d", cpu.ACC);
    printf("\n\n");;
    cpu.reset(&cpu,&memory);

    //LDA Absolute,X (page boundry)
    cpu.X = 0xFF;
    memory.Data[0xFFFC] = InsLDAABSX; //opdocde
    memory.Data[0xFFFD] = 0x02; //adress
    memory.Data[0xFFFE] = 0x44; //adress
    memory.Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu.executeI(&cpu, &memory, 5);
    printf("LDA Absolute X (with page boundry) TEST, EXEXPECTED 0x37, RESULT: 0x%x", cpu.ACC);
    printf("\n\n");;
    cpu.reset(&cpu,&memory);

    //LDA Absolute,y
    cpu.Y = 1;
    memory.Data[0xFFFC] = InsLDAABSY; //opdocde
    memory.Data[0xFFFD] = 0x80; //adress
    memory.Data[0xFFFE] = 0x40; //adress
    memory.Data[0x4081] = 10; //value
    cpu.executeI(&cpu, &memory, 4);
    printf("LDA Absolute Y TEST, EXEXPECTED 10, RESULT: %d", cpu.ACC);
    printf("\n\n");;
    cpu.reset(&cpu,&memory);

    //LDA Absolute,y (page boundry)
    cpu.Y = 0xFF;
    memory.Data[0xFFFC] = InsLDAABSY; //opdocde
    memory.Data[0xFFFD] = 0x02; //adress
    memory.Data[0xFFFE] = 0x44; //adress
    memory.Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu.executeI(&cpu, &memory, 5);
    printf("LDA Absolute Y (with page boundry) TEST, EXEXPECTED 0x37, RESULT: 0x%x", cpu.ACC);
    printf("\n\n");;
    cpu.reset(&cpu,&memory);


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

