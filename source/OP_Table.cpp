//
// Created by Guan Ying Chen on 2019/12/25.
//

#include "OP_Table.h"

OP_Table* OP_Table::myself = nullptr;

OP_Info::OP_Info(int opcode, int formate) : opcode(opcode), formate(formate) {}

OP_Table* OP_Table::getInstance() {
    if(myself == nullptr){
        myself = new OP_Table();
    }
    return myself;
}
