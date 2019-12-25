//
// Created by Guan Ying Chen on 2019/12/25.
//

#include "OP_Table.h"

OP_Info::OP_Info(int opcode, int formate) : opcode(opcode), formate(formate) {}

OP_Table::OP_Table() {
    _table["ADD"] = new OP_Info(0x18, 3);
    _table["ADDF"] = new OP_Info(0x58, 3);
    _table["ADDR"] = new OP_Info(0x90, 2);
    _table["AND"] = new OP_Info(0x40, 3);
    _table["CLEAR"] = new OP_Info(0xB4, 2);
    _table["COMP"] = new OP_Info(0x28, 3);
    _table["COMPF"] = new OP_Info(0x88, 3);
    _table["COMPR"] = new OP_Info(0xA0, 2);
    _table["DIV"] = new OP_Info(0x24, 3);
    _table["DIVF"] = new OP_Info(0x64, 3);
    _table["DIVR"] = new OP_Info(0x9C, 2);
    _table["FIX"] = new OP_Info(0xC4, 1);
    _table["FLOAT"] = new OP_Info(0xC0, 1);
    _table["HIO"] = new OP_Info(0xF4, 1);
    _table["J"] = new OP_Info(0x3C, 3);
    _table["JEQ"] = new OP_Info(0x30, 3);
    _table["JGT"] = new OP_Info(0x34, 3);
    _table["JLT"] = new OP_Info(0x38, 3);
    _table["JSUB"] = new OP_Info(0x48, 3);
    _table["LDA"] = new OP_Info(0x00, 3);
    _table["LDB"] = new OP_Info(0x68, 3);
    _table["LDCH"] = new OP_Info(0x50, 3);
    _table["LDF"] = new OP_Info(0x70, 3);
    _table["LDL"] = new OP_Info(0x08, 3);
    _table["LDS"] = new OP_Info(0x6C, 3);
    _table["LDT"] = new OP_Info(0x74, 3);
    _table["LDX"] = new OP_Info(0x04, 3);
    _table["LPS"] = new OP_Info(0xD0, 3);
    _table["MUL"] = new OP_Info(0x20, 3);
    _table["MULF"] = new OP_Info(0x60, 3);
    _table["MULR"] = new OP_Info(0x98, 3);
    _table["NORM"] = new OP_Info(0xC8, 2);
    _table["OR"] = new OP_Info(0x44, 1);
    _table["RD"] = new OP_Info(0xD8, 3);
    _table["RMO"] = new OP_Info(0xAC, 2);
    _table["RSUB"] = new OP_Info(0x4C, 3);
    _table["SHIFTL"] = new OP_Info(0xA4, 2);
    _table["SHIFTR"] = new OP_Info(0xA8, 2);
    _table["SIO"] = new OP_Info(0xF0, 1);
    _table["SSK"] = new OP_Info(0xEC, 3);
    _table["STA"] = new OP_Info(0x0C, 3);
    _table["STB"] = new OP_Info(0x78, 3);
    _table["STCH"] = new OP_Info(0x54, 3);
    _table["STF"] = new OP_Info(0x80, 3);
    _table["STI"] = new OP_Info(0xD4, 3);
    _table["STL"] = new OP_Info(0x14, 3);
    _table["STS"] = new OP_Info(0x7C, 3);
    _table["STSW"] = new OP_Info(0xE8, 3);
    _table["STT"] = new OP_Info(0x84, 3);
    _table["STX"] = new OP_Info(0x10, 3);
    _table["SUB"] = new OP_Info(0x1C, 3);
    _table["SUBF"] = new OP_Info(0x5C, 3);
    _table["SUBR"] = new OP_Info(0x94, 2);
    _table["SVC"] = new OP_Info(0xB0, 2);
    _table["TD"] = new OP_Info(0xE0, 3);
    _table["TIO"] = new OP_Info(0xF8, 1);
    _table["TIX"] = new OP_Info(0x2C, 3);
    _table["TIXR"] = new OP_Info(0xB8, 2);
    _table["WD"] = new OP_Info(0xDC, 3);
}

const OP_Info &OP_Table::operator[](const std::string &mnemonic) {
    return *_table[mnemonic];
}
