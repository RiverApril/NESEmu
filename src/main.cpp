
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

    DisplaySDL* display = new DisplaySDL("NES-Emu", 0x124, 0x100);
    
    bool chipRunning = true;
    
    int tick = 0;
    
    while(!(display->quit || display->errored)) {
        try{
            display->update(chip.controllerP1Buffer, chip.controllerP2Buffer);
            
            if(chipRunning){
                
                tick++;
                
                display->drawGridAt(chip.memory, 0x100, 0x100, 0x01, 0, 0, 0x100, 0x100);
                display->drawGridAt((unsigned char*)chip.controllerP1Buffer, 0x8, 0x1, 0xFF, 0x100, 0, 0x20, 0x4, 0xFF, 0x08);
                display->drawGridAt((unsigned char*)chip.controllerP2Buffer, 0x8, 0x1, 0xFF, 0x100, 0x4, 0x20, 0x4, 0xFF, 0x08);
                display->drawGridAt(&chip.controllerP1Index, 0x1, 0x1, 0x20, 0x120, 0, 0x4, 0x4);
                display->drawGridAt(&chip.controllerP2Index, 0x1, 0x1, 0x20, 0x120, 0x4, 0x4, 0x4);
                
                printf("\n%04d pc:%04X  A:%02X X:%02X Y:%02X  P:%02X{N%d,V%d,1,0,D%d,I%d,Z%d,C%d} %s (%02X) [%02X %02X]", tick, chip.pc, chip.A, chip.X, chip.Y, chip.S(), chip.N, chip.V, chip.D, chip.I, chip.Z, chip.C, chip.opcodeName(chip.opcode), chip.opcode, chip.byteAfterOpcode, chip.byte2AfterOpcode);
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
                    printf("\nEXIT - Break on Opcode: 0x%X\n", chip.ERR_META);
                    chipRunning = false;
                    break;
                case EXIT_ERR_UNKNOWN_OPCODE:
                    printf("\nERR - Unknown Opcode: 0x%X\n", chip.ERR_META);
                    chipRunning = false;
                    break;
                case EXIT_ERR_STACK_OVERFLOW:
                    printf("\nERR - Stack Overflow\n");
                    chipRunning = false;
                    break;
                case EXIT_ERR_STACK_UNDERFLOW:
                    printf("\nERR - Stack Underflow\n");
                    chipRunning = false;
                    break;
                case EXIT_ERR_UNKNOWN_ADDRESS_MODE:
                    printf("\nERR - Unknown Address Mode: 0x%X\n", chip.ERR_META);
                    chipRunning = false;
                    break;
                default:
                    printf("\nERR: %d\n", EXIT_CODE);
                    chipRunning = false;
                    break;
            }
            printf("\nPress excape in the window to exit.\n");
        }
    }

    delete display;

    return 0;
}
