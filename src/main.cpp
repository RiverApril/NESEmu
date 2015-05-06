
#include <iostream>
#include <fstream>
#include "Chip.h"

using namespace std;

//Use hexcurse to edit the hex file


Chip chip;

void printMemory(unsigned char* memory, unsigned short memoryMin, unsigned short memoryMax){
    for(int i=memoryMin;i<memoryMax;i++){
        printf("%02x ", chip.memory[i]);
    }
    printf("\n");
}

bool loadFile(const char* path, unsigned char* memory, unsigned short memorySize){
    ifstream file;
    file.open(path, ios::binary | ios::in);
    char buffer[memorySize];
    file.read(buffer, memorySize);
    file.close();
    for(unsigned short i=0;i<memorySize;i++){
        memory[i] = (unsigned char)buffer[i];
    }
    //memory = reinterpret_cast<unsigned char*>(buffer);
}

int main(int argc, const char * argv[]) {
    loadFile("testFile", chip.memory, chip.memorySize);
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
