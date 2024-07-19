#define CPU6502_H
#include <stdint.h>

#define Byte unsigned char //8bits
#define Word unsigned short //16bits
#define u32 uint32_t // 32 bits

//instruction opdcode
extern const Byte InsLDAIM; //LAD load imediatly
extern const Byte InsLDAZP; // LAD zero page
extern const Byte InsLDAZPX; //Zero page x
extern const Byte InsJSRABS; //JSR absolute
extern const Byte InsLDAABS; //LAD Absolute
extern const Byte InsLDAABSX;//LAD Absolute,X
extern const Byte InsLDAABSY;//LAD absolute,Y
extern const Byte InsLDAINDX;//LAD Indirect,X
extern const Byte InsLDAINDY;//LAD Indirect,Y
extern const Byte InsLDXIM;//LDX Immediate
extern const Byte InsLDXZP;//LDX Zero Page
extern const Byte InsLDXZPY;//LDX Zero Page,Y
extern const Byte InsLDXABS; //LDX Absolute
extern const Byte InsLDXABSY;//LDX Absolute,Y
extern const Byte InsLDYIM;//LDY Immediate
extern const Byte InsLDYZP;//LDY Zero Page
extern const Byte InsLDYZPX;//LDY Zero Page,X
extern const Byte InsLDYABS; //LDY Absolute
extern const Byte InsLDYABSX; //LDY Absolute,X
extern const Byte InsSTAZP;//STA Zero Page
extern const Byte InsSTAZPX;//STA Zero Page,X
extern const Byte InsSTAABS;//STA Absolute
extern const Byte InsSTAABSX;//STA Absolute,X
extern const Byte InsSTAABSY;//STA Absolute,Y
extern const Byte InsSTAINDX;//STA Indirect,X
extern const Byte InsSTAINDY;//STA Indirect,Y
extern const Byte InsSTXZP;//STX Zero Page
extern const Byte InsSTXZPY;//STX Zero Page,Y
extern const Byte InsSTXABS;//STX Absolute
extern const Byte InsSTYZP;//STY Zero Page
extern const Byte InsSTYZPX;//STY Zero Page,X
extern const Byte InsSTYABS;//STX Absolute
const Byte InsRTSIMP;//RTS IMPLIED 

typedef struct Memory {
    Byte Data[1024 * 64];
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
    
    void (*reset)(struct CPU *cpu, struct Memory *memory, Word pc);
    void (*executeI)(struct CPU *cpu, struct Memory *memory, u32 cycles);
    Byte (*fetchInstrucstion)(struct CPU *cpu, struct Memory *memory, u32 *cycles);
    Byte (*readByteInMemoryZeroPage)(struct Memory *Memory, u32 *cycles, Byte adress);
    Word (*fetchWord)(struct CPU *cpu, struct Memory *memory, u32 *cycles);
    Byte (*readByteInMemory)(struct Memory *memory, u32 *cycles, Word adrress);
} CPU;

void reset(CPU *cpu, Memory *memory,Word pc);
void initMemory(Memory *memory);
void writeWord(Word data, u32 address, Memory *memory, u32 *cycles);
Word fetchWord(CPU *cpu, Memory *memory, u32 *cycles);
Byte fetchInstrucstion(CPU *cpu, Memory *memory, u32 *cycles);
void updateFlagsLOAD(CPU *cpu);
void executeI(CPU *cpu, Memory *memory, u32 cycles);
void startCPUMEMORY(CPU *cpu, Memory *memory);
Byte readByteInMemoryZeroPage(Memory *memory, u32 *cycles, Byte adrress);
Byte readByteInMemory(Memory *memory, u32 *cycles, Word adrress);
Word readWord(Memory *memory, u32 *cycles, Word adrress);
Byte absoluteYValue(CPU *cpu, Memory *memory,u32 *cycles);
Byte absoluteXValue(CPU *cpu, Memory *memory,u32 *cycles);
Byte absoluteValue(CPU *cpu, Memory *memory,u32 *cycles);
Byte zeroPageYValue(CPU *cpu, Memory *memory,u32 *cycles);
Byte zeroPageValue(CPU *cpu, Memory *memory,u32 *cycles);
Byte zeroPageXValue(CPU *cpu, Memory *memory,u32 *cycles);
Word AbsoluteYAdress5(CPU *cpu, Memory *memory,u32 *cycles);
Word AbsoluteYAdress(CPU *cpu, Memory *memory,u32 *cycles);
Word AbsoluteXAdress5(CPU *cpu, Memory *memory,u32 *cycles);
Word AbsoluteXAdress(CPU *cpu, Memory *memory,u32 *cycles);
Byte zeroPageYAdress(CPU *cpu, Memory *memory,u32 *cycles);
Byte zeroPageXAdress(CPU *cpu, Memory *memory,u32 *cycles);
Byte zeroPageAdress(CPU *cpu, Memory *memory,u32 *cycles);
