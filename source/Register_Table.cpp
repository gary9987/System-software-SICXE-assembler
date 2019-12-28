//
// Created by Gary on 2019/12/26.
//

#include "Register_Table.h"


Register_Table::Register_Table() {
    _table["A"] = 0;
    _table["X"] = 1;
    _table["L"] = 2;
    _table["B"] = 3;
    _table["S"] = 4;
    _table["T"] = 5;
    _table["F"] = 6;
    _table["PC"] = 8;
    _table["SW"] = 9;
}

int Register_Table::get(const std::string &reg){
    return _table[reg];
}

Register_Table &Register_Table::getInstance() {
    static Register_Table instance;
    return instance;
}
