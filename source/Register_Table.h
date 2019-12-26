//
// Created by Gary on 2019/12/26.
//

#ifndef SOURCE_REGISTER_TABLE_H
#define SOURCE_REGISTER_TABLE_H

#include <map>
#include <string>

class Register_Table {
public:
    Register_Table();
    static int get(const std::string& reg);

private:
    static std::map<std::string, int> _table;
};


#endif //SOURCE_REGISTER_TABLE_H
