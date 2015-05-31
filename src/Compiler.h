#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Chip.h"

/* Syntax

Adress Modes:
Immediate       : #$44
Zero Page       : $44
Zero Page X     : $44,X
Absolute        : $4400
Absolute X      : $4400,X
Absolute Y      : $4400,Y
Indirect X      : ($44,X);
Indirect Y      : ($44),Y


*/

#define mode_I_ZP_ZPX_AB_ABX_ABY_IX_IY = 0;
#define mode_A_ZP_ZPX_AB_ABX = 1;
#define mode_R = 2;
#define mode_ZP_A = 3;
#define mode_I = 4;
#define mode_I_ZP_A = 5;
#define mode_ZP_ZPX_AB_ABX = 6;
#define mode_ZP_ZPX_ZB_ZBX = 7;
#define mode_AB_I = 8;
#define mode_A = 9;
#define mode_I_ZP_ZPY_AB_ABY = 10;
#define mode_I_ZP_ZPX_AB_ABX = 11;
#define mode_ZP_ZPX_AB_ABX_ABY_IX_IY = 12;
#define mode_ZP_ZPY_AB = 13;
#define mode_ZP_ZPX_AB = 14;

using namespace std;

namespace Compiler{

    map<unsigned char, unsigned char> offsetOctothorpDollar;
    offsetOctothorpDollar[mode_I_ZP_ZPX_AB_ABX_ABY_IX_IY] = 0x08;
    offsetOctothorpDollar[mode_I_ZP_A] = 0x00;
    offsetOctothorpDollar[mode_I_ZP_ZPY_AB_ABY] = 0x00;
    offsetOctothorpDollar[mode_I_ZP_ZPX_AB_ABX] = 0x00;

    map<const char*, unsigned char> opcodeNames;
    map<const char*, unsigned char> opcodeMode;

    opcodeNames["ADC"] = codeADC; opcodeMode["ADC"] = mode_I_ZP_ZPX_AB_ABX_ABY_IX_IY;
    opcodeNames["AND"] = codeAND; opcodeMode["AND"] = mode_I_ZP_ZPX_AB_ABX_ABY_IX_IY;
    opcodeNames["ASL"] = codeASL; opcodeMode["ASL"] = mode_A_ZP_ZPX_AB_ABX;
    opcodeNames["BPL"] = codeBPL; opcodeMode["BPL"] = mode_R;
    opcodeNames["BMI"] = codeBMI; opcodeMode["BMI"] = mode_R;
    opcodeNames["BVC"] = codeBVC; opcodeMode["BVC"] = mode_R;
    opcodeNames["BVS"] = codeBVS; opcodeMode["BVS"] = mode_R;
    opcodeNames["BCC"] = codeBCC; opcodeMode["BCC"] = mode_R;
    opcodeNames["BCS"] = codeBCS; opcodeMode["BCS"] = mode_R;
    opcodeNames["BNE"] = codeBNE; opcodeMode["BNE"] = mode_R;
    opcodeNames["BEQ"] = codeBEQ; opcodeMode["BEQ"] = mode_R;
    opcodeNames["BRK"] = codeBRK; opcodeMode["BRK"] = mode_I;
    opcodeNames["BIT"] = codeBIT; opcodeMode["BIT"] = mode_ZP_A;
    opcodeNames["CMP"] = codeCMP; opcodeMode["CMP"] = mode_I_ZP_ZPX_AB_ABX_ABY_IX_IY;
    opcodeNames["CPX"] = codeCPX; opcodeMode["CPX"] = mode_I_ZP_A;
    opcodeNames["CPY"] = codeCPY; opcodeMode["CPY"] = mode_I_ZP_A;
    opcodeNames["DEC"] = codeDEC; opcodeMode["DEC"] = mode_ZP_ZPX_AB_ABX;
    opcodeNames["DEX"] = codeDEX; opcodeMode["DEX"] = mode_I;
    opcodeNames["DEY"] = codeDEY; opcodeMode["DEY"] = mode_I;
    opcodeNames["EOR"] = codeEOR; opcodeMode["EOR"] = mode_I_ZP_ZPX_AB_ABX_ABY_IX_IY;
    opcodeNames["INC"] = codeINC; opcodeMode["INC"] = mode_ZP_ZPX_ZB_ZBX;
    opcodeNames["INX"] = codeINX; opcodeMode["INX"] = mode_I;
    opcodeNames["INY"] = codeINY; opcodeMode["INY"] = mode_I;
    opcodeNames["CLC"] = codeCLC; opcodeMode["CLC"] = mode_I;
    opcodeNames["SEC"] = codeSEC; opcodeMode["SEC"] = mode_I;
    opcodeNames["CLI"] = codeCLI; opcodeMode["CLI"] = mode_I;
    opcodeNames["SEI"] = codeSEI; opcodeMode["SEI"] = mode_I;
    opcodeNames["CLV"] = codeCLV; opcodeMode["CLV"] = mode_I;
    opcodeNames["CLD"] = codeCLD; opcodeMode["CLD"] = mode_I;
    opcodeNames["SED"] = codeSED; opcodeMode["SED"] = mode_I;
    opcodeNames["JMP"] = codeJMP; opcodeMode["JMP"] = mode_AB_I;
    opcodeNames["JSR"] = codeJSR; opcodeMode["JSR"] = mode_A;
    opcodeNames["RTI"] = codeRTI; opcodeMode["RTI"] = mode_I;
    opcodeNames["RTS"] = codeRTS; opcodeMode["RTS"] = mode_I;
    opcodeNames["LDA"] = codeLDA; opcodeMode["LDA"] = mode_I_ZP_ZPX_AB_ABX_ABY_IX_IY;
    opcodeNames["LDX"] = codeLDX; opcodeMode["LDX"] = mode_I_ZP_ZPY_AB_ABY;
    opcodeNames["LDY"] = codeLDY; opcodeMode["LDY"] = mode_I_ZP_ZPX_AB_ABX;
    opcodeNames["LSR"] = codeLSR; opcodeMode["LSR"] = mode_A_ZP_ZPX_AB_ABX;
    opcodeNames["NOP"] = codeNOP; opcodeMode["NOP"] = mode_I;
    opcodeNames["ORA"] = codeORA; opcodeMode["ORA"] = mode_I_ZP_ZPX_AB_ABX_ABY_IX_IY;
    opcodeNames["PHA"] = codePHA; opcodeMode["PHA"] = mode_I;
    opcodeNames["PHP"] = codePHP; opcodeMode["PHP"] = mode_I;
    opcodeNames["PLA"] = codePLA; opcodeMode["PLA"] = mode_I;
    opcodeNames["PLP"] = codePLP; opcodeMode["PLP"] = mode_I;
    opcodeNames["ROL"] = codeROL; opcodeMode["ROL"] = mode_A_ZP_ZPX_AB_ABX;
    opcodeNames["ROR"] = codeROR; opcodeMode["ROR"] = mode_A_ZP_ZPX_AB_ABX;
    opcodeNames["SBC"] = codeSBC; opcodeMode["SBC"] = mode_I_ZP_ZPX_AB_ABX_ABY_IX_IY;
    opcodeNames["STA"] = codeSTA; opcodeMode["STA"] = mode_ZP_ZPX_AB_ABX_ABY_IX_IY;
    opcodeNames["STX"] = codeSTX; opcodeMode["STX"] = mode_ZP_ZPY_AB;
    opcodeNames["STY"] = codeSTY; opcodeMode["STY"] = mode_ZP_ZPX_AB;
    opcodeNames["TAX"] = codeTAX; opcodeMode["TAX"] = mode_I;
    opcodeNames["TAY"] = codeTAY; opcodeMode["TAY"] = mode_I;
    opcodeNames["TSX"] = codeTSX; opcodeMode["TSX"] = mode_I;
    opcodeNames["TXA"] = codeTXA; opcodeMode["TXA"] = mode_I;
    opcodeNames["TXS"] = codeTXS; opcodeMode["TXS"] = mode_I;
    opcodeNames["TYA"] = codeTYA; opcodeMode["TYA"] = mode_I;


    bool compileFile(const char* sourcePath, const char* binPath){

        vector<unsigned char> binary;

        ifstream sourceFile(sourcePath);

        bool error = false;

        string line;
        while(getLine(sourceFile, line)){
            if(line.empty()){
                continue;
            }

            const char* opcodeName = line.substr(0, 3).c_str();
            if(opcodeNames.count(opcodeName)){
                unsigned char baseOpcode = opcodeNames[opcodeName];
                if(line.length() > 3){

                    if(line[3] == '\n'){
                        binary.push(baseOpcode);
                        continue;
                    } else if(line[3] == ' '){
                        if(line[4] == '#'){
                            if(line[5] == '$'){
                                unsigned char address = parseHex(line.substr(6, 2));
                                if(opcodeMode.count(opcodeName)){
                                    const unsigned char mode = opcodeMode[opcodeName];
                                    if(offsetOctothorpDollar.count(mode)){
                                        address += offsetOctothorpDollar[mode];
                                        binary.push(address);
                                    }else{
                                        printf("Unknown use of #$: %s", opcodeName);
                                        error = true;
                                        break;
                                    }
                                }else{
                                    printf("Unknown opcode name #$: %s", opcodeName);
                                    error = true;
                                    break;
                                }
                                continue;
                            }
                        } else if(line[4] == '$'){
                            unsigned char address = parseHex(line.substr(5, 2));
                            if(){

                            }
                        }
                    }
                }
            }else{
                printf("Unknown opcode name: %s", opcodeName);
                error = true;
                break;
            }

        }

        sourceFile.close();

        if(!error){
            ofstream binFile(binPath);

            unsigned char* buffer = &binary[0];

            binFile.write(buffer, binary.size());

            binFile.close();
        }

        return true;
    }

}
