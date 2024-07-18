#include "cpu6502.h"

//LOAD OPCODES
//LDA
const Byte InsLDAIM = 0xA9; // LDA load Immediate
const Byte InsLDAZP = 0xA5; // LDA zero page
const Byte InsLDAZPX = 0xB5; // LDA zero page X
const Byte InsJSRABS = 0x20; // JSR absolute
const Byte InsLDAABS = 0xAD; //LAD Absolute
const Byte InsLDAABSX = 0xBD;//LAD Absolute,X
const Byte InsLDAABSY = 0xB9;//LAD Absolute,Y
const Byte InsLDAINDX = 0xA1;//LAD Indirect,X
const Byte InsLDAINDY = 0xB1;//LAD Indirect,Y
//LDS
const Byte InsLDXIM = 0xA2;//LDX Immediate
const Byte InsLDXZP = 0xA6;//LDX Zero Page
const Byte InsLDXZPY = 0xB6;//LDX Zero Page,Y
const Byte InsLDXABS = 0xAE; //LDX Absolute
const Byte InsLDXABSY = 0xBE;//LDX Absolute,Y
//LDY
const Byte InsLDYIM = 0xA0;//LDY Immediate
const Byte InsLDYZP = 0xA4;//LDY Zero Page
const Byte InsLDYZPX = 0xB4;//LDY Zero Page,x
const Byte InsLDYABS = 0xAC; //LDY Absolute
const Byte InsLDYABSX = 0XBC; //LDY Absolute,X


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

Word readWord(Memory *memory, u32 *cycles, Word adrress) {
    Byte lowByte =  readByteInMemory(memory,cycles,adrress);
    Byte highByte = readByteInMemory(memory,cycles,adrress+1);
    Word value = ((Word)(highByte << 8)) | lowByte;
    return value;
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

void updateFlagsLOAD(CPU *cpu) {
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
        //LOAD LDA CASES
        case InsLDAIM: {
            temp = fetchInstrucstion(cpu, memory,&cycles);
            cpu->ACC = temp;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDAZP: {
            Byte adress = fetchInstrucstion(cpu,memory,&cycles);
            temp = readByteInMemoryZeroPage(memory, &cycles, adress);
            cpu->ACC = temp;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDAZPX: {
            Byte adress = fetchInstrucstion(cpu,memory,&cycles);
            adress += cpu->X;
            temp = readByteInMemoryZeroPage(memory, &cycles, adress);
            cpu->ACC = temp;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDAABS: {
            Word adress = fetchWord(cpu,memory,&cycles);
            Byte data = readByteInMemory(memory,&cycles,adress);
            cpu->ACC = data;
            updateFlagsLOAD(cpu);
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
            updateFlagsLOAD(cpu);
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
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDAINDX: {
            Byte tempAdress = fetchInstrucstion(cpu,memory,&cycles);
            Word zeroPageBaseAdress = tempAdress + cpu->X;
            Word efectiveAdress = readWord(memory,&cycles,zeroPageBaseAdress);
            Byte value = readByteInMemory(memory,&cycles,efectiveAdress);
            cpu->ACC = value;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDAINDY: {
            Byte ZeroPageAdress = fetchInstrucstion(cpu,memory,&cycles);
            Word adress = readWord(memory,&cycles,ZeroPageAdress);
            Word efectiveadress = adress + cpu->Y;
            Byte value = readByteInMemory(memory,&cycles,efectiveadress);
            cpu->ACC = value;
            updateFlagsLOAD(cpu);
            if ((adress & 0xFF00) != (efectiveadress& 0xFF00)) {
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
        //LOAD LDX CASES
        case InsLDXIM: {
            temp = fetchInstrucstion(cpu, memory,&cycles);
            cpu->X = temp;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDXZP: {
            Byte adress = fetchInstrucstion(cpu,memory,&cycles);
            temp = readByteInMemoryZeroPage(memory, &cycles, adress);
            cpu->X = temp;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDXZPY: {
            Byte adress = fetchInstrucstion(cpu,memory,&cycles);
            adress += cpu->Y;
            temp = readByteInMemoryZeroPage(memory, &cycles, adress);
            cpu->X = temp;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDXABS: {
            Word adress = fetchWord(cpu,memory,&cycles);
            Byte data = readByteInMemory(memory,&cycles,adress);
            cpu->X = data;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDXABSY: {
            Word adress = fetchWord(cpu,memory,&cycles);
            Word adressX = adress + cpu->Y;
            Byte data = readByteInMemory(memory,&cycles,adressX);
            cpu->X = data;
            if ((adress & 0xFF00) != (adressX & 0xFF00)) {
                cycles--;
            }
            updateFlagsLOAD(cpu);
            break;
        }
        //LOAD LDX CASES
        case InsLDYIM: {
            temp = fetchInstrucstion(cpu, memory,&cycles);
            cpu->Y = temp;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDYZP: {
            Byte adress = fetchInstrucstion(cpu,memory,&cycles);
            temp = readByteInMemoryZeroPage(memory, &cycles, adress);
            cpu->Y = temp;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDYZPX: {
            Byte adress = fetchInstrucstion(cpu,memory,&cycles);
            adress += cpu->X;
            temp = readByteInMemoryZeroPage(memory, &cycles, adress);
            cpu->Y = temp;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDYABS: {
            Word adress = fetchWord(cpu,memory,&cycles);
            Byte data = readByteInMemory(memory,&cycles,adress);
            cpu->Y = data;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDYABSX: {
            Word adress = fetchWord(cpu,memory,&cycles);
            Word adressX = adress + cpu->X;
            Byte data = readByteInMemory(memory,&cycles,adressX);
            cpu->Y = data;
            if ((adress & 0xFF00) != (adressX & 0xFF00)) {
                cycles--;
            }
            updateFlagsLOAD(cpu);
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
