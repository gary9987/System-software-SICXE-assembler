//
// Created by Gary on 2019/12/25.
//

#ifndef SOURCE_PASSONE_H
#define SOURCE_PASSONE_H

#include <iostream>
#include <fstream>

class PassOne {
public:
    PassOne(const std::string& url);
    void perform();


private:
    std::ifstream _infile;
    std::string _symbol;
    std::string _origin_opcode;
    std::string _opcode;
    std::string _origin_operand;
    std::string _operand1;
    std::string _operand2;



    bool _flag_extended = false;
    int _op_length = 0, _operand_count = 0;

    int _LOCCTR = 0, _PREV_LOCCTR = 0, _line_Counter = 1;
    int _print_type = 1;

    int _getFormat(const std::string &code);
    void _parseLine(const std::string &line);
};


#endif //SOURCE_PASSONE_H
