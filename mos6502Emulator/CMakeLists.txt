cmake_minimum_required(VERSION 3.10)


project(MOS6502Emulators)


set(CMAKE_C_STANDARD 99)


include_directories(${PROJECT_SOURCE_DIR})

# adding the archives
set(SOURCES
    6502main.c
    cpu6502.c
    TestFuncions.c
    tests/testLoad.c
    tests/testStack.c
    tests/testStore.c
)


# Create executable
add_executable(mos6502emulator ${SOURCES})


