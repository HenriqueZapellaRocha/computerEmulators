#include "cpu6502.h"

// Definição das instruções opcode
const Byte InsLDAIM = 0xA9; // LDA load immediately
const Byte InsLDAZP = 0xA5; // LDA zero page
const Byte InsLDAZPX = 0xB5; // LDA zero page X
const Byte InsJSRABS = 0x20; // JSR absolute
const Byte InsLDAABS = 0xAD; //LAD Absolute
const Byte InsLDAABSX = 0xBD;//LAD Absolute,X
const Byte InsLDAABSY = 0xB9;//LAD Absolute,Y


const u32 maxMemorySize = 1024 * 64;


void reset(CPU *cpu, Memory *memory) {
    cpu->PC = 0x0FFFC;
    cpu->SP = 0xFF;
    cpu->ACC = cpu->X = cpu->Y = 0;
    cpu->C = cpu->Z = cpu->I = cpu->D = cpu->B = cpu->O = cpu->N = 0;
    memory->initMemory(memory);
}

void initMemory(Memory *memory) {
    for (u32 i=0; i < maxMemorySize; i++) {
        memory->Data[i] = 0;
    }
}

void writeWord(Word data, u32 address, Memory *memory, u32 *cycles) {
        memory->Data[address] = data & 0xFF;
        memory->Data[address+1] = (data >> 8);
        (*cycles)--;
}

Byte readByteInMemoryZeroPage(Memory *memory, u32 *cycles, Byte adrress) {
    Byte Data = memory->Data[adrress];
    (*cycles)--;
    return Data;
}

Byte readByteInMemory(Memory *memory, u32 *cycles, Word adrress) {
    Byte Data = memory->Data[adrress];
    (*cycles)--;
    return Data;
}


Word fetchWord(CPU *cpu, Memory *memory, u32 *cycles) {
    //take word
    Word Data = memory->Data[cpu->PC];
    cpu->PC++;
    
    //handle little endian
    Data |= (memory->Data[cpu->PC] << 8);
    cpu->PC++;
    (*cycles)-=2;
    return Data;
}

Byte fetchInstrucstion(CPU *cpu, Memory *memory, u32 *cycles) {
    Byte Data = memory->Data[cpu->PC];
    cpu->PC++;
    (*cycles)--;
    return Data;
}

void updateFlagsLDA(CPU *cpu) {
    if(cpu->ACC==0) {
        cpu->Z = 0x1;
    }
    if((cpu->ACC & 0b10000000) > 0) {
        cpu->N = 0x1;
    }
}

void executeI(CPU *cpu, Memory *memory, u32 cycles) {
    while(cycles > 0) {
        Byte opcode = fetchInstrucstion(cpu, memory, &cycles);
        Byte temp;
        switch (opcode)
        {
        case InsLDAIM: {
            temp = fetchInstrucstion(cpu, memory,&cycles);
            cpu->ACC = temp;
            updateFlagsLDA(cpu);
            break;
        }
        case InsLDAZP: {
            Byte adress = fetchInstrucstion(cpu,memory,&cycles);
            temp = readByteInMemoryZeroPage(memory, &cycles, adress);
            cpu->ACC = temp;
            updateFlagsLDA(cpu);
            break;
        }
        case InsLDAZPX: {
            Byte adress = fetchInstrucstion(cpu,memory,&cycles);
            adress += cpu->X;
            temp = readByteInMemoryZeroPage(memory, &cycles, adress);
            cpu->ACC = temp;
            updateFlagsLDA(cpu);
            break;
        }
        case InsLDAABS: {
            Word adress = fetchWord(cpu,memory,&cycles);
            Byte data = readByteInMemory(memory,&cycles,adress);
            cpu->ACC = data;
            break;
        }
        case InsLDAABSX: {
            Word adress = fetchWord(cpu,memory,&cycles);
            Word adressX = adress + cpu->X;
            Byte data = readByteInMemory(memory,&cycles,adressX);
            cpu->ACC = data;
            if ((adress & 0xFF00) != (adressX & 0xFF00)) {
                cycles--;
                }
            break;
        }
        case InsLDAABSY: {
            Word adress = fetchWord(cpu,memory,&cycles);
            Word adressY = adress + cpu->Y;
            Byte data = readByteInMemory(memory,&cycles,adressY);
            cpu->ACC = data;
            if ((adress & 0xFF00) != (adressY & 0xFF00)) {
                cycles--;
                }
            break;
        }
        case InsJSRABS: {
            Word subAddr = fetchWord(cpu,memory,&cycles);
            writeWord(cpu->PC-1, cpu->SP,memory,&cycles);
            memory->Data[cpu->SP] = (unsigned char)(cpu->PC - 1);
            cycles--;
            cpu->PC = subAddr;
            break;
        }
        default:
            break;
        }
        if(cycles != 0)
            cycles--;
    }
}

void startCPUMEMORY(CPU *cpu, Memory *memory) {
    (*cpu).reset = reset;
    (*cpu).executeI = executeI;
    (*cpu).fetchInstrucstion = fetchInstrucstion;
    (*cpu).readByteInMemoryZeroPage = readByteInMemoryZeroPage;
    (*cpu).readByteInMemory = readByteInMemory;
    (*cpu).fetchWord = fetchWord;
    (*memory).initMemory = initMemory;
    (*cpu).reset(cpu,memory);
}
