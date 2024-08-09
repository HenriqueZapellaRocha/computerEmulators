cmake_minimum_required(VERSION 3.10)


project(MOS6502Emulators)


set(CMAKE_C_STANDARD 99)


include_directories(${PROJECT_SOURCE_DIR})

# adding the archives
set(SOURCES
    6502main.c
    CPU/cpu6502.c
    tests/TestFuncions.c
    tests/testLoad.c
    tests/testStack.c
    tests/testStore.c
    tests/testJmp.c
    tests/testLogic.c
    tests/testTranfers.c
    tests/testIncDc.c
    tests/testBrach.c
    tests/testAritimetic.c
)


# Create executable
add_executable(mos6502emulator ${SOURCES})


