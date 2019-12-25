//
// Created by Gary on 2019/12/25.
//

#ifndef SOURCE_PASSONE_H
#define SOURCE_PASSONE_H

#include <iostream>
#include <fstream>
#include <map>

class PassOne {
public:
    PassOne(const std::string& in_url, const std::string& out_url);
    void perform();
    void clear();

private:
    std::ifstream _infile;
    std::ofstream _outfile;

    std::string _symbol;
    std::string _origin_opcode;
    std::string _opcode;
    std::string _origin_operand;
    std::string _operand1;
    std::string _operand2;
    std::map<std::string, int> _symbolTable;



    bool _flag_extended = false;
    int _op_length = 0, _operand_count = 0;
    int _LOCCTR = 0;

    int _getFormat(const std::string &code);
    void _parseLine(const std::string &line);
    int _byte_length(const std::string &operand) const;
    void _append_to_outfile(int loc, const std::string &in);
};


#endif //SOURCE_PASSONE_H
