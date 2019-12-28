//
// Created by Gary on 2019/12/26.
//

#ifndef SOURCE_REGISTER_TABLE_H
#define SOURCE_REGISTER_TABLE_H

#include <map>
#include <string>

class Register_Table {
public:
    static Register_Table& getInstance();
    int get(const std::string& reg);

private:

    std::map<std::string, int> _table;

    Register_Table();
    Register_Table(Register_Table const&);
    void operator=(Register_Table const&);
};


#endif //SOURCE_REGISTER_TABLE_H
