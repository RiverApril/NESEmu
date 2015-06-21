
#ifndef __NESEmu__Chip__
#define __NESEmu__Chip__

#include <stdio.h>
#include <stack>
#include <stdlib.h>


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



#define memAccumulator A

#define memImmediate byteAfterOpcode

#define addressZeroPage byteAfterOpcode
#define getMemZeroPage getMemory(addressZeroPage)
#define setMemZeroPage(a) setMemory(addressZeroPage, a)

#define addressZeroPageX ((byteAfterOpcode+X) & 0xFF)
#define getMemZeroPageX getMemory(addressZeroPageX)
#define setMemZeroPageX(a) setMemory(addressZeroPageX, a)

#define addressZeroPageY ((byteAfterOpcode+Y) & 0xFF)
#define getMemZeroPageY getMemory(addressZeroPageY)
#define setMemZeroPageY(a) setMemory(addressZeroPageY, a)

#define addressAbsolute (byteAfterOpcode | ((unsigned short)byte2AfterOpcode << 8))
#define getMemAbsolute getMemory(addressAbsolute)
#define setMemAbsolute(a) setMemory(addressAbsolute, a)

#define addressAbsoluteX (((byteAfterOpcode | ((unsigned short)byte2AfterOpcode << 8))+X) & 0xFFFF)
#define getMemAbsoluteX getMemory(addressAbsoluteX)
#define setMemAbsoluteX(a) setMemory(addressAbsoluteX, a)

#define addressAbsoluteY getMemory(((byteAfterOpcode | ((unsigned short)byte2AfterOpcode << 4))+Y) & 0xFFFF)
#define getMemAbsoluteY getMemory(addressAbsoluteY)
#define setMemAbsoluteY(a) setMemory(addressAbsoluteY, a)

#define memIndirectXHelp getMemory(((byteAfterOpcode+X) & 0xFF))
#define addressIndirectX (getMemory(memIndirectXHelp) | ((unsigned short)getMemory(memIndirectXHelp+1) << 8))
#define getMemIndirectX getMemory(addressIndirectX)
#define setMemIndirectX(a) setMemory(addressIndirectX, a)

#define addressIndirectY ((((getMemory(byteAfterOpcode)) | ((unsigned short)getMemory(byte2AfterOpcode) << 8)) + Y) & 0xFFFF)
#define getMemIndirectY getMemory(addressIndirectY)
#define setMemIndirectY(a) setMemory(addressIndirectY, a)

#define addressIndirect (((getMemory(byteAfterOpcode)) | ((unsigned short)getMemory(byte2AfterOpcode) << 8)) & 0xFFFF)
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
#define EXIT_ERR_MEMORY_ADDRESS_OUT_OF_RANGE 5


struct BitField {
    bool bit0 : 1;
    bool bit1 : 1;
    bool bit2 : 1;
    bool bit3 : 1;
    bool bit4 : 1;
    bool bit5 : 1;
    bool bit6 : 1;
    bool bit7 : 1;
};

union BitByteUnion {
    unsigned char byte;
    struct BitField bits;
};


class Chip{

public:
    
    bool controllerP1Buffer[8];
    bool controllerP2Buffer[8];
    
    unsigned char controllerP1Index = 0;
    unsigned char controllerP2Index = 0;

    int ERR_META = 0;

    //Program counter
    unsigned short pc = 0x0000;

    //Accumulator
    unsigned char A = 0x00;

    //X and Y Registers
    unsigned char X = 0x00;
    unsigned char Y = 0x00;
    
    
    BitByteUnion CPU_Flags;
    
#define CPU_C CPU_Flags.bits.bit0
#define CPU_Z CPU_Flags.bits.bit1
#define CPU_I CPU_Flags.bits.bit2
#define CPU_D CPU_Flags.bits.bit3
#define CPU_V CPU_Flags.bits.bit6
#define CPU_N CPU_Flags.bits.bit7

#define CPU_S CPU_Flags.byte


    BitByteUnion PPU_CTRL_Flags;
    
#define PPU_CTRL_N0 PPU_CTRL_Flags.bits.bit0
#define PPU_CTRL_N1 PPU_CTRL_Flags.bits.bit1
#define PPU_CTRL_I PPU_CTRL_Flags.bits.bit2
#define PPU_CTRL_S PPU_CTRL_Flags.bits.bit3
#define PPU_CTRL_B PPU_CTRL_Flags.bits.bit4
#define PPU_CTRL_H PPU_CTRL_Flags.bits.bit5
#define PPU_CTRL_P PPU_CTRL_Flags.bits.bit6
#define PPU_CTRL_V PPU_CTRL_Flags.bits.bit7

#define PPU_CTRL PPU_CTRL_Flags.byte


    BitByteUnion PPU_MASK_Flags;
    
#define PPU_MASK_G PPU_MASK_Flags.bits.bit0
#define PPU_MASK_m PPU_MASK_Flags.bits.bit1
#define PPU_MASK_M PPU_MASK_Flags.bits.bit2
#define PPU_MASK_b PPU_MASK_Flags.bits.bit3
#define PPU_MASK_s PPU_MASK_Flags.bits.bit4
#define PPU_MASK_R PPU_MASK_Flags.bits.bit5
#define PPU_MASK_G PPU_MASK_Flags.bits.bit6
#define PPU_MASK_B PPU_MASK_Flags.bits.bit7

#define PPU_MASK PPU_MASK_Flags.byte


    BitByteUnion PPU_STATUS_Flags;
    
#define PPU_STATUS_O PPU_STATUS_Flags.bits.bit5
#define PPU_STATUS_S PPU_STATUS_Flags.bits.bit6
#define PPU_STATUS_V PPU_STATUS_Flags.bits.bit7

#define PPU_STATUS PPU_STATUS_Flags.byte

    unsigned char OAM_ADDR;
    unsigned char OAM_DATA;
    
    unsigned char PPU_SCROLL_INDEX;
    unsigned char PPU_SCROLL_X;
    unsigned char PPU_SCROLL_Y;
    
    unsigned char PPU_ADDR_INDEX;
    unsigned char PPU_ADDR_MSB;
    unsigned char PPU_ADDR_LSB;
    
    unsigned char PPU_DATA;
    unsigned char OAM_DMA;


    unsigned char memoryRAM[0x800] = {0};
    unsigned char memoryPPURegisters[0x8] = {0};
    unsigned char memoryAPUandIORegisters[0x20] = {0};
    unsigned char memoryProgramMemory[0xBFE0] = {0};
    
    //Note: all numbers that follow in these comments are hexidecimal.
    
    ////// Memory Mapping details:
    //// Work RAM
    // Address         :    0 - 1FFF
    // Translation     : address % 800
    // Emulator Memory : memoryRAM[800]
    
    //// PPU Registers
    // Address         : 2000 - 3FFF
    // Translation     : address % 8
    // Emulator Memory : memoryPPURegisters[8]
    
    //// APU and IO Registers
    // Address         : 4000 - 401F
    // Translation     : address - 4000
    // Emulator Memory : memoryAPUandIORegisters[20]
    
    //// Program ROM, SRAM, RAM
    // Address         : 4020 - FFFF
    // Translation     : address - 4020
    // Emulator Memory : memoryProgramMemory[BFE0]
    
    
    //// Real Addresses:
    //    0 -  7FF : Work RAM
    //  800 -  FFF : Mirror of Work RAM
    // 1000 - 17FF : Mirror of Work RAM
    // 1800 - 1FFF : Mirror of Work RAM
    
    // 2000 - 2007 : PPU Registers
    // 2008 - 3FFF : Every 8 are mirrors of PPU Registers
    
    // 4000 - 401F : APU and IO Registers 
    
    // 4020 - 5FFF : Program ROM
    // 6000 - 7FFF : SRAM
    // 8000 - BFFF : Program RAM
    // C000 - FFFF : Program RAM

public:
    unsigned char stackPointer = 0xFD;
    unsigned char stack[0x100] = {0};

    unsigned char opcode;
    unsigned char byteAfterOpcode;
    unsigned char byte2AfterOpcode;
    
    void reset(bool resetRAM, bool resetPPUandAPUandIO, bool resetPRGRAM);

    const char* opcodeName(unsigned char code);
    
    unsigned char opcodeLength(unsigned char code);

    const unsigned short address_AB_I(unsigned char mode);

    const unsigned char mem_I_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode);

    const unsigned char mem_I_ZP_AB(unsigned char mode);

    const unsigned char mem_ZP_ZPX_AB(unsigned char mode);

    const unsigned char mem_ZP_ZPY_AB(unsigned char mode);

    const unsigned char mem_A_ZP_ZPX_AB_ABX(unsigned char mode);

    const unsigned char mem_I_ZP_ZPX_AB_ABX(unsigned char mode);

    const unsigned char mem_A_ZP_ZPY_AB_ABY(unsigned char mode);

    const unsigned char mem_I_ZP_ZPY_AB_ABY(unsigned char mode);

    void setmem_A_ZP_ZPX_AB_ABX(unsigned char mode, unsigned char value);

    void setmem_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode, unsigned char value);

    void setmem_ZP_ZPX_AB(unsigned char mode, unsigned char value);

    void setmem_ZP_ZPY_AB(unsigned char mode, unsigned char value);

    unsigned char opcodeLength_I_ZP_ZPX_AB_ABX_ABY_IX_IY(unsigned char mode);

    unsigned char opcodeLength_A_ZP_ZPX_AB_ABX(unsigned char mode);

    unsigned char opcodeLength_I_ZP_ZPX_AB_ABX(unsigned char mode);

    unsigned char opcodeLength_A_ZP_ZPY_AB_ABY(unsigned char mode);

    unsigned char opcodeLength_I_ZP_ZPY_AB_ABY(unsigned char mode);

    unsigned char opcodeLength_ZP_ZPX_AB(unsigned char mode);

    unsigned char opcodeLength_ZP_ZPY_AB(unsigned char mode);

    unsigned char getMemory(unsigned short address, bool passive = false);

    void setMemory(unsigned short address, unsigned char value);
    
    void setCharROM(unsigned address, unsigned char value);

    void pushToStack(unsigned char value);

    unsigned char popFromStack();
    
    void prepareOpcode();
    void executeOpcode();
    
};



#endif /* defined(__NESEmu__Chip__) */
