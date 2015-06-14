
#include <iostream>
#include <fstream>
#include "Chip.hpp"

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
    if(file){
        char buffer[memorySize];
        file.read(buffer, memorySize);
        file.close();
        for(unsigned int i=0;i<memorySize;i++){
            memory[i] = (unsigned char)buffer[i];
        }
        return true;
    }else{
        return false;
    }
}

int main(int argc, const char * argv[]) {
    const char* runPath = "";
    const char* compilePath = "";
    if(argc > 1){
        runPath = argv[1];
    }
    if(compilePath[0] != '\0'){
        
    }
    if(runPath[0] != '\0'){
        
    }
    printf("Loading program: '%s'\n", runPath);
    bool success = loadFile(runPath, chip.memory, chip.memorySize);
    if(success){
        printf("Program loaded, running.\n");
    }else{
        printf("Failed to load program.\n");
    }
    try{
        while(true) {
            printf("\npc: 0x%x    ", chip.pc);
            printf("A: 0x%x, X: 0x%x, Y: 0x%x    ", chip.A, chip.X, chip.Y);
            printf("C%c V%c Z%c N%c D%c I%c\n", chip.C?'*':'_', chip.V?'*':'_', chip.Z?'*':'_', chip.N?'*':'_', chip.D?'*':'_', chip.I?'*':'_');
            printMemory(chip.memory, 0, 0x20);
            chip.executeNextOpcode();
        }
    }catch(int EXIT_CODE){
        switch (EXIT_CODE) {
            case EXIT_BREAK:
                printf("EXIT - Break on opcode: 0x%x\n", chip.ERR_META);
                break;
            case EXIT_ERR_UNKNOWN_OPCODE:
                printf("ERR - Unknown Opcode: 0x%x\n", chip.ERR_META);
                break;
            case EXIT_ERR_STACK_OVERFLOW:
                printf("ERR - Stack Overflow\n");
                break;
            case EXIT_ERR_STACK_UNDERFLOW:
                printf("ERR - Stack Underflow\n");
                break;
            case EXIT_ERR_UNKNOWN_ADDRESS_MODE:
                printf("ERR - Unknown Address Mode: 0x%x\n", chip.ERR_META);
                break;
            default:
                printf("ERR: %d\n", EXIT_CODE);
                break;
        }
    }
    return 0;
}
