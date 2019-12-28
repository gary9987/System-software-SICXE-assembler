//
// Created by Gary on 2019/12/25.
//

#ifndef SOURCE_PASSONE_H
#define SOURCE_PASSONE_H

#include <iostream>
#include <fstream>
#include <map>
#include "OP_Table.h"

class PassOne {
public:
    PassOne(const std::string& in_url, const std::string& out_url);
    virtual void perform();

    virtual void clear();

protected:
    std::ifstream _infile;
    std::ofstream _outfile;

    std::string _symbol;
    std::string _origin_opcode;
    std::string _opcode;
    std::string _origin_operand;
    std::string _operand1;
    std::string _operand2;
    static std::map<std::string, int> _shared_symbolTable;

    static int shared_program_length;

    bool _flag_extended = false;
    int _op_length = 0;
    int _LOCCTR = 0;

    int _getFormat(const std::string &code);

    virtual void _parseLine(const std::string &line);
    int _byte_length(const std::string &operand) const;
    void _append_to_outfile(int loc, const std::string &in);
};


#endif //SOURCE_PASSONE_H
