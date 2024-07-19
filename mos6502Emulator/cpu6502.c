#include "cpu6502.h"
#include <stdio.h>

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

//STORE OPCODES
//STA
const Byte InsSTAZP = 0x85;//STA Zero Page
const Byte InsSTAZPX = 0x95;//STA Zero Page,X
const Byte InsSTAABS = 0x8D;//STA Absolute
const Byte InsSTAABSX = 0x9D;//STA Absolute,X
const Byte InsSTAABSY = 0x99;//STA Absolute,Y
const Byte InsSTAINDX = 0x81;//STA Indirect,X
const Byte InsSTAINDY = 0x91;//STA Indirect,Y
//STX
const Byte InsSTXZP = 0x86;//STX Zero Page
const Byte InsSTXZPY = 0x96;//STX Zero Page,Y
const Byte InsSTXABS = 0x8E;//STX Absolute
//STY
const Byte InsSTYZP = 0x84;//STY Zero Page
const Byte InsSTYZPX = 0x94;//STY Zero Page,X
const Byte InsSTYABS = 0x8C;//STX Absolute


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

void writeByteInMemoryFromRegister(Byte regis, Memory *memory,u32 *cycles, Word adress) {
    memory->Data[adress] = regis;
    (*cycles)--;
}

Byte zeroPageValue(CPU *cpu, Memory *memory,u32 *cycles) {
    Byte adress = fetchInstrucstion(cpu,memory,cycles);
    Byte value = readByteInMemoryZeroPage(memory,cycles, adress);
    return value;
}

Byte zeroPageAdress(CPU *cpu, Memory *memory,u32 *cycles) {
    return fetchInstrucstion(cpu,memory,cycles);
}

Byte zeroPageXAdress(CPU *cpu, Memory *memory,u32 *cycles) {
    Byte zpAdrees = fetchInstrucstion(cpu,memory,cycles);
    Byte finalAdress = cpu->X + zpAdrees;
    (*cycles)--;
    return finalAdress;
}

Byte zeroPageYAdress(CPU *cpu, Memory *memory,u32 *cycles) {
    Byte zpAdrees = fetchInstrucstion(cpu,memory,cycles);
    Byte finalAdress = cpu->Y + zpAdrees;
    (*cycles)--;
    return finalAdress;
}

Word AbsoluteXAdress(CPU *cpu, Memory *memory,u32 *cycles) {
    Word adress = fetchWord(cpu,memory,cycles);
    Word adressX = adress + cpu->X;
    if ((adress & 0xFF00) != (adressX & 0xFF00)) {
        (*cycles)--;
    }
    return adressX;
}

Word AbsoluteYAdress(CPU *cpu, Memory *memory,u32 *cycles) {
    Word adress = fetchWord(cpu,memory,cycles);
    Word adressX = adress + cpu->Y;
    if ((adress & 0xFF00) != (adressX & 0xFF00)) {
        (*cycles)--;
    }
    return adressX;
}

Word AbsoluteAdress(CPU *cpu, Memory *memory,u32 *cycles) {
    return fetchWord(cpu,memory,cycles);
}

Byte zeroPageXValue(CPU *cpu, Memory *memory,u32 *cycles) {
    Byte adress = zeroPageXAdress(cpu,memory,cycles);
    Byte temp = readByteInMemoryZeroPage(memory, cycles, adress);
    return temp;
}

Byte zeroPageYValue(CPU *cpu, Memory *memory,u32 *cycles) {
    Byte adress = fetchInstrucstion(cpu,memory,cycles);
    adress += cpu->Y;
    (*cycles)--;
    Byte temp = readByteInMemoryZeroPage(memory, cycles, adress);
    return temp;
}

Byte absoluteValue(CPU *cpu, Memory *memory,u32 *cycles) {
    Word adress = AbsoluteAdress(cpu,memory,cycles);
    return readByteInMemory(memory,cycles,adress);
}

Byte absoluteXValue(CPU *cpu, Memory *memory,u32 *cycles) {
    Word adressX = AbsoluteXAdress(cpu,memory,cycles);
    Byte data = readByteInMemory(memory,cycles,adressX);
    return data;
}

Byte absoluteYValue(CPU *cpu, Memory *memory,u32 *cycles) {
    Word adressY = AbsoluteYAdress(cpu,memory,cycles);
    Byte data = readByteInMemory(memory,cycles,adressY);
    return data;
}

Byte indirectXValue(CPU *cpu, Memory *memory,u32 *cycles) {
    Byte tempAdress = fetchInstrucstion(cpu,memory,cycles);
    Word zeroPageBaseAdress = tempAdress + cpu->X;
    Word efectiveAdress = readWord(memory,cycles,zeroPageBaseAdress);
    Byte value = readByteInMemory(memory,cycles,efectiveAdress);
    (*cycles)--;
    return value;
}

Word indirectXAdress(CPU *cpu, Memory *memory,u32 *cycles) {
    Byte tempAdress = fetchInstrucstion(cpu,memory,cycles);
    Word zeroPageBaseAdress = tempAdress + cpu->X;
    (*cycles)--;
    Word efectiveAdress = readWord(memory,cycles,zeroPageBaseAdress);
    return efectiveAdress;
}

Byte indirectYValue(CPU *cpu, Memory *memory,u32 *cycles) {
    Byte zeroPageValue = fetchInstrucstion(cpu,memory,cycles);
    Word adress = readWord(memory,cycles,zeroPageValue);
    Word efectiveadress = adress + cpu->Y;
    Byte value = readByteInMemory(memory,cycles,efectiveadress);
    if ((adress & 0xFF00) != (efectiveadress& 0xFF00)) {
    (*cycles)--;
    }
    return value;
}

Word indirectYAdress(CPU *cpu, Memory *memory,u32 *cycles) {
    Byte zeroPageValue = fetchInstrucstion(cpu,memory,cycles);
    Word adress = readWord(memory,cycles,zeroPageValue);
    Word efectiveadress = adress + cpu->Y;
    (*cycles)--;
    if ((adress & 0xFF00) != (efectiveadress& 0xFF00)) {
    (*cycles)--;
    }
    return efectiveadress;
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
            cpu->ACC = zeroPageValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDAZPX: {
            cpu->ACC = zeroPageXValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDAABS: {
            cpu->ACC = absoluteValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDAABSX: {
            cpu->ACC = absoluteXValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDAABSY: {
            cpu->ACC = absoluteYValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDAINDX: {
            cpu->ACC = indirectXValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDAINDY: {
            cpu->ACC = indirectYValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
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
            cpu->X = zeroPageValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDXZPY: {
            cpu->X = zeroPageYValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDXABS: {
            cpu->X =  absoluteValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDXABSY: {
            cpu->X = absoluteYValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        //LOAD LDY CASES
        case InsLDYIM: {
            temp = fetchInstrucstion(cpu, memory,&cycles);
            cpu->Y = temp;
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDYZP: {
            cpu->Y = zeroPageValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDYZPX: {
            cpu->Y =zeroPageXValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDYABS: {
            cpu->Y =  absoluteValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        case InsLDYABSX: {
            cpu->Y = absoluteXValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu);
            break;
        }
        //Store STA CASES 
        case InsSTAZP: {
            Byte addres = zeroPageAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->ACC, memory,&cycles,addres);
            break;
        }
        case InsSTAZPX: {
            Byte addres = zeroPageXAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->ACC, memory,&cycles,addres);
            break;
        }
        case InsSTAABS: {
            Word addres = AbsoluteAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->ACC, memory,&cycles,addres);
            break;
        }
        case InsSTAABSX: {
            Word addres = AbsoluteXAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->ACC, memory,&cycles,addres);
            break;
        }
        case InsSTAABSY: {
            Word addres = AbsoluteYAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->ACC, memory,&cycles,addres);
            break;
        }
        case InsSTAINDX: {
            Word addres = indirectXAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->ACC, memory,&cycles,addres);
            break;
        }
        case InsSTAINDY: {
            Word addres = indirectYAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->ACC, memory,&cycles,addres);
            break;
        }
        //Store STX CASES 
        case InsSTXZP: {
            Byte addres = zeroPageAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->X, memory,&cycles,addres);
            break;
        }
        case InsSTXZPY: {
            Byte addres = zeroPageYAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->X, memory,&cycles,addres);
            break;
        }
        case InsSTXABS: {
            Word addres = AbsoluteAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->X, memory,&cycles,addres);
            break;
        }
        //Store STY CASES 
        case InsSTYZP: {
            Byte addres = zeroPageAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->Y, memory,&cycles,addres);
            break;
        }
        case InsSTYZPX: {
            Byte addres = zeroPageXAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->Y, memory,&cycles,addres);
            break;
        }
        case InsSTYABS: {
            Word addres = AbsoluteAdress(cpu,memory,&cycles);
            writeByteInMemoryFromRegister(cpu->Y, memory,&cycles,addres);
            break;
        }
        default:
            printf("\ninstruction not exist\n");
            return;
            break;
        }
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
