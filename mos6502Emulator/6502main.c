
#include <stdio.h>
#include "tests/TestFuncions.h"
#include "tests/testLoad.h"
#include "tests/testStore.h"
#include "tests/testStack.h"
#include "tests/testJmp.h"
#include "tests/testLogic.h"
#include "tests/testTranfers.h"
#include "CPU/cpu6502.h"

int main(void) {
    CPU cpu;
    Memory memory;
    
    startCPUMEMORY(&cpu, &memory);

    int testSelection = 8;

    switch (testSelection)
    {
    case 1: {
        LoadAInstructionsTest(&cpu,&memory);
        break;
    }
    case 2: {
        LoadXInstructionsTest(&cpu,&memory);
        break;
    }
    case 3: {
        LoadYInstructionsTest(&cpu,&memory);
        break;
    }
    case 4: {
        StoreAInstructionsTest(&cpu,&memory);
        break;
    }
    case 5: {
        StoreXInstructionsTest(&cpu,&memory);
        break;
    }
    case 6: {
        StoreYInstructionsTest(&cpu,&memory);
        break;
    }
    case 7: {
        stackOperationsTest(&cpu,&memory);
        jmpTest(&cpu,&memory);
        break;
    }
    case 8: {
        LoadAInstructionsTest(&cpu,&memory);
        LoadXInstructionsTest(&cpu,&memory);
        LoadYInstructionsTest(&cpu,&memory);
        StoreAInstructionsTest(&cpu,&memory);
        StoreXInstructionsTest(&cpu,&memory);
        StoreYInstructionsTest(&cpu,&memory);
        stackOperationsTest(&cpu,&memory);
        jmpTest(&cpu,&memory);
        logicTests(&cpu,&memory);
        testTranfersRegisters(&cpu,&memory);
        break;
    }
    default:
        return 0;
    }

    Byte program[] = {0x00,0x10,0xA9,0xFF,0x85,0x90,0x8D,0x00,0x80,0x49,0xCC,0x4C,0x02,0x10};
    size_t programSize = sizeof(program) / sizeof(program[0]);
    loadProgram(&cpu,&memory,program,programSize);
    cpu.executeI(&cpu,&memory,41);
    return 0; 
}






