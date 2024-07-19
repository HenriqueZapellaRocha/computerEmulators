#include "cpu6502.h"
#include <stdio.h>
#include "TestFuncions.h"

void LoadAInstructionsTest(CPU cpu, Memory memory); 
void LoadXInstructionsTest(CPU cpu, Memory memory);
void LoadYInstructionsTest(CPU cpu, Memory memory);
void StoreAInstructionsTest(CPU cpu, Memory memory);

int main(void) {
    CPU cpu;
    Memory memory;
    
    startCPUMEMORY(&cpu, &memory);

    int testSelection = 4;

    switch (testSelection)
    {
    case 1: {
        LoadAInstructionsTest(cpu,memory);
        break;
    }
    case 2: {
        LoadXInstructionsTest(cpu,memory);
        break;
    }
    case 3: {
        LoadYInstructionsTest(cpu,memory);
        break;
    }
    case 4: {
        StoreAInstructionsTest(cpu,memory);
        break;
    }
    default:
        return 0;
    }

    /*
    //JSR 
    memory.Data[0xFFFC] = InsJSRABS; //opdocde
    memory.Data[0xFFFD] = 0x42; //adress
    memory.Data[0xFFFE] = 0x42; //adress
    memory.Data[0x4242] = InsLDAIM; //immediate lda
    memory.Data[0x4243] = 10;
    cpu.executeI(&cpu, &memory, 8);
    printf("JSR TEST, EXEXPECTED 10, RESULT: %d", cpu.ACC);
    printf("\n");;
    cpu.reset(&cpu,&memory);
    */
    
    return 0; 
}






void LoadAInstructionsTest(CPU cpu, Memory memory) {
    //LDA Immediate
    memory.Data[0xFFFC] = InsLDAIM; //opdocde
    memory.Data[0xFFFD] = 10; // value
    cpu.executeI(&cpu, &memory, 2);
    printf("LDA IMEDIATE TEST");
    assertEqual(cpu.ACC,10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDA Zero Page
    memory.Data[0xFFFC] = InsLDAZP; //opdocde
    memory.Data[0xFFFD] = 0x42; // adress
    memory.Data[0x42] = 20; //value
    cpu.executeI(&cpu, &memory, 3);
    printf("LDA Zero Page TEST");
    assertEqual(cpu.ACC,20);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDA Zero Page,X
    memory.Data[0xFFFC] = InsLDAZPX; //opdocde
    memory.Data[0xFFFD] = 0x44; // adress
    memory.Data[0x46] = 20; //value
    cpu.X = 0x2;
    cpu.executeI(&cpu, &memory, 4);
    printf("LDA Zero Page,X");
    assertEqual(cpu.ACC,20);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDA Absolute
    memory.Data[0xFFFC] = InsLDAABS; //opdocde
    memory.Data[0xFFFD] = 0x42; //adress
    memory.Data[0xFFFE] = 0x42; //adress
    memory.Data[0x4242] = 10; //value
    cpu.executeI(&cpu, &memory, 4);
    printf("LDA Absolute TEST");
    assertEqual(cpu.ACC,10);
    printf("\n");
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
    printf("\n");
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
    printf("\n");
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
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDA Absolute,y (page boundry)
    cpu.Y = 0xFF;
    memory.Data[0xFFFC] = InsLDAABSY; //opdocde
    memory.Data[0xFFFD] = 0x02; //adress
    memory.Data[0xFFFE] = 0x44; //adress
    memory.Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu.executeI(&cpu, &memory, 5);
    printf("LDA Absolute Y (with page boundry) TEST");
    assertEqual(cpu.ACC,0x37);
    printf("\n");
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
    printf("\n");
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
    printf("\n");
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
    printf("\n");;
    cpu.reset(&cpu,&memory);
}

void LoadXInstructionsTest(CPU cpu, Memory memory) {

    //LDX Immediate
    memory.Data[0xFFFC] = InsLDXIM; //opdocde
    memory.Data[0xFFFD] = 10; // value
    cpu.executeI(&cpu, &memory, 2);
    printf("LDX IMEDIATE TEST");
    assertEqual(cpu.X,10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDX Zero Page
    memory.Data[0xFFFC] = InsLDXZP; //opdocde
    memory.Data[0xFFFD] = 0x42; // adress
    memory.Data[0x42] = 20; //value
    cpu.executeI(&cpu, &memory, 3);
    printf("LDX Zero Page TEST");
    assertEqual(cpu.X,20);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDX Zero Page,Y
    memory.Data[0xFFFC] = InsLDXZPY; //opdocde
    memory.Data[0xFFFD] = 0x44; // adress
    memory.Data[0x46] = 20; //value
    cpu.Y = 0x2;
    cpu.executeI(&cpu, &memory, 4);
    printf("LDX Zero Page,Y");
    assertEqual(cpu.X,20);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDX Absolute
    memory.Data[0xFFFC] = InsLDXABS; //opdocde
    memory.Data[0xFFFD] = 0x42; //adress
    memory.Data[0xFFFE] = 0x42; //adress
    memory.Data[0x4242] = 10; //value
    cpu.executeI(&cpu, &memory, 4);
    printf("LDX Absolute TEST");
    assertEqual(cpu.X,10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDX Absolute,y
    cpu.Y = 1;
    memory.Data[0xFFFC] = InsLDXABSY; //opdocde
    memory.Data[0xFFFD] = 0x80; //adress
    memory.Data[0xFFFE] = 0x40; //adress
    memory.Data[0x4081] = 10; //value
    cpu.executeI(&cpu, &memory, 4);
    printf("LDX Absolute Y TEST");
    assertEqual(cpu.X,10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDX Absolute,y (page boundry)
    cpu.Y = 0xFF;
    memory.Data[0xFFFC] = InsLDXABSY; //opdocde
    memory.Data[0xFFFD] = 0x02; //adress
    memory.Data[0xFFFE] = 0x44; //adress
    memory.Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu.executeI(&cpu, &memory, 5);
    printf("LDX Absolute Y (with page boundry) TEST");
    assertEqual(cpu.X,0x37);
    printf("\n");
    cpu.reset(&cpu,&memory);

}

void LoadYInstructionsTest(CPU cpu, Memory memory) {

    //LDY Immediate
    memory.Data[0xFFFC] = InsLDYIM; //opdocde
    memory.Data[0xFFFD] = 10; // value
    cpu.executeI(&cpu, &memory, 2);
    printf("LDY IMEDIATE TEST");
    assertEqual(cpu.Y,10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDY Zero Page
    memory.Data[0xFFFC] = InsLDYZP; //opdocde
    memory.Data[0xFFFD] = 0x42; // adress
    memory.Data[0x42] = 20; //value
    cpu.executeI(&cpu, &memory, 3);
    printf("LDY Zero Page TEST");
    assertEqual(cpu.Y,20);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDY Zero Page,X
    memory.Data[0xFFFC] = InsLDYZPX; //opdocde
    memory.Data[0xFFFD] = 0x44; // adress
    memory.Data[0x46] = 20; //value
    cpu.X = 0x2;
    cpu.executeI(&cpu, &memory, 4);
    printf("LDY Zero Page,X");
    assertEqual(cpu.Y,20);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDY Absolute
    memory.Data[0xFFFC] = InsLDYABS; //opdocde
    memory.Data[0xFFFD] = 0x42; //adress
    memory.Data[0xFFFE] = 0x42; //adress
    memory.Data[0x4242] = 10; //value
    cpu.executeI(&cpu, &memory, 4);
    printf("LDY Absolute TEST");
    assertEqual(cpu.Y,10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDY Absolute,X
    cpu.X = 1;
    memory.Data[0xFFFC] = InsLDYABSX; //opdocde
    memory.Data[0xFFFD] = 0x80; //adress
    memory.Data[0xFFFE] = 0x40; //adress
    memory.Data[0x4081] = 10; //value
    cpu.executeI(&cpu, &memory, 4);
    printf("LDY Absolute X TEST");
    assertEqual(cpu.Y,10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //LDY Absolute,X (page boundry)
    cpu.X = 0xFF;
    memory.Data[0xFFFC] = InsLDYABSX; //opdocde
    memory.Data[0xFFFD] = 0x02; //adress
    memory.Data[0xFFFE] = 0x44; //adress
    memory.Data[0x4501/*cross page boundry*/] = 0x37; 
    cpu.executeI(&cpu, &memory, 5);
    printf("LDY Absolute X (with page boundry) TEST");
    assertEqual(cpu.Y,0x37);
    printf("\n");
    cpu.reset(&cpu,&memory);
}

void StoreAInstructionsTest(CPU cpu, Memory memory) {
    //STA Zero page
    cpu.ACC = 10;
    memory.Data[0xFFFC] = InsSTAZP; //opdocde
    memory.Data[0xFFFD] = 0x2;
    memory.Data[0x2] = 0;
    printf("STA Zero Page TEST");
    cpu.executeI(&cpu,&memory,3);
    assertEqual(memory.Data[0x2],10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //STA Zero page,X
    cpu.ACC = 10;
    cpu.X = 0x10;
    memory.Data[0xFFFC] = InsSTAZPX; //opdocde
    memory.Data[0xFFFD] = 0x2;
    memory.Data[0x02 + 0x10] = 0;
    printf("STA Zero Page,X TEST");
    cpu.executeI(&cpu,&memory,4);
    assertEqual(memory.Data[0x02 + 0x10],10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //STA Absolute
    cpu.ACC = 10;
    memory.Data[0xFFFC] = InsSTAABS; //opdocde
    memory.Data[0xFFFD] = 0x42; //adress
    memory.Data[0xFFFE] = 0x42; //adress
    memory.Data[0x4242] = 0; //value
    printf("STA Absolute TEST");
    cpu.executeI(&cpu, &memory, 4);
    assertEqual(memory.Data[0x4242],10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //STA Absolute,X
    cpu.ACC = 10;
    cpu.X = 0x40;
    memory.Data[0xFFFC] = InsSTAABSX; //opdocde
    memory.Data[0xFFFD] = 0xFF; //adress
    memory.Data[0x40 + 0XFF] = 0; //value
    printf("STA Absolute,X TEST");
    cpu.executeI(&cpu, &memory, 5);
    assertEqual(memory.Data[0x40 + 0XFF],10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //STA Absolute,Y
    cpu.ACC = 10;
    cpu.Y = 0x40;
    memory.Data[0xFFFC] = InsSTAABSY; //opdocde
    memory.Data[0xFFFD] = 0xFF; //adress
    memory.Data[0x40 + 0xFF] = 0; //value
    printf("STA Absolute,Y TEST");
    cpu.executeI(&cpu, &memory, 5);
    assertEqual(memory.Data[0x40+0xFF],10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //STA Indirect,X
    cpu.ACC = 10;
    cpu.X = 0x0F;
    memory.Data[0xFFFC] = InsSTAINDX; //opdocde
    memory.Data[0xFFFD] = 0x20; //adress
    memory.Data[0x002F] = 0x0; //adress
    memory.Data[0x0030] = 0x80;
    memory.Data[0x8000] = 0; //value
    cpu.executeI(&cpu, &memory, 6);
    printf("STA Indirect,X TEST");
    assertEqual(memory.Data[0x8000],10);
    printf("\n");
    cpu.reset(&cpu,&memory);

    //STA Indirect,Y
    cpu.ACC = 10;
    cpu.Y = 0x0F;
    memory.Data[0xFFFC] = InsSTAINDY; //opdocde
    memory.Data[0xFFFD] = 0x20; //adress
    memory.Data[0x0020] = 0x0; //adress
    memory.Data[0x0021] = 0x80;
    memory.Data[0x8000 + 0x0F] = 0; //value
    cpu.executeI(&cpu, &memory, 6);
    printf("STA Indirect,Y TEST");
    assertEqual(memory.Data[0x8000 + 0xF],10);
    printf("\n");
    cpu.reset(&cpu,&memory);

}


