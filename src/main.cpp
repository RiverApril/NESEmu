
#include <iostream>
#include <fstream>
#include "Chip.hpp"
#include "DisplaySDL.hpp"

using namespace std;

//Use hexcurse to edit the hex file


Chip chip;

void printMemory(unsigned char* memory, unsigned int memoryMin, unsigned int memoryDisplayAmount){
    for(unsigned int i=memoryMin;i<memoryMin+memoryDisplayAmount;i++){
        printf("%02X ", chip.memory[i]);
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
    if(compilePath[0] == '\0'){
        //Compilation not fully implemented.
    }
    if(runPath[0] == '\0'){
        printf("Please pecify a program to run.\n");
        return 0;
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
    
    bool chipRunning = true;
    
    while(!(display->quit || display->errored)) {
        try{
            display->update();
            
            if(chipRunning){
                
                display->drawGridAt(chip.memory, 0x100, 0x100, 0x01, 0, 0, 0x100, 0x100);
                
                printf("\npc:%04X  ", chip.pc);
                printf("A:%02X X:%02X Y:%02X  ", chip.A, chip.X, chip.Y);
                printf("P:%02X N%c V%c ?1 ?0 D%c I%c Z%c C%c\n", chip.S(), chip.N?'1':'0', chip.V?'1':'0', chip.D?'1':'0', chip.I?'1':'0', chip.Z?'1':'0', chip.C?'1':'0');
                //printMemory(chip.memory, 0x0, 0xFF);
                /*printf("Stack: [");
                for(int i=0;i<chip.stackPointer;i++){
                    printf("%x%s", chip.stack[i], (i==(chip.stackPointer-1))?"":", ");
                }
                printf("]\n");*/
            
                chip.executeNextOpcode();
            }
            
            display->draw();
            
        }catch(int EXIT_CODE){
            switch (EXIT_CODE) {
                case EXIT_BREAK:
                    printf("EXIT - Break on opcode: 0x%X\n", chip.ERR_META);
                    chipRunning = false;
                    break;
                case EXIT_ERR_UNKNOWN_OPCODE:
                    printf("ERR - Unknown Opcode: 0x%X\n", chip.ERR_META);
                    chipRunning = false;
                    break;
                case EXIT_ERR_STACK_OVERFLOW:
                    printf("ERR - Stack Overflow\n");
                    chipRunning = false;
                    break;
                case EXIT_ERR_STACK_UNDERFLOW:
                    printf("ERR - Stack Underflow\n");
                    chipRunning = false;
                    break;
                case EXIT_ERR_UNKNOWN_ADDRESS_MODE:
                    printf("ERR - Unknown Address Mode: 0x%X\n", chip.ERR_META);
                    chipRunning = false;
                    break;
                default:
                    printf("ERR: %d\n", EXIT_CODE);
                    chipRunning = false;
                    break;
            }
            printf("\nPress excape in the window to exit.\n");
        }
    }

    delete display;

    return 0;
}
