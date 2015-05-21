
#ifndef __NESEmu__Chip__
#define __NESEmu__Chip__

#include <stdio.h>
#include <stack>


#define modeAccumulator 0x08
#define modeImmediate 0x08
#define modeImmediate2 0x00
#define modeZeroPage 0x04
#define modeZeroPage2 0x00
#define modeZeroPageX 0x14
#define modeZeroPageX2 0x10
#define modeZeroPageY 0x14
#define modeZeroPageY2 0x10
#define modeAbsolute 0x0C
#define modeAbsolute2 0x08
#define modeAbsoluteX 0x1C
#define modeAbsoluteY 0x18
#define modeAbsoluteY2 0x1C
#define modeIndirectX 0x00
#define modeIndirectY 0x10
#define modeIndirect 0x2C





#define byteAfterOpcode getMemory(pc+1)
#define byte2AfterOpcode getMemory(pc+2)


#define memAccumulator A

#define memImmediate byteAfterOpcode|A

#define addressZeroPage byteAfterOpcode
#define getMemZeroPage getMemory(addressZeroPage)
#define setMemZeroPage(a) setMemory(addressZeroPage, a)

#define addressZeroPageX ((byteAfterOpcode+X) & 0xFF)
#define getMemZeroPageX getMemory(addressZeroPageX)
#define setMemZeroPageX(a) setMemory(addressZeroPageX, a)

#define addressZeroPageY ((byteAfterOpcode+Y) & 0xFF)
#define getMemZeroPageY getMemory(addressZeroPageY)
#define setMemZeroPageY(a) setMemory(addressZeroPageY, a)

#define addressAbsolute (byteAfterOpcode | ((unsigned short)byte2AfterOpcode << 4))
#define getMemAbsolute getMemory(addressAbsolute)
#define setMemAbsolute(a) setMemory(addressAbsolute, a)

#define addressAbsoluteX (((byteAfterOpcode | ((unsigned short)byte2AfterOpcode << 4))+X) & 0xFFFF)
#define getMemAbsoluteX getMemory(addressAbsoluteX)
#define setMemAbsoluteX(a) setMemory(addressAbsoluteX, a)

#define addressAbsoluteY getMemory(((byteAfterOpcode | ((unsigned short)byte2AfterOpcode << 4))+Y) & 0xFFFF)
#define getMemAbsoluteY getMemory(addressAbsoluteY)
#define setMemAbsoluteY(a) setMemory(addressAbsoluteY, a)

#define memIndirectXHelp getMemory(((byteAfterOpcode+X) & 0xFF))
#define addressIndirectX (getMemory(memIndirectXHelp) | ((unsigned short)getMemory(memIndirectXHelp+1) << 4))
#define getMemIndirectX getMemory(addressIndirectX)
#define setMemIndirectX(a) setMemory(addressIndirectX, a)

#define addressIndirectY ((((getMemory(byteAfterOpcode)) | ((unsigned short)getMemory(byte2AfterOpcode) << 4)) + Y) & 0xFFFF)
#define getMemIndirectY getMemory(addressIndirectY)
#define setMemIndirectY(a) setMemory(addressIndirectY, a)

#define addressIndirect (((getMemory(byteAfterOpcode)) | ((unsigned short)getMemory(byte2AfterOpcode) << 4)) & 0xFFFF)
#define getMemIndirect getMemory(addressIndirect)
#define setMemIndirect(a) getMemory(addressIndirect, a)



#define caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(a) case a+modeImmediate: case a+modeZeroPage: case a+modeZeroPageX: case a+modeAbsolute: case a+modeAbsoluteX: case a+modeAbsoluteY: case a+modeIndirectX: case a+modeIndirectY

#define caseModes_ZP_ZPX_AB_ABX_ABY_IX_IY(a) case a+modeZeroPage: case a+modeZeroPageX: case a+modeAbsolute: case a+modeAbsoluteX: case a+modeAbsoluteY: case a+modeIndirectX: case a+modeIndirectY

#define caseModes_A_ZP_ZPX_AB_ABX(a) case a+modeAccumulator: case a+modeZeroPage: case a+modeZeroPageX: case a+modeAbsolute: case a+modeAbsoluteX

#define caseModes_I_ZP_ZPX_AB_ABX(a) case a+modeImmediate2: case a+modeZeroPage: case a+modeZeroPageX: case a+modeAbsolute: case a+modeAbsoluteX

#define caseModes_A_ZP_ZPY_AB_ABY(a) case a+modeAccumulator: case a+modeZeroPage: case a+modeZeroPageY: case a+modeAbsolute: case a+modeAbsoluteY2

#define caseModes_I_ZP_ZPY_AB_ABY(a) case a+modeImmediate2: case a+modeZeroPage: case a+modeZeroPageY: case a+modeAbsolute: case a+modeAbsoluteY2

#define caseModes_ZP_AB(a) case a+modeZeroPage: case a+modeAbsolute

#define caseModes_I_ZP_AB(a) case a+modeImmediate2: case a+modeZeroPage: case a+modeAbsolute

#define caseModes_ZP_ZPX_AB_ABX(a) case a+modeZeroPage: case a+modeZeroPageX: case a+modeAbsolute: case a+modeAbsoluteX

#define caseModes_ZP_ZPY_AB(a) case a+modeZeroPage2: case a+modeZeroPageY2: case a+modeAbsolute2

#define caseModes_ZP_ZPX_AB(a) case a+modeZeroPage2: case a+modeZeroPageX2: case a+modeAbsolute2

#define caseModes_AB_I(a) case a+modeAbsolute: case a+modeIndirect


#define codeADC 0x61
#define codeAND 0x21
#define codeASL 0x02

#define codeBPL 0x10
#define codeBMI 0x30
#define codeBVC 0x50
#define codeBVS 0x70
#define codeBCC 0x90
#define codeBCS 0xB0
#define codeBNE 0xD0
#define codeBEQ 0xF0

#define codeBRK 0x00

#define codeBIT 0x20
#define codeCMP 0xC1
#define codeCPX 0xE0
#define codeCPY 0xC0
#define codeDEC 0xC2
#define codeDEX 0xCA
#define codeDEY 0x88
#define codeEOR 0x41
#define codeINC 0xE2
#define codeINX 0xE8
#define codeINY 0xC8

#define codeCLC 0x18
#define codeSEC 0x38
#define codeCLI 0x58
#define codeSEI 0x78
#define codeCLV 0xB8
#define codeCLD 0xD8
#define codeSED 0xF8

#define codeJMP 0x40
#define codeJSR 0x20
#define codeRTI 0x40
#define codeRTS 0x60
#define codeLDA 0xA1
#define codeLDX 0xA2
#define codeLDY 0xA0
#define codeLSR 0x42

#define codeNOP 0xEA

#define codeORA 0x01

#define codePHA 0x48
#define codePHP 0x08
#define codePLA 0x68
#define codePLP 0x28
#define codeROL 0x22
#define codeROR 0x62
#define codeSBC 0xE1

#define codeSTA 0x81
#define codeSTX 0x86
#define codeSTY 0x84

#define codeTAX 0xAA
#define codeTAY 0xA8
#define codeTSX 0xBA
#define codeTXA 0x8A
#define codeTXS 0x9A
#define codeTYA 0x98

#define EXIT_BREAK 0
#define EXIT_ERR_UNKNOWN_OPCODE 1
#define EXIT_ERR_STACK_OVERFLOW 2
#define EXIT_ERR_STACK_UNDERFLOW 3
#define EXIT_ERR_UNKNOWN_ADDRESS_MODE 4


class Chip{
    
public:
    
    int ERR_META = 0;
    
    //Program counter
    unsigned short pc = 0x0000;
    
    //Accumulator
    unsigned char A = 0x00;
    
    //X and Y Registers
    unsigned char X = 0x00;
    unsigned char Y = 0x00;
    
    //Carry Flag
    bool C = false;
    
    //Overflow Flag
    bool V = false;
    
    //Zero Flag
    bool Z = false;
    
    //Negative Flag
    bool N = false;
    
    //Decimal Flag
    bool D = false;
    
    //Interrupt Flag
    bool I = false;
    
    
    const static unsigned int memorySize = 0x10000;
    
    unsigned char memory[memorySize] = {0};
    
    unsigned char stackPointer = 0;
    unsigned char stack[0x100] = {0};
    
    unsigned char getMemory(unsigned short address){
        if(address < 0x2000){
            return memory[address % 0x800];
        }else if(address >= 0x2000 && address < 0x4000){
            return memory[(address % 8) + 0x800];
        }else{
            return memory[address];
        }
    }
    
    void setMemory(unsigned short address, unsigned char value){
        if(address < 0x2000){
            memory[address % 0x800] = value;
        }else if(address >= 0x2000 && address < 0x4000){
            memory[address % 8] = value;
        }
    }
    
    const unsigned short mem_AB_I(unsigned char mode){
        switch (mode) {
            case modeAbsolute:
                return getMemAbsolute;
                
            case modeIndirect:
                return getMemIndirect;
                
            default:
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    const unsigned char mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode){
        switch (mode) {
            case modeImmediate:
                return memImmediate;
                
            case modeZeroPage:
                return getMemZeroPage;
                
            case modeZeroPageX:
                return getMemZeroPageX;
                
            case modeAbsolute:
                return getMemAbsolute;
                
            case modeAbsoluteX:
                return getMemAbsoluteX;
                
            case modeAbsoluteY:
                return getMemAbsoluteY;
                
            case modeIndirectX:
                return getMemIndirectX;
                
            case modeIndirectY:
                return getMemIndirectY;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    const unsigned char mem_I_ZP_AB(unsigned char mode){
        switch (mode) {
            case modeImmediate2:
                return memImmediate;
                
            case modeZeroPage:
                return getMemZeroPage;
                
            case modeAbsolute:
                return getMemAbsolute;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    const unsigned char mem_ZP_ZPX_AB(unsigned char mode){
        switch (mode) {
            case modeZeroPage2:
                return getMemZeroPage;
                
            case modeZeroPageX2:
                return getMemZeroPageX;
                
            case modeAbsolute2:
                return getMemAbsolute;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    const unsigned char mem_ZP_ZPY_AB(unsigned char mode){
        switch (mode) {
            case modeZeroPage2:
                return getMemZeroPage;
                
            case modeZeroPageY2:
                return getMemZeroPageY;
                
            case modeAbsolute2:
                return getMemAbsolute;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    const unsigned char mem_A_ZP_ZPX_AB_ABX(unsigned char mode){
        switch (mode) {
            case modeAccumulator:
                return memAccumulator;
                
            case modeZeroPage:
                return getMemZeroPage;
                
            case modeZeroPageX:
                return getMemZeroPageX;
                
            case modeAbsolute:
                return getMemAbsolute;
                
            case modeAbsoluteX:
                return getMemAbsoluteX;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    const unsigned char mem_I_ZP_ZPX_AB_ABX(unsigned char mode){
        switch (mode) {
            case modeImmediate2:
                return memImmediate;
                
            case modeZeroPage:
                return getMemZeroPage;
                
            case modeZeroPageX:
                return getMemZeroPageX;
                
            case modeAbsolute:
                return getMemAbsolute;
                
            case modeAbsoluteX:
                return getMemAbsoluteX;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    const unsigned char mem_A_ZP_ZPY_AB_ABY(unsigned char mode){
        switch (mode) {
            case modeAccumulator:
                return memAccumulator;
                
            case modeZeroPage:
                return getMemZeroPage;
                
            case modeZeroPageY:
                return getMemZeroPageY;
                
            case modeAbsolute:
                return getMemAbsolute;
                
            case modeAbsoluteY:
                return getMemAbsoluteY;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    const unsigned char mem_I_ZP_ZPY_AB_ABY(unsigned char mode){
        switch (mode) {
            case modeImmediate2:
                return memImmediate;
                
            case modeZeroPage:
                return getMemZeroPage;
                
            case modeZeroPageY:
                return getMemZeroPageY;
                
            case modeAbsolute:
                return getMemAbsolute;
                
            case modeAbsoluteY:
                return getMemAbsoluteY;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    void setmem_A_ZP_ZPX_AB_ABX(unsigned char mode, unsigned char value){
        switch (mode) {
            case modeAccumulator:
                memAccumulator = value;
                break;
                
            case modeZeroPage:
                setMemZeroPage(value);
                break;
                
            case modeZeroPageX:
                setMemZeroPageX(value);
                break;
                
            case modeAbsolute:
                setMemAbsolute(value);
                break;
                
            case modeAbsoluteX:
                setMemAbsoluteX(value);
                break;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    void setmem_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode, unsigned char value){
        switch (mode) {
                
            case modeZeroPage:
                setMemZeroPage(value);
                break;
                
            case modeZeroPageX:
                setMemZeroPageX(value);
                break;
                
            case modeAbsolute:
                setMemAbsolute(value);
                break;
                
            case modeAbsoluteX:
                setMemAbsoluteX(value);
                break;
                
            case modeAbsoluteY:
                setMemAbsoluteY(value);
                break;
                
            case modeIndirectX:
                setMemIndirectX(value);
                break;
                
            case modeIndirectY:
                setMemIndirectY(value);
                break;
                
            default:
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    void setmem_ZP_ZPX_AB(unsigned char mode, unsigned char value){
        switch (mode) {
                
            case modeZeroPage2:
                setMemZeroPage(value);
                break;
                
            case modeZeroPageX2:
                setMemZeroPageX(value);
                break;
                
            case modeAbsolute2:
                setMemAbsolute(value);
                break;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    void setmem_ZP_ZPY_AB(unsigned char mode, unsigned char value){
        switch (mode) {
                
            case modeZeroPage2:
                setMemZeroPage(value);
                break;
                
            case modeZeroPageY2:
                setMemZeroPageY(value);
                break;
                
            case modeAbsolute2:
                setMemAbsolute(value);
                break;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    unsigned char opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode){
        switch (mode) {
            case modeImmediate:
                return 2;
                
            case modeZeroPage:
                return 2;
                
            case modeZeroPageX:
                return 2;
                
            case modeAbsolute:
                return 3;
                
            case modeAbsoluteX:
                return 3;
                
            case modeAbsoluteY:
                return 3;
                
            case modeIndirectX:
                return 2;
                
            case modeIndirectY:
                return 2;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    unsigned char opcodeLength_A_ZP_ZPX_AB_ABX(unsigned char mode){
        switch (mode) {
            case modeAccumulator:
                return 1;
                
            case modeZeroPage:
                return 2;
                
            case modeZeroPageX:
                return 2;
                
            case modeAbsolute:
                return 3;
                
            case modeAbsoluteX:
                return 3;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    unsigned char opcodeLength_I_ZP_ZPX_AB_ABX(unsigned char mode){
        switch (mode) {
            case modeImmediate2:
                return 2;
                
            case modeZeroPage:
                return 2;
                
            case modeZeroPageX:
                return 2;
                
            case modeAbsolute:
                return 3;
                
            case modeAbsoluteX:
                return 3;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    unsigned char opcodeLength_A_ZP_ZPY_AB_ABY(unsigned char mode){
        switch (mode) {
            case modeAccumulator:
                return 1;
                
            case modeZeroPage:
                return 2;
                
            case modeZeroPageY:
                return 2;
                
            case modeAbsolute:
                return 3;
                
            case modeAbsoluteY:
                return 3;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    unsigned char opcodeLength_I_ZP_ZPY_AB_ABY(unsigned char mode){
        switch (mode) {
            case modeImmediate2:
                return 2;
                
            case modeZeroPage:
                return 2;
                
            case modeZeroPageY:
                return 2;
                
            case modeAbsolute:
                return 3;
                
            case modeAbsoluteY:
                return 3;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    unsigned char opcodeLength_ZP_ZPX_AB(unsigned char mode){
        switch (mode) {
                
            case modeZeroPage2:
                return 2;
                
            case modeZeroPageX2:
                return 2;
                
            case modeAbsolute2:
                return 3;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    unsigned char opcodeLength_ZP_ZPY_AB(unsigned char mode){
        switch (mode) {
                
            case modeZeroPage2:
                return 2;
                
            case modeZeroPageY2:
                return 2;
                
            case modeAbsolute2:
                return 3;
                
            default:
                ERR_META = mode;
                throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
        }
    }
    
    void pushToStack(unsigned char value){
        stack[stackPointer] = value;
        if(stackPointer < 0xFF){
            stackPointer++;
        }else{
            throw EXIT_ERR_STACK_OVERFLOW;
        }
    }
    
    unsigned char popFromStack(){
        if(stackPointer > 0){
            stackPointer--;
        }else{
            throw EXIT_ERR_STACK_UNDERFLOW;
        }
        return stack[stackPointer+1];
    }
    
    void executeNextOpcode(){
        unsigned char opcode = memory[pc];
        
        printf("Executing opcode 0x%x\n", opcode);
        
        switch(opcode){
            
            caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeADC):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeADC);
                unsigned short temp = m + A + C;
                C = temp > 0xFF;
                V = temp > 0x7F;
                A = (unsigned char)temp;
                Z = !A;
                N = (A & 0x80) >> 7;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeADC);
                break;
            }
            
            caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeAND):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeAND);
                A = m & A;
                Z = !A;
                N = (A & 0x80) >> 7;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeAND);
                break;
            }
            
            caseModes_A_ZP_ZPX_AB_ABX(codeASL):{
                unsigned char m = mem_A_ZP_ZPX_AB_ABX(opcode - codeASL);
                C = m & 0x80;
                m = m << 1;
                Z = !m;
                N = (m & 0x80) >> 7;
                setmem_A_ZP_ZPX_AB_ABX(opcode - codeASL, m);
                pc += opcodeLength_A_ZP_ZPX_AB_ABX(opcode - codeASL);
                break;
            }
            
            case codeBPL:{
                if(!N){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                break;
            }
            
            case codeBMI:{
                if(N){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                break;
            }
            
            case codeBVC:{
                if(!V){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                break;
            }
            
            case codeBVS:{
                if(V){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                break;
            }
            
            case codeBCC:{
                if(!C){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                break;
            }
            
            case codeBCS:{
                if(C){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                break;
            }
            
            case codeBNE:{
                if(!Z){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                break;
            }
            
            case codeBEQ:{
                if(Z){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                break;
            }
            
            caseModes_ZP_AB(codeBIT):{
                unsigned short m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeBIT);
                m += A;
                V = m > 0x7F;
                N = (m & 0x80) >> 7;
                Z = true;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeBIT);
                break;
            }
            
            case codeBRK:{
                ERR_META = opcode;
                throw EXIT_BREAK;
            }
            
            caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeCMP):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCMP);
                C = A >= m;
                Z = !(A-m);//TODO figure out if this is correct
                N = ((A-m) & 0x80) >> 7;//TODO figure out if this is correct
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCMP);
                break;
            }
            
            caseModes_I_ZP_AB(codeCPX):{
                unsigned char m = mem_I_ZP_AB(opcode - codeCPX);
                C = X >= m;
                Z = !(X-m);//TODO figure out if this is correct
                N = ((X-m) & 0x80) >> 7;//TODO figure out if this is correct
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCPX);
                break;
            }
            
            caseModes_I_ZP_AB(codeCPY):{
                unsigned char m = mem_I_ZP_AB(opcode - codeCPY);
                C = Y >= m;
                Z = !(Y-m);//TODO figure out if this is correct
                N = ((Y-m) & 0x80) >> 7;//TODO figure out if this is correct
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCPY);
                break;
            }
            
            caseModes_ZP_ZPX_AB_ABX(codeDEC):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeDEC);
                setmem_A_ZP_ZPX_AB_ABX(opcode - codeDEC, m-1);
                Z = !(m-1);
                N = ((m-1) & 0x80) >> 7;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeDEC);
                break;
            }
            
            case codeDEX:{
                X -= 1;
                Z = !(X);
                N = ((X) & 0x80) >> 7;
                pc ++;
                break;
            }
            
            case codeDEY:{
                Y -= 1;
                Z = !(Y);
                N = ((Y) & 0x80) >> 7;
                pc ++;
                break;
            }
            
            caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeEOR):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeEOR);
                A = m ^ A;
                Z = !A;
                N = (A & 0x80) >> 7;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeEOR);
                break;
            }
            
            caseModes_ZP_ZPX_AB_ABX(codeINC):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeINC);
                setmem_A_ZP_ZPX_AB_ABX(opcode - codeINC, m+1);
                Z = !(m+1);
                N = ((m+1) & 0x80) >> 7;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeINC);
                break;
            }
            
            case codeINX:{
                X += 1;
                Z = !(X);
                N = ((X) & 0x80) >> 7;
                pc ++;
                break;
            }
            
            case codeINY:{
                Y += 1;
                Z = !(Y);
                N = ((Y) & 0x80) >> 7;
                pc ++;
                break;
            }
            
            case codeCLC:{
                C = false;
                pc++;
                break;
            }
            
            case codeSEC:{
                C = true;
                pc++;
                break;
            }
            
            case codeCLI:{
                I = false;
                pc++;
                break;
            }
            
            case codeSEI:{
                I = true;
                pc++;
                break;
            }
            
            case codeCLV:{
                V = false;
                pc++;
                break;
            }
            
            case codeCLD:{
                D = false;
                pc++;
                break;
            }
            
            case codeSED:{
                D = true;
                pc++;
                break;
            }
            
            caseModes_AB_I(codeJMP):{
                pc = mem_AB_I(opcode - codeJMP);
                break;
            }
            
            case codeJSR:{
                unsigned short t = pc+3;
                pushToStack(t & 0xFF);
                pushToStack((t >> 8) & 0xFF);
                pc = (byteAfterOpcode) | ((unsigned short)byte2AfterOpcode << 4);
                break;
            }
            
            case codeRTS:{
                pc = popFromStack();
                pc &= (popFromStack() << 8);
                break;
            }
            
            caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeLDA):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeLDA);
                A = m;
                Z = !A;
                N = (A & 0x80) >> 7;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeLDA);
                break;
            }
            
            caseModes_I_ZP_ZPY_AB_ABY(codeLDX):{
                unsigned char m = mem_I_ZP_ZPY_AB_ABY(opcode - codeLDX);
                X = m;
                Z = !X;
                N = (X & 0x80) >> 7;
                pc += opcodeLength_I_ZP_ZPY_AB_ABY(opcode - codeLDX);
                break;
            }
            
            caseModes_I_ZP_ZPX_AB_ABX(codeLDY):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX(opcode - codeLDY);
                Y = m;
                Z = !Y;
                N = (Y & 0x80) >> 7;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX(opcode - codeLDY);
                break;
            }
            
            caseModes_A_ZP_ZPX_AB_ABX(codeLSR):{
                unsigned char m = mem_A_ZP_ZPX_AB_ABX(opcode - codeLSR);
                C = m & 0x01;
                m = m >> 1;
                Z = !m;
                N = ((m & 0x80)) >> 7;
                setmem_A_ZP_ZPX_AB_ABX(opcode - codeLSR, m);
                pc += opcodeLength_A_ZP_ZPX_AB_ABX(opcode - codeLSR);
                break;
            }
            
            case codeNOP:{
                //This does nothing
                pc ++;
                break;
            }
            
            caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeORA):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeORA);
                A = m | A;
                Z = !A;
                N = (A & 0x80) >> 7;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeORA);
                break;
            }
            
            case codePHA:{
                pushToStack(A);
                pc ++;
                break;
            }
            
            case codePHP:{
                pushToStack((N << 5) & (Z << 4) & (C << 3) & (I << 2) & (D << 1) & (V));
                pc ++;
                break;
            }
            
            case codePLA:{
                A = popFromStack();
                Z = !A;
                N = (A & 0x80) >> 7;
                pc ++;
                break;
            }
            
            case codePLP:{
                unsigned char flags = popFromStack();
                N = (flags >> 5) & 0x1;
                Z = (flags >> 4) & 0x1;
                C = (flags >> 3) & 0x1;
                I = (flags >> 2) & 0x1;
                D = (flags >> 1) & 0x1;
                V = (flags) & 0x1;
                pc ++;
                break;
            }
            
            caseModes_A_ZP_ZPX_AB_ABX(codeROL):{
                unsigned char m = mem_A_ZP_ZPX_AB_ABX(opcode - codeROL);
                C = (m & 0x80) >> 7;
                m = m << 1;
                m = m & C;
                Z = !m;
                N = (m & 0x80) >> 7;
                setmem_A_ZP_ZPX_AB_ABX(opcode - codeROL, m);
                pc += opcodeLength_A_ZP_ZPX_AB_ABX(opcode - codeROL);
                break;
            }
            
            caseModes_A_ZP_ZPX_AB_ABX(codeROR):{
                unsigned char m = mem_A_ZP_ZPX_AB_ABX(opcode - codeROR);
                C = m & 0x1;
                m = m >> 1;
                m = m & (C << 7);
                Z = !m;
                N = (m & 0x80) >> 7;
                setmem_A_ZP_ZPX_AB_ABX(opcode - codeROR, m);
                pc += opcodeLength_A_ZP_ZPX_AB_ABX(opcode - codeROR);
                break;
            }
            
            case codeRTI:{
                unsigned char flags = popFromStack();
                N = (flags >> 5) & 0x1;
                Z = (flags >> 4) & 0x1;
                C = (flags >> 3) & 0x1;
                I = (flags >> 2) & 0x1;
                D = (flags >> 1) & 0x1;
                V = (flags) & 0x1;
                pc = popFromStack();
                break;
            }
            
            caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeSBC):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSBC);
                unsigned short temp = A - m - C;
                C = temp > 0xFF;
                V = temp > 0x7F;
                A = (unsigned char)temp;
                Z = !A;
                N = (A & 0x80) >> 7;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSBC);
                break;
            }
            
            caseModes_ZP_ZPX_AB_ABX_ABY_IX_IY(codeSTA):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSTA);
                setmem_ZP_ZPX_AB_ABX_ABY_IX_IY(m, A);
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSTA);
                break;
            }
            
            caseModes_ZP_ZPY_AB(codeSTX):{
                unsigned char m = mem_ZP_ZPY_AB(opcode - codeSTX);
                setmem_ZP_ZPY_AB(m, X);
                pc += opcodeLength_ZP_ZPY_AB(opcode - codeSTX);
                break;
            }
            
            caseModes_ZP_ZPX_AB(codeSTY):{
                unsigned char m = mem_ZP_ZPX_AB(opcode - codeSTY);
                setmem_ZP_ZPX_AB(m, Y);
                pc += opcodeLength_ZP_ZPX_AB(opcode - codeSTY);
                break;
            }
            
            case codeTAX:{
                X = A;
                Z = !X;
                N = (X & 0x80) >> 7;
                pc += 1;
                break;
            }
            
            case codeTAY:{
                Y = A;
                Z = !Y;
                N = (Y & 0x80) >> 7;
                pc += 1;
                break;
            }
            
            case codeTSX:{
                X = stackPointer;
                Z = !Y;
                N = (Y & 0x80) >> 7;
                pc += 1;
                break;
            }
            
            case codeTXA:{
                A = X;
                Z = !A;
                N = (A & 0x80) >> 7;
                pc += 1;
                break;
            }
            
            case codeTYA:{
                A = Y;
                Z = !A;
                N = (A & 0x80) >> 7;
                pc += 1;
                break;
            }
            
            case codeTXS:{
                stackPointer = X;
                pc += 1;
                break;
            }
            
            default:
                ERR_META = opcode;
                throw EXIT_ERR_UNKNOWN_OPCODE;
            
        }
    }
    
};



#endif /* defined(__NESEmu__Chip__) */
