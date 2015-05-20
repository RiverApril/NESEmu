
#include <iostream>
#include <fstream>
#include "Chip.h"

using namespace std;

//Use hexcurse to edit the hex file


Chip chip;

void printMemory(unsigned char* memory, unsigned int memoryMin, unsigned int memoryMax){
    for(unsigned int i=memoryMin;i<memoryMax;i++){
        printf("%02x ", chip.memory[i]);
    }
    printf("\n");
}

bool loadFile(const char* path, unsigned char* memory, unsigned int memorySize){
    ifstream file;
    file.open(path, ios::binary | ios::in);
    char buffer[memorySize];
    file.read(buffer, memorySize);
    file.close();
    for(unsigned int i=0;i<memorySize;i++){
        memory[i] = (unsigned char)buffer[i];
    }
    return true;
}

int main(int argc, const char * argv[]) {
    const char* path = "testFile";
    if(argc > 1){
        path = argv[1];
    }
    printf("Loading program: '%s'", path);
    loadFile(path, chip.memory, chip.memorySize);
    int exitCode = EXIT_CODE_NONE;
    while(exitCode == EXIT_CODE_NONE){
        printf("\npc: 0x%x    ", chip.pc);
        printf("A: 0x%x, X: 0x%x, Y: 0x%x    ", chip.A, chip.X, chip.Y);
        printf("C%c V%c Z%c N%c D%c I%c\n", chip.C?'*':'_', chip.V?'*':'_', chip.Z?'*':'_', chip.N?'*':'_', chip.D?'*':'_', chip.I?'*':'_');
        printMemory(chip.memory, 0, 0x20);
        exitCode = chip.executeNextOpcode();
    }
    printf("\nExit Code: %d\n", exitCode);
    return 0;
}
