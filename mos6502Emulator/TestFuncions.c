#include "TestFuncions.h"
#include <stdio.h>

#define RED_BG "\033[41m"
#define GREEN_BG "\033[42m"
#define RESET "\033[0m"

void assertEqual(int testValue, int ExpectedValue){

    if(testValue == ExpectedValue) {

        printf(" "GREEN_BG "passed" RESET);
    } else {
        printf(" "RED_BG "failed" RESET);
    }
}
