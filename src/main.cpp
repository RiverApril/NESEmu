
#include <iostream>
#include <fstream>
#include "Chip.hpp"
#include "DisplaySDL.hpp"

using namespace std;

Chip chip;

void printMemory(unsigned char* memory, unsigned int memoryMin, unsigned int memoryDisplayAmount){
    for(unsigned int i=memoryMin;i<memoryMin+memoryDisplayAmount;i++){
        printf("%02X ", memory[i]);
    }
    printf("\n");
}

bool loadFile(const char* path, Chip* chip){
    ifstream file;
    file.open(path, ios::binary | ios::in);
    
    file.seekg(0, ios::end);
    int fileSize = file.tellg();
    file.seekg(0, ios::beg);
    
    if(file){
        char buffer[fileSize];
        file.read(buffer, fileSize);
        file.close();
        
        printf("File Size: %X\n", fileSize);
        
        unsigned char header[16];
        for(unsigned int i=0;i<0x10;i++){
            header[i] = (unsigned char)buffer[i];
        }
        
        if(!(header[0] == 0x4E && header[1] == 0x45 && header[2] == 0x53 && header[3] == 0x1A)){
            printf("File does not start with NES<eof> [4E 45 53 1A]\n");
            return false;
        }
        
        unsigned int PRG_ROMSize = header[4]; // 0x4000 (16 KB) units
        unsigned int CHR_ROMSize = header[5]; // 0x2000 (8 KB) units
        
        unsigned int flags6 = header[6];
        unsigned int flags7 = header[7];
        
        unsigned int PRG_RAMSize = header[8]; // 0x2000 (8 KB) units
        if(PRG_RAMSize == 0){
            PRG_RAMSize = 1;
        }
        
        unsigned int flags9 = header[9];
        unsigned int flags10 = header[10];
        
        /*for(unsigned int i=0x10;i<memorySize-startIndex;i++){
            chip.setMemory(i+startIndex, (unsigned char)buffer[i]);
        }*/
        
        printf("PRG ROM Size: %X * 4000 = %X\n", PRG_ROMSize, PRG_ROMSize*0x4000);
        printf("CHR ROM Size: %X * 2000 = %X\n", CHR_ROMSize, CHR_ROMSize*0x2000);
        
        unsigned int loc;
        unsigned int size;
        
        loc = 0x10;
        size = (PRG_ROMSize*0x4000);
        for(unsigned int i=0; i<size; i++){
            chip->setMemory(0xC000+i, (unsigned char)buffer[loc + i]);
        }
        loc += size;
        size = (CHR_ROMSize*0x2000);
        for(unsigned int i=0; i<size; i++){
            chip->setMemory(0xE000+i, (unsigned char)buffer[loc + i]);
        }
        
        
        return true;
    }else{
        printf("Could not open file: '%s'\n", path);
        return false;
    }
}

void setMemory(int address, unsigned char value){
    chip.setMemory((unsigned short)address, value);
}

unsigned char getMemoryPassive(int address){
    return chip.getMemory((unsigned short)address, true);
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
    chip.reset(true, true, true);
    bool success = loadFile(runPath, &chip);
    if(success){
        printf("Program loaded, running.\n");
    }else{
        printf("Failed to load program.\n");
        return 0;
    }

    DisplaySDL* display = new DisplaySDL("NES-Emu", 0x220, 0x200);
    
    bool chipRunning = true;
    
    int tick = 0;
    
    while(!(display->quit || display->errored)) {
        try{
            display->update(chip.controllerP1Buffer, chip.controllerP2Buffer);
            
            if(chipRunning){
                
                tick++;
                
                chip.prepareOpcode();
                
                
                display->drawGridAt(&getMemoryPassive, 0x100, 0x100, 0x01, 0, 0, 0x200, 0x200);
                display->drawGridAt((unsigned char*)chip.controllerP1Buffer, 0x8, 0x1, 0xFF, 0x200, 0, 0x20, 0x4, 0xFF, 0x08);
                display->drawGridAt((unsigned char*)chip.controllerP2Buffer, 0x8, 0x1, 0xFF, 0x200, 0x4, 0x20, 0x4, 0xFF, 0x08);
                int pcx = (chip.pc % 0x100)*2;
                int pcy = (chip.pc / 0x100)*2;
                display->drawPixelAt(pcx, pcy, 0xFF, 0, 0, 0xFF, 2, 2);
                display->drawPixelAt(0, pcy, 0xFF, 0, 0, 0x80, pcx, 2);
                display->drawPixelAt(pcx, 0, 0xFF, 0, 0, 0x80, 2, pcy);
                
                
                printf("\n%04d pc:%04X SP:%02X  A:%02X X:%02X Y:%02X  P:%02X{N%dV%d?1?0D%dI%dZ%dC%d} %s (%02X) ", tick, chip.pc, chip.stackPointer, chip.A, chip.X, chip.Y, chip.CPU_S, chip.CPU_N, chip.CPU_V, chip.CPU_D, chip.CPU_I, chip.CPU_Z, chip.CPU_C, chip.opcodeName(chip.opcode), chip.opcode);
                
                unsigned char len = chip.opcodeLength(chip.opcode);
                
                switch(len){
                    case 1:
                        printf("[     ]");
                        break;
                    case 2:
                        printf("[%02X   ]", chip.byteAfterOpcode);
                        break;
                    case 3:
                        printf("[%02X %02X]", chip.byteAfterOpcode, chip.byte2AfterOpcode);
                        break;
                    default:
                        printf("LEN: %d", len);
                        break;
                    
                }
                
                chip.executeOpcode();
                printf("\n");
                for(unsigned int i=0;i<0x800;i++){
                    if(chip.getMemory(i)){
                        printf(" %04X=%02X", i, chip.getMemory(i));
                    }
                }
                
                printf("F921: %02X", chip.getMemory(0xF931));
                
                /*printf("Stack: [");
                for(int i=0;i<chip.stackPointer;i++){
                    printf("%x%s", chip.stack[i], (i==(chip.stackPointer-1))?"":", ");
                }
                printf("]\n");*/
                
            }
            
            display->draw();
            
        }catch(int EXIT_CODE){
            chipRunning = false;
            switch (EXIT_CODE) {
                case EXIT_BREAK:
                    printf("\nEXIT - Break on Opcode: %02X\n", chip.ERR_META);
                    break;
                case EXIT_ERR_UNKNOWN_OPCODE:
                    printf("\nERR - Unknown Opcode: %02X\n", chip.ERR_META);
                    break;
                case EXIT_ERR_STACK_OVERFLOW:
                    printf("\nERR - Stack Overflow\n");
                    break;
                case EXIT_ERR_STACK_UNDERFLOW:
                    printf("\nERR - Stack Underflow\n");
                    break;
                case EXIT_ERR_UNKNOWN_ADDRESS_MODE:
                    printf("\nERR - Unknown Address Mode: %X\n", chip.ERR_META);
                    break;
                case EXIT_ERR_MEMORY_ADDRESS_OUT_OF_RANGE:
                    printf("\nERR - Memory Address out of range: %X\n", chip.ERR_META);
                    break;
                default:
                    printf("\nERR: %X, %X\n", EXIT_CODE, chip.ERR_META);
                    break;
            }
            printf("\nPress excape in the window to exit.\n");
        }
    }

    delete display;

    return 0;
}
