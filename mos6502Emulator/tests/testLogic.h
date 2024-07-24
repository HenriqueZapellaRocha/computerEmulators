#define testLogic_h
#include "../CPU/cpu6502.h"

enum ops {
    AND,
    EOR,
    ORA
};


void logicTests(CPU *cpu, Memory *memory);
void bitTests(CPU *cpu, Memory *memory);