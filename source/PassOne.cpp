//
// Created by Gary on 2019/12/25.
//

#include "PassOne.h"
#include "OP_Table.h"
#include <string>
#include <sstream>

using namespace std;

PassOne::PassOne(const std::string& url){
    _infile.open(url);
    if(!_infile){
        cerr<<"Open file failure\n";
    }
    else{
        cout<<"Open file Success\n";
    }
}

void PassOne::perform() {
    string read;
    while(getline(_infile, read)){
        cout<<"now read line : "<<read<<endl;
        _parseLine(read);
    }


}

void PassOne::_parseLine(const std::string &line) {
    string cp_line(line);
    stringstream streamline(cp_line);

    // Check is Symbol
    if(cp_line[0] != '\t'){
        streamline >> _symbol;
    }

    // Get OPCODE
    streamline >> _origin_opcode;
    _op_length = _getFormat(_origin_opcode);

    // Get OPERAND
    streamline >> _origin_operand;
    if(_origin_operand != ""){

        int idx = 0;
        while(_origin_operand[idx] != ',' && idx < _origin_operand.length()){
            idx ++ ;
        }

        if( idx < _origin_operand.length()){
            // Have two operands
            _operand_count = 2;
            _operand1.assign(_origin_operand, 0, idx);
            _operand2.assign(_origin_operand, idx+1 , _origin_operand.length()-idx-1);
        }
        else{
            // Have one operand
            _operand_count = 1;
            _operand1 = _origin_operand;
        }
    }

}

int PassOne::_getFormat(const std::string &code) {

    int re = 0;

    // If OPCODE prefix is '+', then it is extended format
    if(code[0] == '+'){
        _flag_extended = true;
        _opcode.assign(_origin_opcode, 1, _origin_opcode.length()-1);
    }
    else{
        _opcode = _origin_opcode;
    }

    OP_Table table;
    if(table.find(_opcode)){
        if(table[_opcode].formate == 3){
            re = _flag_extended ? 4 : 3;
        }
        else{
            re = table[_opcode].formate;
        }
    }
    else if(_opcode == "BYTE"){
        re = 5;
    }
    else if(_opcode == "WORD"){
        re = 6;
    }
    else if(_opcode == "RESB"){
        re = 0;
    }
    else if(_opcode == "RESW"){
        re = 0;
    }
    else if(_opcode == "BASE"){
        re = 7;
    }
    else if(_opcode == "START"){
        re = 0;
    }
    else if(_opcode == "END"){
        re = -0;
    }
    return re;
}
