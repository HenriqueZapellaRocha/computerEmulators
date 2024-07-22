
#include <stdio.h>
#include "TestFuncions.h"
#include "tests/testLoad.h"
#include "tests/testStore.h"
#include "tests/testStack.h"


int main(void) {
    CPU cpu;
    Memory memory;
    
    startCPUMEMORY(&cpu, &memory);

    int testSelection = 1;

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
        break;
    }
    default:
        return 0;
    }

    return 0; 
}






