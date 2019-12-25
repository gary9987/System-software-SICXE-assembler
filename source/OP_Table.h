//
// Created by Guan Ying Chen on 2019/12/25.
//

#ifndef SOURCE_OP_TABLE_H
#define SOURCE_OP_TABLE_H


#include <string>
#include <map>

class OP_Info{
    int opcode;
    int formate;

public:
    OP_Info(int opcode, int formate);
};

class OP_Table{
public:
    static OP_Table* getInstance();
    std::map<std::string, OP_Info> table;

private:
    static OP_Table *myself;

};





#endif //SOURCE_OP_TABLE_H
