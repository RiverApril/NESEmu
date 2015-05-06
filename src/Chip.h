
#ifndef __NESEmu__Chip__
#define __NESEmu__Chip__

#include <stdio.h>
#include <stack>

#define modeAccumulator 0x08
#define modeImmediate 0x08
#define modeImmediate2 0x00
#define modeZeroPage 0x04
#define modeZeroPageX 0x14
#define modeAbsolute 0x0C
#define modeAbsoluteX 0x1C
#define modeAbsoluteY 0x18
#define modeIndirectX 0x00
#define modeIndirectY 0x10
#define modeIndirect 0x2C

#define byteAfterOpcode memory[pc+1]
#define byte2AfterOpcode memory[pc+2]


#define memAccumulator A

#define memImmediate byteAfterOpcode|A
#define memZeroPage memory[byteAfterOpcode]
#define memZeroPageX memory[(byteAfterOpcode+X) & 0xFF]
#define memAbsolute memory[byteAfterOpcode | ((unsigned short)byte2AfterOpcode << 4)]
#define memAbsoluteX memory[((byteAfterOpcode | ((unsigned short)byte2AfterOpcode << 4))+X) & 0xFFFF]
#define memAbsoluteY memory[((byteAfterOpcode | ((unsigned short)byte2AfterOpcode << 4))+Y) & 0xFFFF]

#define memIndirectXHelp memory[((byteAfterOpcode+X) & 0xFF)]
#define memIndirectX memory[memory[memIndirectXHelp] | ((unsigned short)memory[memIndirectXHelp+1] << 4)]

#define memIndirectY memory[(((memory[byteAfterOpcode]) | ((unsigned short)memory[byte2AfterOpcode] << 4)) + Y) & 0xFFFF]

#define memIndirect memory[((memory[byteAfterOpcode]) | ((unsigned short)memory[byte2AfterOpcode] << 4)) & 0xFFFF]



#define caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(a) case a+modeImmediate: case a+modeZeroPage: case a+modeZeroPageX: case a+modeAbsolute: case a+modeAbsoluteX: case a+modeAbsoluteY: case a+modeIndirectX: case a+modeIndirectY

#define caseModes_A_ZP_ZPX_AB_ABX(a) case a+modeAccumulator: case a+modeZeroPage: case a+modeZeroPageX: case a+modeAbsolute: case a+modeAbsoluteX

#define caseModes_ZP_AB(a) case a+modeZeroPage: case a+modeAbsolute

#define caseModes_I_ZP_AB(a) case a+modeImmediate2: case a+modeZeroPage: case a+modeAbsolute

#define caseModes_ZP_ZPX_AB_ABX(a) case a+modeZeroPage: case a+modeZeroPageX: case a+modeAbsolute: case a+modeAbsoluteX

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

#define codeBIT 0x20
#define codeBRK 0x00
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
#define codeRTS 0x60

#define EXIT_CODE_NONE -1
#define EXIT_CODE_BREAK 0
#define EXIT_CODE_ERROR 1

class Chip{
    
public:
    
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
    
    
    const static unsigned short memorySize = 0x1000;
    
    unsigned char memory[memorySize] = {0};
    
    
    std::stack<unsigned short> pcStack;
    
    
    
    const unsigned short mem_AB_I(unsigned char mode){
        switch (mode) {
            case modeAbsolute:
                return memAbsolute;
                
            case modeIndirect:
                return memIndirect;
                
            default:
                printf("Error (undefined mode): %d", (int)mode);
                return 0;
        }
    }
    
    const unsigned char mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode){
        switch (mode) {
            case modeImmediate:
                return memImmediate;
                
            case modeZeroPage:
                return memZeroPage;
                
            case modeZeroPageX:
                return memZeroPageX;
                
            case modeAbsolute:
                return memAbsolute;
                
            case modeAbsoluteX:
                return memAbsoluteX;
                
            case modeAbsoluteY:
                return memAbsoluteY;
                
            case modeIndirectX:
                return memIndirectX;
                
            case modeIndirectY:
                return memIndirectY;
                
            default:
                printf("Error (undefined mode): %d", (int)mode);
                return 0;
        }
    }
    
    const unsigned char mem_I_ZP_AB(unsigned char mode){
        switch (mode) {
            case modeImmediate2:
                return memImmediate;
                
            case modeZeroPage:
                return memZeroPage;
                
            case modeAbsolute:
                return memAbsolute;
                
            default:
                printf("Error (undefined mode): %d", (int)mode);
                return 0;
        }
    }
    
    const unsigned char mem_A_ZP_ZPX_AB_ABX(unsigned char mode){
        switch (mode) {
            case modeAccumulator:
                return memAccumulator;
                
            case modeZeroPage:
                return memZeroPage;
                
            case modeZeroPageX:
                return memZeroPageX;
                
            case modeAbsolute:
                return memAbsolute;
                
            case modeAbsoluteX:
                return memAbsoluteX;
                
            default:
                printf("Error (undefined mode): %d", (int)mode);
                return 0;
        }
    }
    
    void setmem_A_ZP_ZPX_AB_ABX(unsigned char mode, unsigned char value){
        switch (mode) {
            case modeAccumulator:
                memAccumulator = value;
                break;
                
            case modeZeroPage:
                memZeroPage = value;
                break;
                
            case modeZeroPageX:
                memZeroPageX = value;
                break;
                
            case modeAbsolute:
                memAbsolute = value;
                break;
                
            case modeAbsoluteX:
                memAbsoluteX = value;
                break;
                
            default:
                printf("Error (undefined mode): %d", (int)mode);
                break;
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
                printf("Error (undefined mode): %d", (int)mode);
                return 1;
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
                printf("Error (undefined mode): %d", (int)mode);
                return 1;
        }
    }
    
    int executeNextOpcode(){
        unsigned char opcode = memory[pc];
        
        printf("Executing opcode 0x%x\n", opcode);
        
        switch(opcode){
            
            caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeADC):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeADC) + C;
                C = ((unsigned short)m+A) > 0xFF;
                V = ((unsigned short)m+A) > 0x7F;
                A = m + A;
                Z = !A;//is 0
                N = A & 0x80;//the msb bit is set
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeADC);
                return EXIT_CODE_NONE;
            }
            
            caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeAND):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeAND);
                A = m & A;
                Z = !A;
                N = A & 0x80;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeAND);
                return EXIT_CODE_NONE;
            }
            
            caseModes_A_ZP_ZPX_AB_ABX(codeASL):{
                unsigned char m = mem_A_ZP_ZPX_AB_ABX(opcode - codeASL);
                C = m & 0x80;
                Z = !(m << 1);
                N = (m & 0x80);
                setmem_A_ZP_ZPX_AB_ABX(opcode - codeASL, m << 1);
                pc += opcodeLength_A_ZP_ZPX_AB_ABX(opcode - codeASL);
                return EXIT_CODE_NONE;
            }
            
            case codeBPL:{
                if(!N){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                return EXIT_CODE_NONE;
            }
            
            case codeBMI:{
                if(N){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                return EXIT_CODE_NONE;
            }
            
            case codeBVC:{
                if(!V){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                return EXIT_CODE_NONE;
            }
            
            case codeBVS:{
                if(V){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                return EXIT_CODE_NONE;
            }
            
            case codeBCC:{
                if(!C){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                return EXIT_CODE_NONE;
            }
            
            case codeBCS:{
                if(C){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                return EXIT_CODE_NONE;
            }
            
            case codeBNE:{
                if(!Z){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                return EXIT_CODE_NONE;
            }
            
            case codeBEQ:{
                if(Z){
                    pc += (signed char)byteAfterOpcode;
                }else{
                    pc += 2;
                }
                return EXIT_CODE_NONE;
            }
            
            caseModes_ZP_AB(codeBIT):{
                unsigned short m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeBIT);
                m += A;
                V = m > 0x7F;
                N = m & 0x80;
                Z = true;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeBIT);
                return EXIT_CODE_NONE;
            }
            
            case codeBRK:{
                return EXIT_CODE_BREAK;
            }
            
            caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeCMP):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCMP);
                C = A >= m;
                Z = !(A-m);//TODO figure out if this is correct
                N = (A-m) & 0x80;//TODO figure out if this is correct
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCMP);
                return EXIT_CODE_NONE;
            }
            
            caseModes_I_ZP_AB(codeCPX):{
                unsigned char m = mem_I_ZP_AB(opcode - codeCPX);
                C = X >= m;
                Z = !(X-m);//TODO figure out if this is correct
                N = (X-m) & 0x80;//TODO figure out if this is correct
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCPX);
                return EXIT_CODE_NONE;
            }
            
            caseModes_I_ZP_AB(codeCPY):{
                unsigned char m = mem_I_ZP_AB(opcode - codeCPY);
                C = Y >= m;
                Z = !(Y-m);//TODO figure out if this is correct
                N = (Y-m) & 0x80;//TODO figure out if this is correct
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCPY);
                return EXIT_CODE_NONE;
            }
            
            caseModes_ZP_ZPX_AB_ABX(codeDEC):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeDEC);
                setmem_A_ZP_ZPX_AB_ABX(opcode - codeDEC, m-1);
                Z = !(m-1);
                N = (m-1) & 0x80;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeDEC);
                return EXIT_CODE_NONE;
            }
            
            case codeDEX:{
                X -= 1;
                Z = !(X);
                N = (X) & 0x80;
                pc ++;
                return EXIT_CODE_NONE;
            }
            
            case codeDEY:{
                Y -= 1;
                Z = !(Y);
                N = (Y) & 0x80;
                pc ++;
                return EXIT_CODE_NONE;
            }
            
            caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeEOR):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeEOR);
                A = m ^ A;
                Z = !A;
                N = A & 0x80;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeEOR);
                return EXIT_CODE_NONE;
            }
            
            caseModes_ZP_ZPX_AB_ABX(codeINC):{
                unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeINC);
                setmem_A_ZP_ZPX_AB_ABX(opcode - codeINC, m+1);
                Z = !(m+1);
                N = (m+1) & 0x80;
                pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeINC);
                return EXIT_CODE_NONE;
            }
            
            case codeINX:{
                X += 1;
                Z = !(X);
                N = (X) & 0x80;
                pc ++;
                return EXIT_CODE_NONE;
            }
            
            case codeINY:{
                Y += 1;
                Z = !(Y);
                N = (Y) & 0x80;
                pc ++;
                return EXIT_CODE_NONE;
            }
            
            case codeCLC:{
                C = false;
                pc++;
                return EXIT_CODE_NONE;
            }
            
            case codeSEC:{
                C = true;
                pc++;
                return EXIT_CODE_NONE;
            }
            
            case codeCLI:{
                I = false;
                pc++;
                return EXIT_CODE_NONE;
            }
            
            case codeSEI:{
                I = true;
                pc++;
                return EXIT_CODE_NONE;
            }
            
            case codeCLV:{
                V = false;
                pc++;
                return EXIT_CODE_NONE;
            }
            
            case codeCLD:{
                D = false;
                pc++;
                return EXIT_CODE_NONE;
            }
            
            case codeSED:{
                D = true;
                pc++;
                return EXIT_CODE_NONE;
            }
            
            caseModes_AB_I(codeJMP):{
                pc = mem_AB_I(opcode - codeJMP);
                return EXIT_CODE_NONE;
            }
            
            case codeJSR:{
                pcStack.push(pc+3);
                pc = (byteAfterOpcode) | ((unsigned short)byte2AfterOpcode << 4);
                return EXIT_CODE_NONE;
            }
            
            case codeRTS:{
                pc = pcStack.top();
                pcStack.pop();
                return EXIT_CODE_NONE;
            }
            
        }
        
        printf("Execution Failed\n");
        
        return EXIT_CODE_ERROR;
    }
    
};



#endif /* defined(__NESEmu__Chip__) */