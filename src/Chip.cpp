
#include "Chip.hpp"

void executePPU(Chip* chip){
    while(chip->runPPU){

    }
}

void Chip::startPPU(){
    runPPU = true;
    threadPPU = thread(executePPU, this);
}

void Chip::stopPPU(){
    runPPU = false;
    threadPPU.join();
}

void Chip::setPCToResetVector(){    
    pc = (getMemory(RESET_VECTOR+1, true) << 8) | getMemory(RESET_VECTOR, true);
}

void Chip::cycle(unsigned char cycles){
    cpuCycles += cycles;
}

void Chip::reset(bool resetRAM, bool resetPPUandAPUandIO, bool resetPRGRAM){

    for(unsigned char i=0;i<8;i++){
        controllerP1Buffer[i] = false;
        controllerP2Buffer[i] = false;
    }
    controllerP1Index = 0;
    controllerP2Index = 0;

    ERR_META = 0;

    pc = 0x0000;

    A = 0;
    X = 0;
    Y = 0;

    CPU_C = false;
    CPU_V = false;
    CPU_Z = false;
    CPU_N = false;
    CPU_D = false;
    CPU_I = true;
    CPU_Flag4 = false;
    CPU_Flag5 = true;

    PPU_CTRL = 0;
    PPU_MASK = 0;
    PPU_STATUS = 0;
    PPU_STATUS_V = 1;
    PPU_STATUS_O = 1;
    OAM_ADDR = 0;
    OAM_DATA = 0;

    PPU_SCROLL_INDEX = 0;
    PPU_SCROLL_X = 0;
    PPU_SCROLL_Y = 0;

    PPU_ADDR_INDEX = 0;
    PPU_ADDR_MSB = 0;
    PPU_ADDR_LSB = 0;

    PPU_DATA = 0;
    OAM_DMA = 0;

    if(resetRAM){
        for(unsigned int i=0;i<0x800;i++){
            memoryRAM[i] = 0;
        }
    }

    if(resetPPUandAPUandIO){
        for(unsigned int i=0;i<0x8;i++){
            memoryPPURegisters[i] = 0;
        }
        for(unsigned int i=0;i<0x20;i++){
            memoryAPUandIORegisters[i] = 0;
        }
    }

    if(resetPRGRAM){
        for(unsigned int i=0;i<0xBFE0;i++){
            memoryProgramMemory[i] = 0;
        }
    }

    stackPointer = 0xFD;

    for(unsigned short i=0;i<0x100;i++){
        stack[i] = 0;
    }
    
    cpuCycles = 0;

    opcode = 0;
    byteAfterOpcode = 0;
    byte2AfterOpcode = 0;

}

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

unsigned char Chip::opcodeLength(unsigned char code){
    switch(code){
        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeADC):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeADC);
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeAND):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeAND);
        }

        caseModes_A_ZP_ZPX_AB_ABX(codeASL):{
            return opcodeLength_A_ZP_ZPX_AB_ABX(code-codeASL);
        }

        case codeBPL:{
            return 2;
        }

        case codeBMI:{
            return 2;
        }

        case codeBVC:{
            return 2;
        }

        case codeBVS:{
            return 2;
        }

        case codeBCC:{
            return 2;
        }

        case codeBCS:{
            return 2;
        }

        case codeBNE:{
            return 2;
        }

        case codeBEQ:{
            return 2;
        }

        caseModes_ZP_AB(codeBIT):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeBIT);
        }

        case codeBRK:{
            return 1;
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeCMP):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeCMP);
        }

        caseModes_I_ZP_AB(codeCPX):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeCPX);
        }

        caseModes_I_ZP_AB(codeCPY):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeCPY);
        }

        caseModes_ZP_ZPX_AB_ABX(codeDEC):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeDEC);
        }

        case codeDEX:{
            return 1;
        }

        case codeDEY:{
            return 1;
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeEOR):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeEOR);
        }

        caseModes_ZP_ZPX_AB_ABX(codeINC):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeINC);
        }

        case codeINX:{
            return 1;
        }

        case codeINY:{
            return 1;
        }

        case codeCLC:{
            return 1;
        }

        case codeSEC:{
            return 1;
        }

        case codeCLI:{
            return 1;
        }

        case codeSEI:{
            return 1;
        }

        case codeCLV:{
            return 1;
        }

        case codeCLD:{
            return 1;
        }

        case codeSED:{
            return 1;
        }

        caseModes_AB_I(codeJMP):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeJMP);
        }

        case codeJSR:{
            return 3;
        }

        case codeRTS:{
            return 1;
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeLDA):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeLDA);
        }

        caseModes_I_ZP_ZPY_AB_ABY(codeLDX):{
            return opcodeLength_I_ZP_ZPY_AB_ABY(code-codeLDX);
        }

        caseModes_I_ZP_ZPX_AB_ABX(codeLDY):{
            return opcodeLength_I_ZP_ZPX_AB_ABX(code-codeLDY);
        }

        caseModes_A_ZP_ZPX_AB_ABX(codeLSR):{
            return opcodeLength_A_ZP_ZPX_AB_ABX(code-codeLSR);
        }

        case codeNOP:{
            return 1;
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeORA):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeORA);
        }

        case codePHA:{
            return 1;
        }

        case codePHP:{
            return 1;
        }

        case codePLA:{
            return 1;
        }

        case codePLP:{
            return 1;
        }

        caseModes_A_ZP_ZPX_AB_ABX(codeROL):{
            return opcodeLength_A_ZP_ZPX_AB_ABX(code-codeROL);
        }

        caseModes_A_ZP_ZPX_AB_ABX(codeROR):{
            return opcodeLength_A_ZP_ZPX_AB_ABX(code-codeROR);
        }

        case codeRTI:{
            return 1;
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeSBC):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeSBC);
        }

        caseModes_ZP_ZPX_AB_ABX_ABY_IX_IY(codeSTA):{
            return opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(code-codeSTA);
        }

        caseModes_ZP_ZPY_AB(codeSTX):{
            return opcodeLength_ZP_ZPY_AB(code-codeSTX);
        }

        caseModes_ZP_ZPX_AB(codeSTY):{
            return opcodeLength_ZP_ZPX_AB(code-codeSTY);
        }

        case codeTAX:{
            return 1;
        }

        case codeTAY:{
            return 1;
        }

        case codeTSX:{
            return 1;
        }

        case codeTXA:{
            return 1;
        }

        case codeTYA:{
            return 1;
        }

        case codeTXS:{
            return 1;
        }

        default:
            return 3;
    }
}

const unsigned short Chip::address_AB_I(unsigned char mode){
    switch (mode) {
        case modeAbsolute:
            return addressAbsolute;

        case modeIndirect:
            return addressIndirect;

        default:
            printf("func: address_AB_I\n");
            ERR_META = mode;
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
            printf("func: mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY\n");
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
            printf("func: mem_I_ZP_AB\n");
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
            printf("func: mem_ZP_ZPX_AB\n");
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
            printf("func: mem_ZP_ZPY_AB\n");
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
            printf("func: mem_A_ZP_ZPX_AB_ABX\n");
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
            printf("func: mem_I_ZP_ZPX_AB_ABX\n");
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
            printf("func: mem_A_ZP_ZPY_AB_ABY\n");
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
            printf("func: mem_I_ZP_ZPY_AB_ABY\n");
            ERR_META = mode;
            throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
    }
}

const unsigned char Chip::time_I_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode){
    switch (mode) {
        case modeImmediate:
            return 2;

        case modeZeroPage:
            return 3;

        case modeZeroPageX:
            return 4;

        case modeAbsolute:
            return 4;

        case modeAbsoluteX:
            return 4+(((addressAbsoluteX & 0xFF00) >> 8) != byte2AfterOpcode);

        case modeAbsoluteY:
            return 4+(((addressAbsoluteY & 0xFF00) >> 8) != byte2AfterOpcode);

        case modeIndirectX:
            return 6;

        case modeIndirectY:
            return 5+(((addressIndirectY & 0xFF00) >> 8) != byte2AfterOpcode);

        default:
            printf("func: time_I_ZP_ZPX_AB_ABX_ABY_IX_IY\n");
            ERR_META = mode;
            throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
    }
}

const unsigned char Chip::time_larger_I_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode){
    switch (mode) {

        case modeZeroPage:
            return 5;

        case modeZeroPageX:
            return 6;

        case modeAbsolute:
            return 6;

        case modeAbsoluteX:
            return 7;

        default:
            printf("func: time_I_ZP_ZPX_AB_ABX_ABY_IX_IY\n");
            ERR_META = mode;
            throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
    }
}

const unsigned char Chip::time_other_I_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode){
    switch (mode) {

        case modeZeroPage:
            return 3;

        case modeZeroPageX:
            return 4;

        case modeAbsolute:
            return 4;

        case modeAbsoluteX:
            return 5;

        case modeAbsoluteY:
            return 5;

        case modeIndirectX:
            return 6;

        case modeIndirectY:
            return 6;

        default:
            printf("func: time_I_ZP_ZPX_AB_ABX_ABY_IX_IY\n");
            ERR_META = mode;
            throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
    }
}

const unsigned char Chip::time_I_ZP_AB(unsigned char mode){
    switch (mode) {
        case modeImmediate2:
            return 2;

        case modeZeroPage:
            return 3;

        case modeAbsolute:
            return 4;

        default:
            printf("func: time_I_ZP_AB\n");
            ERR_META = mode;
            throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
    }
}

const unsigned char Chip::time_ZP_ZPX_AB(unsigned char mode){
    switch (mode) {
        case modeZeroPage2:
            return 3;

        case modeZeroPageX2:
            return 4;

        case modeAbsolute2:
            return 4;

        default:
            printf("func: time_ZP_ZPX_AB\n");
            ERR_META = mode;
            throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
    }
}

const unsigned char Chip::time_ZP_ZPY_AB(unsigned char mode){
    switch (mode) {
        case modeZeroPage2:
            return 3;

        case modeZeroPageY2:
            return 4;

        case modeAbsolute2:
            return 4;

        default:
            printf("func: time_ZP_ZPY_AB\n");
            ERR_META = mode;
            throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
    }
}

const unsigned char Chip::time_A_ZP_ZPX_AB_ABX(unsigned char mode){
    switch (mode) {
        case modeAccumulator:
            return 2;

        case modeZeroPage:
            return 5;

        case modeZeroPageX:
            return 6;

        case modeAbsolute:
            return 6;

        case modeAbsoluteX:
            return 7;

        default:
            printf("func: time_A_ZP_ZPX_AB_ABX\n");
            ERR_META = mode;
            throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
    }
}

const unsigned char Chip::time_I_ZP_ZPX_AB_ABX(unsigned char mode){
    switch (mode) {
        case modeImmediate2:
            return 2;

        case modeZeroPage:
            return 3;

        case modeZeroPageX:
            return 4;

        case modeAbsolute:
            return 4;

        case modeAbsoluteX:
            return 4+(((addressAbsoluteX & 0xFF00) >> 8) != byte2AfterOpcode);

        default:
            printf("func: time_I_ZP_ZPX_AB_ABX\n");
            ERR_META = mode;
            throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
    }
}

const unsigned char Chip::time_I_ZP_ZPY_AB_ABY(unsigned char mode){
    switch (mode) {
        case modeImmediate2:
            return 2;

        case modeZeroPage:
            return 3;

        case modeZeroPageY:
            return 4;

        case modeAbsolute:
            return 4;

        case modeAbsoluteY:
            return 4+(((addressAbsoluteY & 0xFF00) >> 8) != byte2AfterOpcode);

        default:
            printf("func: time_I_ZP_ZPY_AB_ABY\n");
            ERR_META = mode;
            throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
    }
}

const unsigned char Chip::time_AB_I(unsigned char mode){
    switch (mode) {

        case modeAbsolute:
            return 3;

        case modeIndirect:
            return 5;

        default:
            printf("func: time_AB_I\n");
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
            printf("func: setmem_A_ZP_ZPX_AB_ABX\n");
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
            printf("func: setmem_ZP_ZPX_AB_ABX_ABY_IX_IY\n");
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
            printf("func: setmem_ZP_ZPX_AB\n");
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
            printf("func: setmem_ZP_ZPY_AB\n");
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
            printf("func: opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY\n");
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
            printf("func: opcodeLength_A_ZP_ZPX_AB_ABX\n");
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
            printf("func: opcodeLength_I_ZP_ZPX_AB_ABX\n");
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
            printf("func: opcodeLength_A_ZP_ZPY_AB_ABY\n");
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
            printf("func: opcodeLength_I_ZP_ZPY_AB_ABY\n");
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
            printf("func: opcodeLength_ZP_ZPX_AB\n");
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
            printf("func: opcodeLength_ZP_ZPY_AB\n");
            ERR_META = mode;
            throw EXIT_ERR_UNKNOWN_ADDRESS_MODE;
    }
}
unsigned char Chip::getPPUMemory(unsigned short address, bool passive){
    switch(address){

        default:{
            if(address < 0x2000){
                return ppuMemoryPatternTables[address];
            }else if(address >= 0x2000 && address < 0x3000){
                return ppuMemoryPatternTables[address-0x2000];
            }else if(address >= 0x3000 && address < 0x3F00){
                return ppuMemoryPatternTables[address-0x3000];
            }else{
                ERR_META = address;
                throw EXIT_ERR_MEMORY_ADDRESS_OUT_OF_RANGE;
            }
        }

    }
}

void Chip::setPPUMemory(unsigned short address, unsigned char value){

}

unsigned char Chip::getMemory(unsigned short address, bool passive){
    switch(address){

        case 0x2000:{
            if(passive)return PPU_CTRL;
            return 0;
        }

        case 0x2001:{
            if(passive)return PPU_MASK;
            return 0;
        }

        case 0x2002:{
            if(!passive){
                PPU_SCROLL_INDEX = 0;
                PPU_ADDR_INDEX = 0;
            }
            return PPU_STATUS;
        }

        case 0x2003:{
            if(passive)return OAM_ADDR;
            return 0;
        }

        case 0x2004:{
            return OAM_DATA;
        }

        case 0x2005:{
            if(passive){
                switch(PPU_SCROLL_INDEX){
                    case 0:
                        return PPU_SCROLL_X;
                    case 1:
                        return PPU_SCROLL_Y;
                }
            }
            return 0;
        }

        case 0x2006:{
            if(passive){
                switch(PPU_ADDR_INDEX){
                    case 0:
                        return PPU_ADDR_MSB;
                    case 1:
                        return PPU_ADDR_LSB;
                }
            }
            return 0;
        }

        case 0x2007:{
            return PPU_DATA;
        }

        case 0x4014:{
            if(passive)return OAM_DMA;
            return 0;
        }

        case 0x4016:{
            if(controllerP1Index < 0x8){
                unsigned char v = (controllerP1Buffer[controllerP1Index] ? 0x1 : 0x0) | 0xA0;
                if(!passive)controllerP1Index++;
                return v;
            }else{
                return 0xA1;
            }
        }

        case 0x4017:{
            if(controllerP2Index < 0x8){
                unsigned char v = (controllerP2Buffer[controllerP2Index] ? 0x1 : 0x0) | 0xA0;
                if(!passive)controllerP2Index++;
                return v;
            }else{
                return 0xA1;
            }
        }

        default:{
            if(address >= 0x100 && address <= 0x1FF){
                return stack[address - 0x100];
            }else if(address < 0x2000){
                return memoryRAM[address % 0x800];
            }else if(address >= 0x2000 && address < 0x4000){
                return memoryPPURegisters[address % 0x8];
            }else if(address >= 0x4000 && address < 0x4020){
                return memoryAPUandIORegisters[address - 0x4000];
            }else if(address >= 0x4020){
                return memoryProgramMemory[address - 0x4020];
            }else{
                ERR_META = address;
                throw EXIT_ERR_MEMORY_ADDRESS_OUT_OF_RANGE;
            }
        }

    }
}

void Chip::setMemory(unsigned short address, unsigned char value){
    switch(address){

        case 0x2000:{
            PPU_CTRL = value;
            return;
        }

        case 0x2001:{
            PPU_MASK = value;
            return;
        }

        case 0x2002:{
            return;
        }

        case 0x2003:{
            OAM_ADDR = value;
            return;
        }

        case 0x2004:{
            OAM_DATA = value;
            OAM_ADDR ++;
            return;
        }

        case 0x2005:{
            switch(PPU_SCROLL_INDEX){
                case 0:
                    PPU_SCROLL_X = value;
                    PPU_SCROLL_INDEX++;
                    return;
                case 1:
                    PPU_SCROLL_Y = value;
                    PPU_SCROLL_INDEX++;
                    return;
            }
            return;
        }

        case 0x2006:{
            switch(PPU_ADDR_INDEX){
                case 0:
                    PPU_ADDR_MSB = value;
                    PPU_ADDR_INDEX++;
                    return;
                case 1:
                    PPU_ADDR_LSB = value;
                    PPU_ADDR_INDEX++;
                    return;
            }
            return;
        }

        case 0x2007:{
            PPU_DATA = value;
        }

        case 0x4014:{
            OAM_DMA = value;
        }

        case 0x4016:{
            if(value & 0x1){
                controllerP1Index = 0;
            }
            return;
        }

        default:{
            if(address >= 0x100 && address <= 0x1FF){
                stack[address - 0x100] = value;
            }else if(address < 0x2000){
                memoryRAM[address % 0x800] = value;
            }else if(address >= 0x2000 && address < 0x4000){
                memoryPPURegisters[address % 0x8] = value;
            }else if(address >= 0x4000 && address < 0x4020){
                memoryAPUandIORegisters[address - 0x4000] = value;
            }else if(address >= 0x4020){
                memoryProgramMemory[address - 0x4020] = value;
            }else{
                ERR_META = address;
                throw EXIT_ERR_MEMORY_ADDRESS_OUT_OF_RANGE;
            }
            return;
        }
    }
}

void Chip::setCharROM(unsigned address, unsigned char value){
    //todo
}

void Chip::pushToStack(unsigned char value){
    stack[stackPointer] = value;
    if(stackPointer > 0x00){
        stackPointer--;
    }else{
        throw EXIT_ERR_STACK_OVERFLOW;
    }
}

unsigned char Chip::popFromStack(){
    if(stackPointer < 0xFF){
        stackPointer++;
    }else{
        throw EXIT_ERR_STACK_UNDERFLOW;
    }
    return stack[stackPointer];
}

bool differentPages(unsigned short a, unsigned short b){
    return (a & 0xFF00) != (b & 0xFF00);
}

void Chip::prepareOpcode(){
    opcode = getMemory(pc);
    byteAfterOpcode = getMemory(pc+1);
    byte2AfterOpcode = getMemory(pc+2);
}

void Chip::executeOpcode(){
    
    if(getMemory(0x6001, false) == 0xDE && getMemory(0x6002, false) == 0xB0 && getMemory(0x6003, false) == 0x61){
    
        string s = "";
        int i = 0;
        
        while(true){
            char c = (char)getMemory(0x6004+i, false);
            if(c == '\0'){
                break;
            }
            s += c;
            i++;
        }
        
        printf("(%2X %2X %2X %2X) [%d] \"%s\"\n", getMemory(0x6000, false), getMemory(0x6001, false), getMemory(0x6002, false), getMemory(0x6003, false), i, s.c_str());
        //printf("%s\n", memoryProgramMemory - 0x4020 + 0x6004);
    }
    
    if(!CPU_I){
        CPU_I = true;
        pushToStack((pc&0xFF00) >> 8);
        pushToStack(pc&0x00FF);
        pushToStack(CPU_S_GET(false, true));
        pc = (getMemory(IRQ_VECTOR+1) << 8) | getMemory(IRQ_VECTOR);
        cycle(6);
    }

    switch(opcode){

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeADC):{
            unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeADC);
            unsigned short temp = m + A + CPU_C;
            CPU_C = temp > 0xFF;
            CPU_V = ~(A ^ m) & (A ^ temp) & 0x80;
            A = (unsigned char)temp;
            CPU_Z = !A;
            CPU_N = (A & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeADC);
            cycle(time_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeADC));
            break;
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeAND):{
            unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeAND);
            A = m & A;
            CPU_Z = !A;
            CPU_N = (A & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeAND);
            cycle(time_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeAND));
            break;
        }

        caseModes_A_ZP_ZPX_AB_ABX(codeASL):{
            unsigned char m = mem_A_ZP_ZPX_AB_ABX(opcode - codeASL);
            CPU_C = m & 0x80;
            m = m << 1;
            CPU_Z = !m;
            CPU_N = (m & 0x80) >> 7;
            setmem_A_ZP_ZPX_AB_ABX(opcode - codeASL, m);
            pc += opcodeLength_A_ZP_ZPX_AB_ABX(opcode - codeASL);
            cycle(time_A_ZP_ZPX_AB_ABX(opcode - codeASL));
            break;
        }

        case codeBPL:{
            pc += 2;
            if(!CPU_N){
                if(differentPages(pc, pc+(char)byteAfterOpcode)){
                    cycle(1);
                }
                pc += (char)byteAfterOpcode;
                cycle(1);
            }
            cycle(2);
            break;
        }

        case codeBMI:{
            pc += 2;
            if(CPU_N){
                if(differentPages(pc, pc+(char)byteAfterOpcode)){
                    cycle(1);
                }
                pc += (char)byteAfterOpcode;
                cycle(1);
            }
            cycle(2);
            break;
        }

        case codeBVC:{
            pc += 2;
            if(!CPU_V){
                pc += (char)byteAfterOpcode;
            }
            cycle(3);//BVC always 3, unlike other B__ instructions
            break;
        }

        case codeBVS:{
            pc += 2;
            if(CPU_V){
                if(differentPages(pc, pc+(char)byteAfterOpcode)){
                    cycle(1);
                }
                pc += (char)byteAfterOpcode;
                cycle(1);
            }
            cycle(2);
            break;
        }

        case codeBCC:{
            pc += 2;
            if(!CPU_C){
                if(differentPages(pc, pc+(char)byteAfterOpcode)){
                    cycle(1);
                }
                pc += (char)byteAfterOpcode;
                cycle(1);
            }
            cycle(2);
            break;
        }

        case codeBCS:{
            pc += 2;
            if(CPU_C){
                if(differentPages(pc, pc+(char)byteAfterOpcode)){
                    cycle(1);
                }
                pc += (char)byteAfterOpcode;
                cycle(1);
            }
            cycle(2);
            break;
        }

        case codeBNE:{
            pc += 2;
            if(!CPU_Z){
                if(differentPages(pc, pc+(char)byteAfterOpcode)){
                    cycle(1);
                }
                pc += (char)byteAfterOpcode;
                cycle(1);
            }
            cycle(2);
            break;
        }

        case codeBEQ:{
            pc += 2;
            if(CPU_Z){
                if(differentPages(pc, pc+(char)byteAfterOpcode)){
                    cycle(1);
                }
                pc += (char)byteAfterOpcode;
                cycle(1);
            }
            cycle(2);
            break;
        }

        caseModes_ZP_AB(codeBIT):{
            unsigned short m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeBIT);
            CPU_V = (m & 0x40) >> 6;
            CPU_N = (m & 0x80) >> 7;
            CPU_Z = !(m & A);
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeBIT);
            cycle(time_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeBIT));
            break;
        }

        case codeBRK:{
            //printf("Old PC before: %X = %X\n", pc, getMemory(pc, false));
            pc += opcodeLength(codeBRK);
            //printf("Old PC after: %X = %X\n", pc, getMemory(pc, false));
            
            CPU_I = true;
            pushToStack((pc&0xFF00) >> 8);
            pushToStack(pc&0x00FF);
            pushToStack(CPU_S_GET(false, true) | 0x8); //send B flag
            pc = (getMemory(IRQ_VECTOR+1) << 8) | getMemory(IRQ_VECTOR);
            //printf("New PC after BReaK: %X = %X\n", pc, getMemory(pc, false));
            cycle(6);
            
            break;
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeCMP):{
            unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCMP);
            CPU_C = A >= m;
            CPU_Z = !(A-m);
            CPU_N = ((A-m) & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCMP);
            cycle(time_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCMP));
            break;
        }

        caseModes_I_ZP_AB(codeCPX):{
            unsigned char m = mem_I_ZP_AB(opcode - codeCPX);
            CPU_C = X >= m;
            CPU_Z = !(X-m);
            CPU_N = ((X-m) & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCPX);
            cycle(time_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCPX));
            break;
        }

        caseModes_I_ZP_AB(codeCPY):{
            unsigned char m = mem_I_ZP_AB(opcode - codeCPY);
            CPU_C = Y >= m;
            CPU_Z = !(Y-m);
            CPU_N = ((Y-m) & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCPY);
            cycle(time_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeCPY));
            break;
        }

        caseModes_ZP_ZPX_AB_ABX(codeDEC):{
            unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeDEC);
            setmem_A_ZP_ZPX_AB_ABX(opcode - codeDEC, m-1);
            CPU_Z = !(m-1);
            CPU_N = ((m-1) & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeDEC);
            cycle(time_larger_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeDEC));
            break;
        }

        case codeDEX:{
            X -= 1;
            CPU_Z = !(X);
            CPU_N = ((X) & 0x80) >> 7;
            pc ++;
            cycle(2);
            break;
        }

        case codeDEY:{
            Y -= 1;
            CPU_Z = !(Y);
            CPU_N = ((Y) & 0x80) >> 7;
            pc ++;
            cycle(2);
            break;
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeEOR):{
            unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeEOR);
            A = m ^ A;
            CPU_Z = !A;
            CPU_N = (A & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeEOR);
            cycle(time_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeEOR));
            break;
        }

        caseModes_ZP_ZPX_AB_ABX(codeINC):{
            unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeINC);
            setmem_A_ZP_ZPX_AB_ABX(opcode - codeINC, m+1);
            CPU_Z = !(m+1);
            CPU_N = ((m+1) & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeINC);
            cycle(time_larger_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeINC));
            break;
        }

        case codeINX:{
            X += 1;
            CPU_Z = !(X);
            CPU_N = ((X) & 0x80) >> 7;
            pc ++;
            cycle(2);
            break;
        }

        case codeINY:{
            Y += 1;
            CPU_Z = !(Y);
            CPU_N = ((Y) & 0x80) >> 7;
            pc ++;
            cycle(2);
            break;
        }

        case codeCLC:{
            CPU_C = false;
            pc++;
            cycle(2);
            break;
        }

        case codeSEC:{
            CPU_C = true;
            pc++;
            cycle(2);
            break;
        }

        case codeCLI:{
            CPU_I = false;
            pc++;
            cycle(2);
            break;
        }

        case codeSEI:{
            CPU_I = true;
            pc++;
            cycle(2);
            break;
        }

        case codeCLV:{
            CPU_V = false;
            pc++;
            cycle(2);
            break;
        }

        case codeCLD:{
            CPU_D = false;
            pc++;
            cycle(2);
            break;
        }

        case codeSED:{
            CPU_D = true;
            pc++;
            cycle(2);
            break;
        }

        caseModes_AB_I(codeJMP):{
            pc = address_AB_I(opcode - codeJMP);
            cycle(time_AB_I(opcode - codeJMP));
            break;
        }

        case codeJSR:{
            pc += 3;
            pushToStack(pc & 0xFF);
            pushToStack((pc >> 8) & 0xFF);
            pc = (byteAfterOpcode) | ((unsigned short)byte2AfterOpcode << 8);
            cycle(6);
            break;
        }

        case codeRTS:{
            pc = (popFromStack() << 8);
            pc |= popFromStack();
            cycle(6);
            break;
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeLDA):{
            unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeLDA);
            A = m;
            CPU_Z = !A;
            CPU_N = (A & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeLDA);
            cycle(time_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeLDA));
            break;
        }

        caseModes_I_ZP_ZPY_AB_ABY(codeLDX):{
            unsigned char m = mem_I_ZP_ZPY_AB_ABY(opcode - codeLDX);
            X = m;
            CPU_Z = !X;
            CPU_N = (X & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPY_AB_ABY(opcode - codeLDX);
            cycle(time_I_ZP_ZPY_AB_ABY(opcode - codeLDX));
            break;
        }

        caseModes_I_ZP_ZPX_AB_ABX(codeLDY):{
            unsigned char m = mem_I_ZP_ZPX_AB_ABX(opcode - codeLDY);
            Y = m;
            CPU_Z = !Y;
            CPU_N = (Y & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX(opcode - codeLDY);
            cycle(time_I_ZP_ZPX_AB_ABX(opcode - codeLDY));
            break;
        }

        caseModes_A_ZP_ZPX_AB_ABX(codeLSR):{
            unsigned char m = mem_A_ZP_ZPX_AB_ABX(opcode - codeLSR);
            CPU_C = m & 0x01;
            m = m >> 1;
            CPU_Z = !m;
            CPU_N = ((m & 0x80)) >> 7;
            setmem_A_ZP_ZPX_AB_ABX(opcode - codeLSR, m);
            pc += opcodeLength_A_ZP_ZPX_AB_ABX(opcode - codeLSR);
            cycle(time_A_ZP_ZPX_AB_ABX(opcode - codeLSR));
            break;
        }

        case codeNOP:{
            //This does nothing
            pc ++;
            cycle(2);
            break;
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeORA):{
            unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeORA);
            A = m | A;
            CPU_Z = !A;
            CPU_N = (A & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeORA);
            cycle(time_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeORA));
            break;
        }

        case codePHA:{
            pushToStack(A);
            pc ++;
            cycle(3);
            break;
        }

        case codePHP:{
            pushToStack(CPU_S_GET(true, true));
            pc ++;
            cycle(3);
            break;
        }

        case codePLA:{
            A = popFromStack();
            CPU_Z = !A;
            CPU_N = (A & 0x80) >> 7;
            pc ++;
            cycle(4);
            break;
        }

        case codePLP:{
            CPU_S_SET(popFromStack(), false, true);
            pc ++;
            cycle(4);
            break;
        }

        caseModes_A_ZP_ZPX_AB_ABX(codeROL):{
            unsigned char m = mem_A_ZP_ZPX_AB_ABX(opcode - codeROL);
            CPU_C = (m & 0x80) >> 7;
            m = m << 1;
            m = m & CPU_C;
            CPU_Z = !m;
            CPU_N = (m & 0x80) >> 7;
            setmem_A_ZP_ZPX_AB_ABX(opcode - codeROL, m);
            pc += opcodeLength_A_ZP_ZPX_AB_ABX(opcode - codeROL);
            cycle(time_A_ZP_ZPX_AB_ABX(opcode - codeROL));
            break;
        }

        caseModes_A_ZP_ZPX_AB_ABX(codeROR):{
            unsigned char m = mem_A_ZP_ZPX_AB_ABX(opcode - codeROR);
            CPU_C = m & 0x1;
            m = m >> 1;
            m = m & (CPU_C << 7);
            CPU_Z = !m;
            CPU_N = (m & 0x80) >> 7;
            setmem_A_ZP_ZPX_AB_ABX(opcode - codeROR, m);
            pc += opcodeLength_A_ZP_ZPX_AB_ABX(opcode - codeROR);
            cycle(time_A_ZP_ZPX_AB_ABX(opcode - codeROR));
            break;
        }

        case codeRTI:{
            CPU_S_SET(popFromStack(), false, true);
            unsigned short low = popFromStack();
            unsigned short high = popFromStack();
            pc = (high << 8) | low;
            cycle(6);
            break;
        }

        caseModes_I_ZP_ZPX_AB_ABX_ABY_IX_IY(codeSBC):{
            unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSBC);
            unsigned short temp = A - m - (!CPU_C);
            CPU_C = temp <= 0xFF;
            CPU_V = (A ^ m) & (A ^ temp) & 0x80;
            A = (unsigned char)temp;
            CPU_Z = !A;
            CPU_N = (A & 0x80) >> 7;
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSBC);
            cycle(time_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSBC));
            break;
        }

        caseModes_ZP_ZPX_AB_ABX_ABY_IX_IY(codeSTA):{
            //unsigned char m = mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSTA);
            setmem_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSTA, A);
            pc += opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSTA);
            cycle(time_other_I_ZP_ZPX_AB_ABX_ABY_IX_IY(opcode - codeSTA));
            break;
        }

        caseModes_ZP_ZPY_AB(codeSTX):{
            setmem_ZP_ZPY_AB(opcode - codeSTX, X);
            pc += opcodeLength_ZP_ZPY_AB(opcode - codeSTX);
            cycle(time_ZP_ZPY_AB(opcode - codeSTX));
            break;
        }

        caseModes_ZP_ZPX_AB(codeSTY):{
            setmem_ZP_ZPX_AB(opcode - codeSTY, Y);
            pc += opcodeLength_ZP_ZPX_AB(opcode - codeSTY);
            cycle(time_ZP_ZPY_AB(opcode - codeSTY));
            break;
        }

        case codeTAX:{
            X = A;
            CPU_Z = !X;
            CPU_N = (X & 0x80) >> 7;
            pc += 1;
            cycle(2);
            break;
        }

        case codeTAY:{
            Y = A;
            CPU_Z = !Y;
            CPU_N = (Y & 0x80) >> 7;
            pc += 1;
            cycle(2);
            break;
        }

        case codeTSX:{
            X = stackPointer;
            CPU_Z = !X;
            CPU_N = (X & 0x80) >> 7;
            pc += 1;
            cycle(2);
            break;
        }

        case codeTXA:{
            A = X;
            CPU_Z = !A;
            CPU_N = (A & 0x80) >> 7;
            pc += 1;
            cycle(2);
            break;
        }

        case codeTYA:{
            A = Y;
            CPU_Z = !A;
            CPU_N = (A & 0x80) >> 7;
            pc += 1;
            cycle(2);
            break;
        }

        case codeTXS:{
            stackPointer = X;
            pc += 1;
            cycle(2);
            break;
        }

        default:
            ERR_META = opcode;
            throw EXIT_ERR_UNKNOWN_OPCODE;

    }
}
