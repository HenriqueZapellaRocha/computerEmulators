#include "cpu6502.h"

// Definição das instruções opcode
const Byte InsLDAIM = 0xA9; // LDA load immediately
const Byte InsLDAZP = 0xA5; // LDA zero page
const Byte InsLDAZPX = 0xB5; // LDA zero page X
const Byte InsJSRABS = 0x20; // JSR absolute
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

Byte readByteInMemory(Memory *memory, u32 *cycles, u32 adrress) {
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
            temp = readByteInMemory(memory, &cycles, adress);
            cpu->ACC = temp;
            updateFlagsLDA(cpu);
            break;
        }
        case InsLDAZPX: {
            Byte adress = fetchInstrucstion(cpu,memory,&cycles);
            temp = readByteInMemory(memory, &cycles, adress);
            temp += cpu->X;
            cycles--;
            cpu->ACC = temp;
            updateFlagsLDA(cpu);
            break;
        }
        case InsJSRABS: {
            Word subAddr = fetchWord(cpu,memory,&cycles);
            writeWord(cpu->PC-1, cpu->SP,memory,&cycles);
            memory->Data[cpu->SP] = (unsigned char)(cpu->PC - 1);
            cycles--;
            cpu->PC = subAddr;
            cycles--;
            break;
        }
        default:
            break;
        }
    }
}

void startCPUMEMORY(CPU *cpu, Memory *memory) {
    (*cpu).reset = reset;
    (*cpu).executeI = executeI;
    (*cpu).fetchInstrucstion = fetchInstrucstion;
    (*cpu).readByteInMemory = readByteInMemory;
    (*cpu).fetchWord = fetchWord;
    (*memory).initMemory = initMemory;
    (*cpu).reset(cpu,memory);
}
