#include "cpu6502.h"
#include <stdio.h>

//LOAD OPCODES
//LDA
const Byte InsLDAIM = 0xA9;// LDA load Immediate
const Byte InsLDAZP = 0xA5;// LDA zero page
const Byte InsLDAZPX = 0xB5;// LDA zero page X
const Byte InsLDAABS = 0xAD;//LAD Absolute
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

//STACK OPERATIONS OPCODES
const Byte InsJSRABS = 0x20;// JSR absolute
const Byte InsRTSIMP = 0x60;//RTS IMPLIED 
const Byte InsTSX = 0xBA;//TSX STACK POINTER TO X 
const Byte InsTXS = 0x9A;//TXS X to Stack pointer. Inverse of tsx
const Byte InsPHA = 0x48;//PHA
const Byte InsPHP = 0x08;//PHP
const Byte InsPLA = 0x68;//PLA
const Byte InsPLP = 0x28;//PLP

//JMP
const Byte InsJMPABS = 0x4C;//JMP ABSOLUTE
const Byte InsJMPIND = 0x6C;//JMP INDIRECT
//Logic 
//AND 
const Byte InsANDIM = 0x29;//AND immediate
const Byte InsANDZP = 0x25;//AND Zero Page
const Byte InsANDZPX = 0x35;//AND Zero Page,X
const Byte InsANDABS = 0x2D;//AND Absolute
const Byte InsANDABSX = 0x3D;//AND Absolute,X
const Byte InsANDABSY = 0x39;//AND Absolute,Y
const Byte InsANDINDX = 0x21;//AND (Indirect,X)
const Byte InsANDINDY = 0x31;//AND (Indirect),Y
//EOR
const Byte InsEORIM = 0x49;//EOR immediate
const Byte InsEORZP = 0x45;//EOR Zero Page
const Byte InsEORZPX = 0x55;//EOR Zero Page,X
const Byte InsEORABS = 0x4D;//EOR Absolute
const Byte InsEORABSX = 0x5D;//EOR Absolute,X
const Byte InsEORABSY = 0x59;//EOR Absolute,Y
const Byte InsEORINDX = 0x41;//EOR (Indirect,X)
const Byte InsEORINDY = 0x51;//EOR (Indirect),Y
//ORA
const Byte InsORAIM = 0x09;//ORA immediate
const Byte InsORAZP = 0x05;//ORA Zero Page
const Byte InsORAZPX = 0x15;//ORA Zero Page,X
const Byte InsORAABS = 0x0D;//ORA Absolute
const Byte InsORAABSX = 0x1D;//ORA Absolute,X
const Byte InsORAABSY = 0x19;//ORA Absolute,Y
const Byte InsORAINDX = 0x01;//ORA (Indirect,X)
const Byte InsORAINDY = 0x11;//ORA (Indirect),Y
const Byte InsBITZP = 0x24;//BIT Zero Page
const Byte InsBITABS = 0x2C;//BIT Absolute

//Register Transfers
const Byte InsTAX = 0xAA;//TAX
const Byte InsTAY = 0xA8;//TAY
const Byte InsTXA = 0x8A;//TXA
const Byte InsTYA = 0x98;//TYA
//Increment Decrement
const Byte InsINCZP = 0xE6;//INC, Zero Page
const Byte InsINCZPX = 0xF6;//INC, Zero Page,X 
const Byte InsINCABS = 0xEE;//INC, Absolute
const Byte InsINCABSX = 0xFE;//INC, Absolute,X
const Byte InsINX = 0xE8;//INX
const Byte InsINY = 0xC8;//INY
const Byte InsDECZP = 0xc6;//DEC, Zero Page
const Byte InsDECZPX = 0xd6;//DEC, Zero Page,X 
const Byte InsDECABS = 0xCE;//DEC, Absolute
const Byte InsDECABSX = 0xDE;//DEC, Absolute,X
const Byte InsDEX = 0xCA;//DEX
const Byte InsDEY = 0x88;//DEY

const u32 maxMemorySize = 1024 * 64;


void reset(CPU *cpu, Memory *memory,Word pc) {
    if(pc == 0) {
    cpu->PC = 0x0FFFC;
    } else {
    cpu->PC = pc;
    }
    cpu->SP = 0xFF;
    cpu->ACC = cpu->X = cpu->Y = 0;
    cpu->status.bits.C = cpu->status.bits.Z = cpu->status.bits.I = cpu->status.bits.D = cpu->status.bits.B = cpu->status.bits.V = cpu->status.bits.N = 0;
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
        (*cycles)-=2;
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

void updateFlagsLOAD(Byte regist, CPU *cpu) {
    if(regist==0) {
        cpu->status.bits.Z = 0x1;
    }
    if((regist & 0b10000000) > 0) {
        cpu->status.bits.N = 0x1;
    }
}

void updateFlagsBIT(Byte value1,Byte value2, CPU *cpu) {
    if((value1 & value2) == 0) {
        cpu->status.bits.Z = 0x1;
    }
    if((value2 & 0b10000000) > 0) {
        cpu->status.bits.N = 0x1;
    }
    if((value2 & 0b01000000) > 0) {
        cpu->status.bits.V = 0x1;
    }
}

void writeByte(Byte regis, Memory *memory,u32 *cycles, Word adress) {
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
//handling sta absolute x is the unique who ever consume 5 cycles
Word AbsoluteXAdress5(CPU *cpu, Memory *memory,u32 *cycles) {
    Word adress = fetchWord(cpu,memory,cycles);
    Word adressX = adress + cpu->X;
    (*cycles)--;
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
//handling sta absolute y is the unique who ever consume 5 cycles
Word AbsoluteYAdress5(CPU *cpu, Memory *memory,u32 *cycles) {
    Word adress = fetchWord(cpu,memory,cycles);
    Word adressX = adress + cpu->Y;
    (*cycles)--;
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
    if ((adress & 0xFF00) != (efectiveadress& 0xFF00)) {
    (*cycles)--;
    }
    return efectiveadress;
}
//handling sta indirect,Y is the unique who ever consume 6 cycles
Word indirectYAdress6(CPU *cpu, Memory *memory,u32 *cycles) {
    Byte zeroPageValue = fetchInstrucstion(cpu,memory,cycles);
    Word adress = readWord(memory,cycles,zeroPageValue);
    Word efectiveadress = adress + cpu->Y;
    (*cycles)--;
    return efectiveadress;
}

//program counter to stack
void PcToStack(CPU *cpu, Memory *memory, u32 *cycles) {
    Word stackAddress = 0x0100 | cpu->SP; //SP in word for the 1st page
    writeWord(cpu->PC-1, stackAddress-1,memory,cycles);
    cpu->SP -= 2;
}

//accumulator to stack
void pushByteToStack(CPU *cpu, Memory *memory, u32 *cycles,Byte value) {
    Word stackAddress = 0x0100 | cpu->SP; //SP in word for the 1st page
    writeByte(value,memory,cycles,stackAddress);
    cpu->SP -=1;
    (*cycles)--;
}

//pop things from stack
Byte popByteStack(Memory *memory, u32 *cycles, CPU *cpu) {
    Word stackAddress = 0x0100 | cpu->SP; //SP in word for the 1st page
    Byte value = readByteInMemory(memory,cycles,stackAddress+1);
    cpu->SP++;
    (*cycles)--;
    return value;
}


//pop things from stack
Word popWordStack(Memory *memory, u32 *cycles, CPU *cpu) {
    Word stackAddress = 0x0100 | cpu->SP; //SP in word for the 1st page
    Word returnAddr = readWord(memory,cycles,stackAddress+1);
    cpu->SP += 2;
    (*cycles)--;
    return returnAddr;
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
            updateFlagsLOAD(cpu->ACC, cpu);
            break;
        }
        case InsLDAZP: {
            cpu->ACC = zeroPageValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->ACC, cpu);
            break;
        }
        case InsLDAZPX: {
            cpu->ACC = zeroPageXValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->ACC, cpu);
            break;
        }
        case InsLDAABS: {
            cpu->ACC = absoluteValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->ACC, cpu);
            break;
        }
        case InsLDAABSX: {
            cpu->ACC = absoluteXValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->ACC, cpu);
            break;
        }
        case InsLDAABSY: {
            cpu->ACC = absoluteYValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->ACC, cpu);
            break;
        }
        case InsLDAINDX: {
            cpu->ACC = indirectXValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->ACC, cpu);
            break;
        }
        case InsLDAINDY: {
            cpu->ACC = indirectYValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->ACC, cpu);
            break;
        }
        //LOAD LDX CASES
        case InsLDXIM: {
            temp = fetchInstrucstion(cpu, memory,&cycles);
            cpu->X = temp;
            updateFlagsLOAD(cpu->X, cpu);
            break;
        }
        case InsLDXZP: {
            cpu->X = zeroPageValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->X, cpu);
            break;
        }
        case InsLDXZPY: {
            cpu->X = zeroPageYValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->X, cpu);
            break;
        }
        case InsLDXABS: {
            cpu->X =  absoluteValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->X, cpu);
            break;
        }
        case InsLDXABSY: {
            cpu->X = absoluteYValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->X, cpu);
            break;
        }
        //LOAD LDY CASES
        case InsLDYIM: {
            temp = fetchInstrucstion(cpu, memory,&cycles);
            cpu->Y = temp;
            updateFlagsLOAD(cpu->Y, cpu);
            break;
        }
        case InsLDYZP: {
            cpu->Y = zeroPageValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->Y, cpu);
            break;
        }
        case InsLDYZPX: {
            cpu->Y =zeroPageXValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->Y, cpu);
            break;
        }
        case InsLDYABS: {
            cpu->Y =  absoluteValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->Y, cpu);
            break;
        }
        case InsLDYABSX: {
            cpu->Y = absoluteXValue(cpu,memory,&cycles);
            updateFlagsLOAD(cpu->Y, cpu);
            break;
        }
        //Store STA CASES 
        case InsSTAZP: {
            Byte addres = zeroPageAdress(cpu,memory,&cycles);
            writeByte(cpu->ACC, memory,&cycles,addres);
            break;
        }
        case InsSTAZPX: {
            Byte addres = zeroPageXAdress(cpu,memory,&cycles);
            writeByte(cpu->ACC, memory,&cycles,addres);
            break;
        }
        case InsSTAABS: {
            Word addres = AbsoluteAdress(cpu,memory,&cycles);
            writeByte(cpu->ACC, memory,&cycles,addres);
            break;
        }
        case InsSTAABSX: {
            Word addres = AbsoluteXAdress5(cpu,memory,&cycles);
            writeByte(cpu->ACC, memory,&cycles,addres);
            break;
        }
        case InsSTAABSY: {
            Word addres = AbsoluteYAdress5(cpu,memory,&cycles);
            writeByte(cpu->ACC, memory,&cycles,addres);
            break;
        }
        case InsSTAINDX: {
            Word addres = indirectXAdress(cpu,memory,&cycles);
            writeByte(cpu->ACC, memory,&cycles,addres);
            break;
        }
        case InsSTAINDY: {
            Word addres = indirectYAdress6(cpu,memory,&cycles);
            writeByte(cpu->ACC, memory,&cycles,addres);
            break;
        }
        //Store STX CASES 
        case InsSTXZP: {
            Byte addres = zeroPageAdress(cpu,memory,&cycles);
            writeByte(cpu->X, memory,&cycles,addres);
            break;
        }
        case InsSTXZPY: {
            Byte addres = zeroPageYAdress(cpu,memory,&cycles);
            writeByte(cpu->X, memory,&cycles,addres);
            break;
        }
        case InsSTXABS: {
            Word addres = AbsoluteAdress(cpu,memory,&cycles);
            writeByte(cpu->X, memory,&cycles,addres);
            break;
        }
        //Store STY CASES 
        case InsSTYZP: {
            Byte addres = zeroPageAdress(cpu,memory,&cycles);
            writeByte(cpu->Y, memory,&cycles,addres);
            break;
        }
        case InsSTYZPX: {
            Byte addres = zeroPageXAdress(cpu,memory,&cycles);
            writeByte(cpu->Y, memory,&cycles,addres);
            break;
        }
        case InsSTYABS: {
            Word addres = AbsoluteAdress(cpu,memory,&cycles);
            writeByte(cpu->Y, memory,&cycles,addres);
            break;
        }
        //STACK operations
        //JSR INSTRUCTION 
        case InsJSRABS: {
            Word subAddr = fetchWord(cpu,memory,&cycles);
            PcToStack(cpu,memory,&cycles); //puting the retun adress in the stack
            cpu->PC = subAddr;
            cycles--;
            break;
        }
        //RTS INSTRUCTION 
        case InsRTSIMP: {
            Word adress = popWordStack(memory,&cycles,cpu);
            cpu->PC = adress+1;
            cycles -=2;
            break;
        }
        //TSX INSTRUCTION 
        case InsTSX: {
            cpu->X = cpu->SP;
            updateFlagsLOAD(cpu->X, cpu); //reusing de load update flages beacause it affects the same flags 
            cycles--;
            break;
        }
        //TXS INSTRUCTION 
        case InsTXS: {
            cpu->SP = cpu->X;
            cycles--;
            break;
        }
        //PHA INSTRUCTION 
        case InsPHA: {
            pushByteToStack(cpu,memory,&cycles,cpu->ACC);
            break;
        }
        //PHP INSTRUCTION 
        case InsPHP: {
            pushByteToStack(cpu,memory,&cycles,cpu->status.byte);
            break;
        }
        //PLA INSTRUCTION 
        case InsPLA: {
            cpu->ACC = popByteStack(memory,&cycles,cpu);
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            cycles--;
            break;
        }
        //PLP INSTRUCTION 
        case InsPLP: {
            cpu->status.byte = popByteStack(memory,&cycles,cpu);
            cycles--;
            break;
        }
        //JMP Absolute
        case InsJMPABS: {
            cpu->PC = AbsoluteAdress(cpu,memory,&cycles);
            break;
        }
        //JMP Indirect
        case InsJMPIND: {
            cpu->PC = AbsoluteAdress(cpu,memory,&cycles);
            cpu->PC = readWord(memory,&cycles,cpu->PC);
            break;
        }
        //AND CASES
        case InsANDIM: {
            cpu->ACC = fetchInstrucstion(cpu,memory,&cycles) & cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsANDZP: {
            cpu->ACC = zeroPageValue(cpu,memory,&cycles) & cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsANDZPX: {
            cpu->ACC = zeroPageXValue(cpu,memory,&cycles) & cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsANDABS: {
            cpu->ACC = absoluteValue(cpu,memory,&cycles) & cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsANDABSX: {
            cpu->ACC = absoluteXValue(cpu,memory,&cycles) & cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsANDABSY: {
            cpu->ACC = absoluteYValue(cpu,memory,&cycles) & cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsANDINDX: {
            cpu->ACC = indirectXValue(cpu,memory,&cycles) & cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsANDINDY: {
            cpu->ACC = indirectYValue(cpu,memory,&cycles) & cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        //EOR CASES
        case InsEORIM: {
            cpu->ACC = fetchInstrucstion(cpu,memory,&cycles) ^ cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsEORZP: {
            cpu->ACC = zeroPageValue(cpu,memory,&cycles) ^ cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsEORZPX: {
            cpu->ACC = zeroPageXValue(cpu,memory,&cycles) ^ cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsEORABS: {
            cpu->ACC = absoluteValue(cpu,memory,&cycles) ^ cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsEORABSX: {
            cpu->ACC = absoluteXValue(cpu,memory,&cycles) ^ cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsEORABSY: {
            cpu->ACC = absoluteYValue(cpu,memory,&cycles) ^ cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsEORINDX: {
            cpu->ACC = indirectXValue(cpu,memory,&cycles) ^ cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsEORINDY: {
            cpu->ACC = indirectYValue(cpu,memory,&cycles) ^ cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        //ORA CASES
        case InsORAIM: {
            cpu->ACC = fetchInstrucstion(cpu,memory,&cycles) | cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsORAZP: {
            cpu->ACC = zeroPageValue(cpu,memory,&cycles) | cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsORAZPX: {
            cpu->ACC = zeroPageXValue(cpu,memory,&cycles) | cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsORAABS: {
            cpu->ACC = absoluteValue(cpu,memory,&cycles) | cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsORAABSX: {
            cpu->ACC = absoluteXValue(cpu,memory,&cycles) | cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsORAABSY: {
            cpu->ACC = absoluteYValue(cpu,memory,&cycles) | cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsORAINDX: {
            cpu->ACC = indirectXValue(cpu,memory,&cycles) | cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsORAINDY: {
            cpu->ACC = indirectYValue(cpu,memory,&cycles) | cpu->ACC;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsBITZP: {
            updateFlagsBIT(cpu->ACC, zeroPageValue(cpu,memory,&cycles),cpu);
            break;
        }
        case InsBITABS: {
            updateFlagsBIT(cpu->ACC, absoluteValue(cpu,memory,&cycles),cpu);
            break;
        }
        //Transfer register CASES
        case InsTAX: {
            cpu->X = cpu->ACC;
            cycles--;
            updateFlagsLOAD(cpu->X, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsTAY: {
            cpu->Y = cpu->ACC;
            cycles--;
            updateFlagsLOAD(cpu->Y, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsTXA: {
            cpu->ACC = cpu->X;
            cycles--;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsTYA: {
            cpu->ACC = cpu->Y;
            cycles--;
            updateFlagsLOAD(cpu->ACC, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        //Increment Decrement Cases
        case InsINCZP: {
            Byte adress = zeroPageAdress(cpu,memory,&cycles);
            Byte value = readByteInMemoryZeroPage(memory,&cycles,adress)+1;
            cycles--; //from the adding 1 
            writeByte(value,memory,&cycles,adress);
            updateFlagsLOAD(memory->Data[adress], cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsINCZPX: {
            Byte adress = zeroPageXAdress(cpu,memory,&cycles);
            Byte value = readByteInMemoryZeroPage(memory,&cycles,adress)+1;
            cycles--; //from the adding 1 
            writeByte(value,memory,&cycles,adress);
            updateFlagsLOAD(memory->Data[adress], cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsINCABS: {
            Word adress = AbsoluteAdress(cpu,memory,&cycles);
            Byte value = readByteInMemory(memory,&cycles,adress)+1;
            cycles--; //from the adding 1 
            writeByte(value,memory,&cycles,adress);
            updateFlagsLOAD(memory->Data[adress], cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsINCABSX: {
            Word adress = AbsoluteXAdress5(cpu,memory,&cycles);
            Byte value = readByteInMemory(memory,&cycles,adress)+1;
            cycles--; //from the adding 1 
            writeByte(value,memory,&cycles,adress);
            updateFlagsLOAD(memory->Data[adress], cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsINX: {
            cpu->X +=1;
            cycles--;
            updateFlagsLOAD(cpu->X, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsINY: {
            cpu->Y +=1;
            cycles--;
            updateFlagsLOAD(cpu->Y, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsDECZP: {
            Byte adress = zeroPageAdress(cpu,memory,&cycles);
            Byte value = readByteInMemoryZeroPage(memory,&cycles,adress)-1;
            cycles--; //from the adding 1 
            writeByte(value,memory,&cycles,adress);
            updateFlagsLOAD(memory->Data[adress], cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsDECZPX: {
            Byte adress = zeroPageXAdress(cpu,memory,&cycles);
            Byte value = readByteInMemoryZeroPage(memory,&cycles,adress)-1;
            cycles--; //from the adding 1 
            writeByte(value,memory,&cycles,adress);
            updateFlagsLOAD(memory->Data[adress], cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsDECABS: {
            Word adress = AbsoluteAdress(cpu,memory,&cycles);
            Byte value = readByteInMemory(memory,&cycles,adress)-1;
            cycles--; //from the adding 1 
            writeByte(value,memory,&cycles,adress);
            updateFlagsLOAD(memory->Data[adress], cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsDECABSX: {
            Word adress = AbsoluteXAdress5(cpu,memory,&cycles);
            Byte value = readByteInMemory(memory,&cycles,adress)-1;
            cycles--; //from the adding 1 
            writeByte(value,memory,&cycles,adress);
            updateFlagsLOAD(memory->Data[adress], cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsDEX: {
            cpu->X -=1;
            cycles--;
            updateFlagsLOAD(cpu->X, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        case InsDEY: {
            cpu->Y -=1;
            cycles--;
            updateFlagsLOAD(cpu->Y, cpu); //reusing de load update flages beacause it affects the same flags 
            break;
        }
        default:
            printf("\ninstruction not exist\n");
            return;
            break;
        }
    }
}

void loadProgram(CPU *cpu, Memory *memory,Byte *program,size_t programSize) {
    //collect the start of the program 
    Byte low = program[0];
    Word high = program[1] << 8;
    Word initialAdress = high | low;
    cpu->PC = initialAdress;
    //putting the rest of the progam in memeory
    Word memoryC = initialAdress;
    for(Word i =2; i < programSize;i++) {
        memory->Data[memoryC] = program[i];
        memoryC++;
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
    (*cpu).reset(cpu,memory,0);
}
