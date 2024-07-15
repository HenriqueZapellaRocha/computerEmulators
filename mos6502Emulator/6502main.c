#include <stdint.h>
#include <stdio.h>
#define Byte unsigned char //8bits
#define Word unsigned short //16bits
#define u32 uint32_t // 32 bits

//instruction opdcode
const Byte InsLDAIM = 0xA9;
const Byte InsLDAZP = 0xA5;
const Byte InsLDAZPX = 0xB5;
const Byte InsJSRABS = 0X20;

const u32 maxMemorySize = 1024 * 64;

typedef struct Memory {
    Byte Data[maxMemorySize];
    void (*initMemory)(struct Memory *memory);
} Memory;

typedef struct CPU {
    
    Word PC; //Program Counter 16bit
    Byte SP; //Stack Pointer 8bit
    Byte ACC; //accumulator 8bit
    
    Byte X,Y; //registers 8bit
    
    //Status flags
    Byte C : 1; //Carry Flag
    Byte Z : 1; //Zero Flag
    Byte I : 1; //Interrupt Disable
    Byte D : 1; //Decimal Mode
    Byte B : 1; //Break Command
    Byte O : 1; //Overflow Flag
    Byte N : 1; //Negative Flag
    
    void (*reset)(struct CPU *cpu, struct Memory *memory);
    void (*executeI)(struct CPU *cpu, struct Memory *memory, u32 cycles);
    Byte (*fetchInstrucstion)(struct CPU *cpu, struct Memory *memory, u32 *cycles);
    Byte (*readByteInMemory)(struct Memory *Memory, u32 *cycles, u32 adress);
    Word (*fetchWord)(struct CPU *cpu, struct Memory *memory, u32 *cycles);
} CPU;

void reset(CPU *cpu, Memory *memory) {
    cpu->PC = 0x0FFC;
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
        }
        case InsLDAZPX: {
            Byte adress = fetchInstrucstion(cpu,memory,&cycles);
            temp = readByteInMemory(memory, &cycles, adress);
            temp += cpu->X;
            cpu->ACC = temp;
            updateFlagsLDA(cpu);
        }
        case InsJSRABS: {
            Word subAddr = fetchWord(cpu,memory,&cycles);
            writeWord(cpu->PC-1, cpu->SP,memory,&cycles);
            memory->Data[cpu->SP] = cpu->PC - 1; 
            cycles--;
            cpu->PC = subAddr;
            cycles--;
        }
        default:
            break;
        }
    }
}


int main() {
    CPU cpu;
    Memory memory;
    
    //initiate isntrutions pointers
    cpu.reset = reset;
    cpu.executeI = executeI;
    cpu.fetchInstrucstion = fetchInstrucstion;
    cpu.readByteInMemory = readByteInMemory;
    cpu.fetchWord = fetchWord;
    memory.initMemory = initMemory;
    
    //reset cpu and memory
    cpu.reset(&cpu, &memory);
    cpu.X = 10;
    // Exemplo: inserir instrução LDA imediato e valor imediato na memória
    memory.Data[0xFFC] = InsLDAZPX; // LDA imediato opcode
    memory.Data[0XFFD] = 0x42; // Valor imediato para LDA
    memory.Data[0X42] = 10;
    //testing lda immediate
    cpu.executeI(&cpu,&memory,3);

    
    
    //verify the values
    printf("CPU PC: %04X\n", cpu.PC);
    printf("CPU SP: %02X\n", cpu.SP);
    printf("CPU ACC: %0X\n", cpu.ACC);
    printf("Memory[0]: %02X\n", memory.Data[0]);
    
    return 0;
}