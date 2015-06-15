
#include <iostream>
#include <fstream>
#include "Chip.hpp"
#include "DisplaySDL.hpp"

using namespace std;

//Use hexcurse to edit the hex file


Chip chip;

void printMemory(unsigned char* memory, unsigned int memoryMin, unsigned int memoryDisplayAmount){
    for(unsigned int i=memoryMin;i<memoryMin+memoryDisplayAmount;i++){
        printf("%02x ", chip.memory[i]);
    }
    printf("\n");
}

bool loadFile(const char* path, unsigned int startIndex, unsigned char* memory, unsigned int memorySize){
    ifstream file;
    file.open(path, ios::binary | ios::in);
    if(file){
        char buffer[memorySize];
        file.read(buffer, memorySize);
        file.close();
        for(unsigned int i=0;i<memorySize-startIndex;i++){
            memory[i+startIndex] = (unsigned char)buffer[i];
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
    bool success = loadFile(runPath, 0xBFF0, chip.memory, chip.memorySize);
    chip.pc = 0xC000;
    chip.I = true;
    if(success){
        printf("Program loaded, running.\n");
    }else{
        printf("Failed to load program.\n");
        return 0;
    }

    DisplaySDL* display = new DisplaySDL("NES EMU", 0x100, 0x100);

    try{
        while(true) {
            printf("\npc: %X   ", chip.pc);
            printf("A: %X, X: %X, Y: %X    ", chip.A, chip.X, chip.Y);
            printf("P:%X  N%c V%c ?* ?_ D%c I%c Z%c C%c\n", ((chip.N << 7) | (chip.V << 6) | (1 << 5) | (0 << 4) | (chip.D << 3) | (chip.I << 2) | (chip.Z << 1) | (chip.C)), chip.N?'*':'_', chip.V?'*':'_', chip.D?'*':'_', chip.I?'*':'_', chip.Z?'*':'_', chip.C?'*':'_');
            printMemory(chip.memory, 0x0, 0x10);
            printf("Stack: [");
            for(int i=0;i<chip.stackPointer;i++){
                printf("%x%s", chip.stack[i], (i==(chip.stackPointer-1))?"":", ");
            }
            printf("]\n");
            display->update();
            display->drawGridAt(chip.memory, 0x100, 0x100, 0x01, 0, 0, 0x100, 0x100);
            display->draw();

            chip.executeNextOpcode();
        }
    }catch(int EXIT_CODE){
        switch (EXIT_CODE) {
            case EXIT_BREAK:
                printf("EXIT - Break on opcode: 0x%X\n", chip.ERR_META);
                break;
            case EXIT_ERR_UNKNOWN_OPCODE:
                printf("ERR - Unknown Opcode: 0x%X\n", chip.ERR_META);
                break;
            case EXIT_ERR_STACK_OVERFLOW:
                printf("ERR - Stack Overflow\n");
                break;
            case EXIT_ERR_STACK_UNDERFLOW:
                printf("ERR - Stack Underflow\n");
                break;
            case EXIT_ERR_UNKNOWN_ADDRESS_MODE:
                printf("ERR - Unknown Address Mode: 0x%X\n", chip.ERR_META);
                break;
            case EXIT_SDL_QUIT:
                printf("EXIT - SDL Quit\n");
                break;
            default:
                printf("ERR: %d\n", EXIT_CODE);
                break;
        }
    }

    printf("\nPress enter to exit...\n");
    cin.ignore();

    delete display;

    return 0;
}
