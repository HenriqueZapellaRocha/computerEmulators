#ifndef CPU6502_H
#define CPU6502_H
#include <stdint.h>
#include <stdio.h>

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
extern const Byte InsRTSIMP;//RTS IMPLIED 
extern const Byte InsJMPABS;//JMP ABSOLUTE
extern const Byte InsJMPIND;//JMP INDIRECT
extern const Byte InsTSX;//TSX STACK POINTER TO X 
extern const Byte InsTXS;//TXS X to Stack pointer. Inverse of tsx
extern const Byte InsPHA;//PHA
extern const Byte InsPHP;//PHP
extern const Byte InsPLA;//PLA
extern const Byte InsPLP;//PLP
extern const Byte InsANDIM;//AND immediate
extern const Byte InsANDIM;//AND immediate
extern const Byte InsANDZP;//AND Zero Page
extern const Byte InsANDZPX;//AND Zero Page,X
extern const Byte InsANDABS;//AND Absolute
extern const Byte InsANDABSX;//AND Absolute,X
extern const Byte InsANDABSY;//AND Absolute,Y
extern const Byte InsANDINDX;//AND (Indirect,X)
extern const Byte InsANDINDY;//AND (Indirect),Y
extern const Byte InsEORIM;//EOR immediate
extern const Byte InsEORZP;//EOR Zero Page
extern const Byte InsEORZPX;//EOR Zero Page,X
extern const Byte InsEORABS;//EOR Absolute
extern const Byte InsEORABSX;//EOR Absolute,X
extern const Byte InsEORABSY;//EOR Absolute,Y
extern const Byte InsEORINDX;//EOR (Indirect,X)
extern const Byte InsEORINDY;//EOR (Indirect),Y
extern const Byte InsORAIM;//ORA immediate
extern const Byte InsORAZP;//ORA Zero Page
extern const Byte InsORAZPX;//ORA Zero Page,X
extern const Byte InsORAABS;//ORA Absolute
extern const Byte InsORAABSX;//ORA Absolute,X
extern const Byte InsORAABSY;//ORA Absolute,Y
extern const Byte InsORAINDX;//ORA (Indirect,X)
extern const Byte InsORAINDY;//ORA (Indirect),Y
extern const Byte InsBITZP;//BIT Zero Page
extern const Byte InsBITABS;//BIT Absolute
extern const Byte InsTAX;//TAX
extern const Byte InsTAY;//TAY
extern const Byte InsTSX;//TSX
extern const Byte InsTXA;//TXA
extern const Byte InsTXS;//TXS
extern const Byte InsTYA;//TYA
extern const Byte InsINCZP;//INC, Zero Page
extern const Byte InsINCZPX;//INC, Zero Page,X 
extern const Byte InsINCABS;//INC, Absolute
extern const Byte InsINCABSX;//INC, Absolute,X
extern const Byte InsINX;//INX
extern const Byte InsINY;//INY
extern const Byte InsDECZP;//DEC, Zero Page
extern const Byte InsDECZPX;//DEC, Zero Page,X 
extern const Byte InsDECABS;//DEC, Absolute
extern const Byte InsDECABSX;//DEC, Absolute,X
extern const Byte InsDEX;//DEX
extern const Byte InsDEY ;//DEY
extern const Byte InsBEQ;//BEQ
extern const Byte InsBCC ;//BCC
extern const Byte InsBCS;//BCS
extern const Byte InsBMI;//BMI
extern const Byte InsBNE;//BNE
extern const Byte InsBVC;//BVC
extern const Byte InsBPL;//BPL
extern const Byte InsBVS;//BVS
extern const Byte InsCLC;//CLC 
extern const Byte InsCLD;//CLD
extern const Byte InsCLC;//CLC 
extern const Byte InsCLI;//CLI
extern const Byte InsCLV;//CLV
extern const Byte InsSEC;//SEC
extern const Byte InsSED;//SED
extern const Byte InsSEI;//SEI
extern const Byte InsNOP;//NOP
extern const Byte InsADCIM;//ADC immediate

typedef struct Memory {
    Byte Data[1024 * 64];
    void (*initMemory)(struct Memory *memory);
} Memory;

typedef union {
    struct {
    unsigned char C : 1; // Carry Flag
    unsigned char Z : 1; // Zero Flag
    unsigned char I : 1; // Interrupt Disable
    unsigned char D : 1; // Decimal Mode
    unsigned char B : 1; // Break Command
    unsigned char V : 1; // Overflow Flag
    unsigned char N : 1; // Negative Flag
    } bits;
    Byte byte;
} ProcessorStatus;


typedef struct CPU {
    
    Word PC; //Program Counter 16bit
    Byte SP; //Stack Pointer 8bit
    Byte ACC; //accumulator 8bit
    
    Byte X,Y; //registers 8bit
    
    ProcessorStatus status; //flags
    
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
void updateFlagsLOAD(Byte regis, CPU *cpu);
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
void pushByteToStack(CPU *cpu, Memory *memory, u32 *cycles,Byte value);
void writeByte(Byte regis, Memory *memory,u32 *cycles, Word adress);
Byte popByteStack(Memory *memory, u32 *cycles, CPU *cpu);
void loadProgram(CPU *cpu, Memory *memory,Byte *program,size_t programSize);
void branch(Byte flagStatus,Byte equal, CPU *cpu, Memory *memory, u32 *cycles);
#endif // CPU6502_H
