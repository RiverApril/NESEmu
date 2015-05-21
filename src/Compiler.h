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

using namespace std;

namespace Compiler{

    map<const char*, unsigned char> opcodeNames;
    opcodeNames["ADC"] = codeADC;
    opcodeNames["AND"] = codeAND;
    opcodeNames["ASL"] = codeASL;
    opcodeNames["BPL"] = codeBPL;
    opcodeNames["BMI"] = codeBMI;
    opcodeNames["BVC"] = codeBVC;
    opcodeNames["BVS"] = codeBVS;
    opcodeNames["BCC"] = codeBCC;
    opcodeNames["BCS"] = codeBCS;
    opcodeNames["BNE"] = codeBNE;
    opcodeNames["BEQ"] = codeBEQ;
    opcodeNames["BRK"] = codeBRK;
    opcodeNames["BIT"] = codeBIT;
    opcodeNames["CMP"] = codeCMP;
    opcodeNames["CPX"] = codeCPX;
    opcodeNames["CPY"] = codeCPY;
    opcodeNames["DEC"] = codeDEC;
    opcodeNames["DEX"] = codeDEX;
    opcodeNames["DEY"] = codeDEY;
    opcodeNames["EOR"] = codeEOR;
    opcodeNames["INC"] = codeINC;
    opcodeNames["INX"] = codeINX;
    opcodeNames["INY"] = codeINY;
    opcodeNames["CLC"] = codeCLC;
    opcodeNames["SEC"] = codeSEC;
    opcodeNames["CLI"] = codeCLI;
    opcodeNames["SEI"] = codeSEI;
    opcodeNames["CLV"] = codeCLV;
    opcodeNames["CLD"] = codeCLD;
    opcodeNames["SED"] = codeSED;
    opcodeNames["JMP"] = codeJMP;
    opcodeNames["JSR"] = codeJSR;
    opcodeNames["RTI"] = codeRTI;
    opcodeNames["RTS"] = codeRTS;
    opcodeNames["LDA"] = codeLDA;
    opcodeNames["LDX"] = codeLDX;
    opcodeNames["LDY"] = codeLDY;
    opcodeNames["LSR"] = codeLSR;
    opcodeNames["NOP"] = codeNOP;
    opcodeNames["ORA"] = codeORA;
    opcodeNames["PHA"] = codePHA;
    opcodeNames["PHP"] = codePHP;
    opcodeNames["PLA"] = codePLA;
    opcodeNames["PLP"] = codePLP;
    opcodeNames["ROL"] = codeROL;
    opcodeNames["ROR"] = codeROR;
    opcodeNames["SBC"] = codeSBC;
    opcodeNames["STA"] = codeSTA;
    opcodeNames["STX"] = codeSTX;
    opcodeNames["STY"] = codeSTY;
    opcodeNames["TAX"] = codeTAX;
    opcodeNames["TAY"] = codeTAY;
    opcodeNames["TSX"] = codeTSX;
    opcodeNames["TXA"] = codeTXA;
    opcodeNames["TXS"] = codeTXS;
    opcodeNames["TYA"] = codeTYA;


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
                if(line[4] == '\n'){
                    continue;
                } else if(line[4] == ' '){
                    if(line[5] == '#'){
                        if(line[6] == '$'){
                            parseHex(line.substr(7, 2));
                        }
                    }
                }
            }else{
                printf("Unknown Opcode Name: %s", opcodeName);
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