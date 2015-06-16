
#include "Chip.hpp"

const char* Chip::opcodeName(unsigned char code){
    switch(code){
        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeADC):{
            return "ADC";
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeAND):{
            return "AND";
        }

        caseModes_A_ZP_ZPX_AB_ABX(codeASL):{
            return "ASL";
        }

        case codeBPL:{
            return "BPL";
        }

        case codeBMI:{
            return "BMI";
        }

        case codeBVC:{
            return "BVC";
        }

        case codeBVS:{
            return "BVS";
        }

        case codeBCC:{
            return "BCC";
        }

        case codeBCS:{
            return "BCS";
        }

        case codeBNE:{
            return "BNE";
        }

        case codeBEQ:{
            return "BEQ";
        }

        caseModes_ZP_AB(codeBIT):{
            return "BIT";
        }

        case codeBRK:{
            return "BRK";
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeCMP):{
            return "CMP";
        }

        caseModes_I_ZP_AB(codeCPX):{
            return "CPX";
        }

        caseModes_I_ZP_AB(codeCPY):{
            return "CPY";
        }

        caseModes_ZP_ZPX_AB_ABX(codeDEC):{
            return "DEC";
        }

        case codeDEX:{
            return "DEX";
        }

        case codeDEY:{
            return "DEY";
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeEOR):{
            return "EOR";
        }

        caseModes_ZP_ZPX_AB_ABX(codeINC):{
            return "INC";
        }

        case codeINX:{
            return "INX";
        }

        case codeINY:{
            return "INY";
        }

        case codeCLC:{
            return "CLC";
        }

        case codeSEC:{
            return "SEC";
        }

        case codeCLI:{
            return "CLT";
        }

        case codeSEI:{
            return "SEI";
        }

        case codeCLV:{
            return "CLV";
        }

        case codeCLD:{
            return "CLD";
        }

        case codeSED:{
            return "SED";
        }

        caseModes_AB_I(codeJMP):{
            return "JMP";
        }

        case codeJSR:{
            return "JSR";
        }

        case codeRTS:{
            return "RTS";
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeLDA):{
            return "LDA";
        }

        caseModes_I_ZP_ZPY_AB_ABY(codeLDX):{
            return "LDX";
        }

        caseModes_I_ZP_ZPX_AB_ABX(codeLDY):{
            return "LDY";
        }

        caseModes_A_ZP_ZPX_AB_ABX(codeLSR):{
            return "LSR";
        }

        case codeNOP:{
            return "NOP";
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeORA):{
            return "ORA";
        }

        case codePHA:{
            return "PHA";
        }

        case codePHP:{
            return "PHP";
        }

        case codePLA:{
            return "PLA";
        }

        case codePLP:{
            return "PLP";
        }

        caseModes_A_ZP_ZPX_AB_ABX(codeROL):{
            return "ROL";
        }

        caseModes_A_ZP_ZPX_AB_ABX(codeROR):{
            return "ROR";
        }

        case codeRTI:{
            return "RTI";
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeSBC):{
            return "SBC";
        }

        caseModes_ZP_ZPX_AB_ABX_ABY_IX_IY(codeSTA):{
            return "STA";
        }

        caseModes_ZP_ZPY_AB(codeSTX):{
            return "STX";
        }

        caseModes_ZP_ZPX_AB(codeSTY):{
            return "STY";
        }

        case codeTAX:{
            return "TAX";
        }

        case codeTAY:{
            return "TAY";
        }

        case codeTSX:{
            return "TSX";
        }

        case codeTXA:{
            return "TXA";
        }

        case codeTYA:{
            return "TYA";
        }

        case codeTXS:{
            return "TXS";
        }

        default:
            return "???";
    }
}

unsigned char Chip::getMemory(unsigned short address){
    if(address < 0x2000){
        return memory[address % 0x800];
    }else if(address >= 0x2000 && address < 0x4000){
        return memory[(address % 8) + 0x800];
    }else{
        return memory[address];
    }
}

void Chip::setMemory(unsigned short address, unsigned char value){
    if(address < 0x2000){
        memory[address % 0x800] = value;
    }else if(address >= 0x2000 && address < 0x4000){
        memory[address % 8] = value;
    }
}

const unsigned short Chip::mem_AB_I(unsigned char mode){
    switch (mode) {
        case modeAbsolute:
            return getMemAbsolute;

        case modeIndirect:
            return getMemIndirect;

        default:
            throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
    }
}

const unsigned char Chip::mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode){
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

const unsigned char Chip::mem_I_ZP_AB(unsigned char mode){
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

const unsigned char Chip::mem_ZP_ZPX_AB(unsigned char mode){
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

const unsigned char Chip::mem_ZP_ZPY_AB(unsigned char mode){
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

const unsigned char Chip::mem_A_ZP_ZPX_AB_ABX(unsigned char mode){
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

const unsigned char Chip::mem_I_ZP_ZPX_AB_ABX(unsigned char mode){
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

const unsigned char Chip::mem_A_ZP_ZPY_AB_ABY(unsigned char mode){
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

const unsigned char Chip::mem_I_ZP_ZPY_AB_ABY(unsigned char mode){
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

void Chip::setmem_A_ZP_ZPX_AB_ABX(unsigned char mode, unsigned char value){
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

void Chip::setmem_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode, unsigned char value){
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

void Chip::setmem_ZP_ZPX_AB(unsigned char mode, unsigned char value){
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

void Chip::setmem_ZP_ZPY_AB(unsigned char mode, unsigned char value){
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

unsigned char Chip::opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode){
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

unsigned char Chip::opcodeLength_A_ZP_ZPX_AB_ABX(unsigned char mode){
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

unsigned char Chip::opcodeLength_I_ZP_ZPX_AB_ABX(unsigned char mode){
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

unsigned char Chip::opcodeLength_A_ZP_ZPY_AB_ABY(unsigned char mode){
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

unsigned char Chip::opcodeLength_I_ZP_ZPY_AB_ABY(unsigned char mode){
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

unsigned char Chip::opcodeLength_ZP_ZPX_AB(unsigned char mode){
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

unsigned char Chip::opcodeLength_ZP_ZPY_AB(unsigned char mode){
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

void Chip::pushToStack(unsigned char value){
    stack[stackPointer] = value;
    if(stackPointer < 0xFF){
        stackPointer++;
    }else{
        throw EXIT_ERR_STACK_OVERFLOW;
    }
}

unsigned char Chip::popFromStack(){
    if(stackPointer > 0){
        stackPointer--;
    }else{
        throw EXIT_ERR_STACK_UNDERFLOW;
    }
    return stack[stackPointer];
}

void Chip::executeNextOpcode(){
    opcode = memory[pc];
    byteAfterOpcode = getMemory(pc+1);
    byte2AfterOpcode = getMemory(pc+2);

    printf("Executing opcode %s (0x%X) [%x %x]\n", opcodeName(opcode), opcode, byteAfterOpcode, byte2AfterOpcode);

    switch(opcode){

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeADC):{
            unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeADC);
            unsigned short temp = m + A + C;
            C = temp > 0xFF;
            V = (m & 0x80) >> 7;
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
            pc += 2;
            if(!N){
                pc += (char)byteAfterOpcode;
            }
            break;
        }

        case codeBMI:{
            pc += 2;
            if(N){
                pc += (char)byteAfterOpcode;
            }
            break;
        }

        case codeBVC:{
            pc += 2;
            if(!V){
                pc += (char)byteAfterOpcode;
            }
            break;
        }

        case codeBVS:{
            pc += 2;
            if(V){
                pc += (char)byteAfterOpcode;
            }
            break;
        }

        case codeBCC:{
            pc += 2;
            if(!C){
                pc += (char)byteAfterOpcode;
            }
            break;
        }

        case codeBCS:{
            pc += 2;
            if(C){
                pc += (char)byteAfterOpcode;
            }
            break;
        }

        case codeBNE:{
            pc += 2;
            if(!Z){
                pc += (char)byteAfterOpcode;
            }
            break;
        }

        case codeBEQ:{
            pc += 2;
            if(Z){
                pc += (char)byteAfterOpcode;
            }
            break;
        }

        caseModes_ZP_AB(codeBIT):{
            unsigned short m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeBIT);
            V = (m & 0x40) >> 6;
            N = (m & 0x80) >> 7;
            Z = !(m & A);
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
            Z = !(A-m);
            N = ((A-m) & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCMP);
            break;
        }

        caseModes_I_ZP_AB(codeCPX):{
            unsigned char m = mem_I_ZP_AB(opcode - codeCPX);
            C = X >= m;
            Z = !(X-m);
            N = ((X-m) & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCPX);
            break;
        }

        caseModes_I_ZP_AB(codeCPY):{
            unsigned char m = mem_I_ZP_AB(opcode - codeCPY);
            C = Y >= m;
            Z = !(Y-m);
            N = ((Y-m) & 0x80) >> 7;
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
            pc += 3;
            pushToStack(pc & 0xFF);
            pushToStack((pc >> 8) & 0xFF);
            pc = (byteAfterOpcode) | ((unsigned short)byte2AfterOpcode << 8);
            break;
        }

        case codeRTS:{
            pc = (popFromStack() << 8);
            pc |= popFromStack();
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
            pushToStack(S());
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
            N = (flags >> 7) & 0x1;
            V = (flags >> 6) & 0x1;

            D = (flags >> 3) & 0x1;
            I = (flags >> 2) & 0x1;
            Z = (flags >> 1) & 0x1;
            C = (flags >> 0) & 0x1;
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
            //unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSTA);
            setmem_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSTA, A);
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
