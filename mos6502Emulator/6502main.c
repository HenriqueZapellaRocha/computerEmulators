#include <stdint.h>
#include <stdio.h>
#define Byte unsigned char //8bits
#define Word unsigned short //16bits
#define u32 uint32_t // 32 bits

//instruction opdcode
const Byte InsLDAIM = 0xA9;

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
    Byte (*fetchInstrucstion)(struct CPU *cpu, struct Memory *memory, u32 cycles);
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

Byte fetchInstrucstion(CPU *cpu, Memory *memory, u32 cycles) {
    Byte Data = memory->Data[cpu->PC];
    cpu->PC++;
    cycles--;
    return Data;
}

void executeI(CPU *cpu, Memory *memory, u32 cycles) {
    while(cycles > 0) {
        Byte opcode = fetchInstrucstion(cpu, memory, cycles);
        Byte temp;
        switch (opcode)
        {
        case InsLDAIM: {
            temp = fetchInstrucstion(cpu, memory, cycles);
            cpu->ACC = temp;
            if(temp==0) {
                cpu->Z = 0x1;
            }
            if((temp & 0b10000000) > 0) {
                cpu->N = 0x1;
            }
            cycles--;
            break;
        }
        default:
            break;
        }
        cycles--;
    }
}


int main() {
    CPU cpu;
    Memory memory;
    
    //initiate isntrutions pointers
    cpu.reset = reset;
    cpu.executeI = executeI;
    cpu.fetchInstrucstion = fetchInstrucstion;
    memory.initMemory = initMemory;
    
    //reset cpu and memory
    cpu.reset(&cpu, &memory);

        // Exemplo: inserir instrução LDA imediato e valor imediato na memória
    memory.Data[0xFFC] = InsLDAIM; // LDA imediato opcode
    memory.Data[0XFFD] = 10; // Valor imediato para LDA
    //testing lda immediate
    cpu.executeI(&cpu,&memory,2);

    
    
    //verify the values
    printf("CPU PC: %04X\n", cpu.PC);
    printf("CPU SP: %02X\n", cpu.SP);
    printf("Memory[0]: %02X\n", memory.Data[0]);
    
    return 0;
}