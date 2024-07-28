#include "testBrach.h"
#include "../CPU/cpu6502.h"
#include <stdio.h>
#include "TestFuncions.h"

void branchTests(CPU *cpu, Memory *memory) {

    cpu->reset(cpu,memory,0xFF00);
}
