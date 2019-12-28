//
// Created by Guan Ying Chen on 2019/12/25.
//

#ifndef SOURCE_OP_TABLE_H
#define SOURCE_OP_TABLE_H


#include <string>
#include <map>

class OP_Info{
public:
    OP_Info(int opcode, int formate);
    int opcode;
    int formate;
};

class OP_Table{
public:

    static OP_Table& getInstance();

    bool find(const std::string& target) const;
    const OP_Info& get(const std::string& mnemonic);
    ~OP_Table();
private:
    OP_Table();

    std::map<std::string, OP_Info*> _table;
};





#endif //SOURCE_OP_TABLE_H
