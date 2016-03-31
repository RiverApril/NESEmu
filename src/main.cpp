
#include <iostream>
#include <fstream>
#include <sstream>
#include "Chip.hpp"
#include "DisplaySDL.hpp"

using namespace std;

Chip chip;
DisplaySDL* display;

void printMemory(unsigned char* memory, unsigned int memoryMin, unsigned int memoryDisplayAmount){
    for(unsigned int i=memoryMin;i<memoryMin+memoryDisplayAmount;i++){
        fprintf(stderr, "%02X ", memory[i]);
    }
    fprintf(stderr, "\n");
}

bool loadFile(const char* path, Chip& chip){
    ifstream file;
    file.open(path, ios::binary | ios::in);

    file.seekg(0, ios::end);
    int fileSize = file.tellg();
    file.seekg(0, ios::beg);

    if(file){
        char buffer[fileSize];
        file.read(buffer, fileSize);
        file.close();

        fprintf(stderr, "File Size: %X\n", fileSize);

        unsigned char header[16];
        for(unsigned int i=0;i<0x10;i++){
            header[i] = (unsigned char)buffer[i];
        }

        if(!(header[0] == 0x4E && header[1] == 0x45 && header[2] == 0x53 && header[3] == 0x1A)){
            fprintf(stderr, "File does not start with NES<eof> [4E 45 53 1A]\n");
            return false;
        }

        unsigned int NROM_Type = header[4];

        unsigned int loc;

        loc = 0x10;
        
        if(NROM_Type == 1){ //NROM 128
            printf("NROM 128\n");
            for(unsigned int i=0; i<0x4000; i++){
                chip.setMemory(0x8000+i, (unsigned char)buffer[loc]);
                chip.setMemory(0xC000+i, (unsigned char)buffer[loc]);
                loc++;
            }
        } else if(NROM_Type == 2){ //NROM 256
            printf("NROM 256\n");
            for(unsigned int i=0; i<0x8000; i++){
                chip.setMemory(0x8000+i, (unsigned char)buffer[loc]);
                loc++;
            }
        } else {
            fprintf(stderr, "NROM type invailid: %d\n", NROM_Type);
            return false;
        }
        
        for(unsigned int i=0; i<0x2000; i++){
            chip.setCharROM(i, (unsigned char)buffer[loc]);
            loc++;
        }


        return true;
    }else{
        fprintf(stderr, "Could not open file: '%s'\n", path);
        return false;
    }
}

void setMemory(int address, unsigned char value){
    chip.setMemory((unsigned short)address, value);
}

unsigned char getMemoryPassive(int address){
    return chip.getMemory((unsigned short)address, true);
}

bool dumpMemoryToFile(const char* path, unsigned char (*getMem)(int), unsigned int start, unsigned int end){
    ofstream file;
    file.open(path, ios::binary | ios::out);

    unsigned int size = end-start;
    unsigned char buffer[size];

    for(int i=0;i<size;i++){
        buffer[i] = getMem(start+i);
    }

    if(file){
        file.write((const char*)buffer, size);
        file.close();
        return true;
    }
    return false;

}

int main(int argc, const char * argv[]) {
    
    const char* runPath = "";
    if(argc > 1){
        runPath = argv[1];
    }
    if(runPath[0] == '\0'){
        fprintf(stderr, "Please pecify a program to run.\n");
        return 0;
    }
    fprintf(stderr, "Loading program: '%s'\n", runPath);
    chip.reset(true, true, true);
    bool success = loadFile(runPath, chip);
    if(success){
        fprintf(stderr, "Program loaded, running.\n");
    }else{
        fprintf(stderr, "Failed to load program.\n");
        return 0;
    }
    if(argc > 2){
        if(argv[2][0] == '0' && (argv[2][1] == 'x' || argv[2][1] == 'X')){
            unsigned int x;
            std::stringstream ss;
            ss << std::hex << (argv[2]+2);
            ss >> x;
            chip.pc = (unsigned short)x;
        }else{
            unsigned int x;
            std::stringstream ss;
            ss << argv[2];
            ss >> x;
            chip.pc = (unsigned short)x;
        }
        printf("PC set to %X from manual input\n", chip.pc);
    }else{
        chip.setPCToResetVector();
        printf("PC set to %X from reset vector at %X\n", chip.pc, RESET_VECTOR);
    }

    display = new DisplaySDL("NES-Emu", 0x220, 0x200);

    bool chipRunning = true;

    int tick = 0;

    chip.startPPU();

    while(!(display->quit || display->errored)) {
        try{
            
            if(display->mouseDown){
                printf("(%d, %d) Memory: [%4X] = %2X\n", (display->mouseX/2), (display->mouseY/2), (display->mouseY/2)*0x100+(display->mouseX/2), getMemoryPassive((display->mouseY/2)*0x100+(display->mouseX/2)));
            }

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
                
                
                fprintf(stdout, "%04X  ", chip.pc);
                switch(chip.opcodeLength(chip.opcode)){
                    case 1:
                        fprintf(stdout, "%02X      ", chip.opcode);
                        break;
                    case 2:
                        fprintf(stdout, "%02X %02X   ", chip.opcode, chip.byteAfterOpcode);
                        break;
                    case 3:
                        fprintf(stdout, "%02X %02X %02X", chip.opcode, chip.byteAfterOpcode, chip.byte2AfterOpcode);
                        break;
                }
                fprintf(stdout, "  %s", chip.opcodeName(chip.opcode));
                fprintf(stdout, "   ");
                fprintf(stdout, "A:%02X X:%02X Y:%02X P:%02X SP:%02X   ", chip.A, chip.X, chip.Y, chip.CPU_S_GET(false, true), chip.stackPointer);
                
                /*
                fprintf(stdout, "%04d pc:%04X SP:%02X  A:%02X X:%02X Y:%02X  P:%02X {N%dV%d?%dB%dD%dI%dZ%dC%d} %s (%02X) ", tick, chip.pc, chip.stackPointer, chip.A, chip.X, chip.Y, chip.CPU_S_GET(), chip.CPU_N, chip.CPU_V, chip.CPU_Flag5, chip.CPU_Flag4, chip.CPU_D, chip.CPU_I, chip.CPU_Z, chip.CPU_C, chip.opcodeName(chip.opcode), chip.opcode);

                unsigned char len = chip.opcodeLength(chip.opcode);

                switch(len){
                    case 1:
                        fprintf(stdout, "[     ]");
                        break;
                    case 2:
                        fprintf(stdout, "[%02X   ]", chip.byteAfterOpcode);
                        break;
                    case 3:
                        fprintf(stdout, "[%02X %02X]", chip.byteAfterOpcode, chip.byte2AfterOpcode);
                        break;
                    default:
                        fprintf(stderr, "LEN: %d", len);
                        break;

                }*/

                fprintf(stdout, "\n");

                chip.executeOpcode();

                /*fprintf(stderr, "\n");
                for(unsigned int i=0;i<0x800;i++){
                    if(chip.getMemory(i)){
                        fprintf(stderr, " %04X=%02X", i, chip.getMemory(i));
                    }
                }*/

                /*if(tick == 0x0001){
                    dumpMemoryToFile("dump.bin", &getMemoryPassive, 0, 0xFFFF);
                }*/

                //fprintf(stderr, "F921: %02X", chip.getMemory(0xF931));

                /*fprintf(stderr, "Stack: [");
                for(int i=0;i<chip.stackPointer;i++){
                    fprintf(stderr, "%x%s", chip.stack[i], (i==(chip.stackPointer-1))?"":", ");
                }
                fprintf(stderr, "]\n");*/

            }
            
            display->update(chip.controllerP1Buffer, chip.controllerP2Buffer);
            
            display->draw();

        }catch(int EXIT_CODE){
            chipRunning = false;
            switch (EXIT_CODE) {
                case EXIT_ERR_UNKNOWN_OPCODE:
                    fprintf(stderr, "\nERR - Unknown Opcode: %02X\n", chip.ERR_META);
                    break;
                case EXIT_ERR_STACK_OVERFLOW:
                    fprintf(stderr, "\nERR - Stack Overflow\n");
                    break;
                case EXIT_ERR_STACK_UNDERFLOW:
                    fprintf(stderr, "\nERR - Stack Underflow\n");
                    break;
                case EXIT_ERR_UNKNOWN_ADDRESS_MODE:
                    fprintf(stderr, "\nERR - Unknown Address Mode: %X\n", chip.ERR_META);
                    break;
                case EXIT_ERR_MEMORY_ADDRESS_OUT_OF_RANGE:
                    fprintf(stderr, "\nERR - Memory Address out of range: %X\n", chip.ERR_META);
                    break;
                default:
                    fprintf(stderr, "\nERR: %X, %X\n", EXIT_CODE, chip.ERR_META);
                    break;
            }
            fprintf(stderr, "\nPress excape in the window to exit.\n");
        }
    }

    chip.stopPPU();

    delete display;

    return 0;
}
