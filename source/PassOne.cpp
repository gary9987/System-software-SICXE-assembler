//
// Created by Gary on 2019/12/25.
//

#include "PassOne.h"
#include "OP_Table.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <vector>

using namespace std;

PassOne::PassOne(const std::string& in_url, const std::string& out_url){
    _infile.open(in_url);
    _outfile.open(out_url);

    if(!_infile){
        cerr<<"Open file failure\n";
    }
    else{
        cout<<"Open file Success\n";
    }
}

void PassOne::perform() {
    string read;

    // Read first line
    getline(_infile, read);
    cout<<"now read line : "<<read<<endl;
    _parseLine(read);

    if(_opcode == "START"){
        // Save starting address
        stringstream tmp(_operand1);
        tmp>>_LOCCTR;

        _outfile<<setw(4)<<setfill('0')<<_LOCCTR<<"    "<<read<<endl;
    }

    while(getline(_infile, read)){

        int CP_LOCCTR = _LOCCTR;
        int output_type = 0;

        clear();

        cout<<"now read line : "<<read<<endl;
        _parseLine(read);

        if(_opcode == "END"){
            _outfile<<"           "<<read<<endl;
            break;
        }

        // Append symbol to symbol table
        if(_symbol != ""){
            if(_symbolTable.count(_symbol) == 0){
                _symbolTable[_symbol] = _LOCCTR;
            }
            else{
                cerr << "ERROR! " <<_symbol<< " has already declared.\n";
            }
        }

        // Find OP code
        OP_Table opTable;
        if(opTable.find(_opcode)){
            _LOCCTR += _op_length;
        }
        else if(_opcode == "WORD"){
            _LOCCTR += 3;
        }
        else if(_opcode == "RESW"){
            _LOCCTR += 3 * strtol(_operand1.c_str(), NULL, 10);
        }
        else if(_opcode == "RESB"){
            _LOCCTR += strtol(_operand1.c_str(), NULL, 10);;
        }
        else if(_opcode == "BYTE"){
            _LOCCTR += _byte_length(_operand1);
        }
        else if(_opcode == "BASE"){
            _LOCCTR += 3;
            output_type = 1;
        }
        else{
            cerr<<"Undefined op code\n";
            return;
        }

        // Output type 1
        if(output_type){
            _outfile<<"        "<<read<<endl;
        }
        // Output type 0
        else{
            _append_to_outfile(CP_LOCCTR, read);
        }
    }


    // End Pass One
    _outfile.close();
    _infile.close();
}

void PassOne::_parseLine(const std::string &line) {
    string cp_line(line);
    string temp;
    stringstream streamline(cp_line);

    vector<string> sub_line;
    // CLOOP +JSUB RDREC
    while (streamline >> temp){
        sub_line.push_back(temp);
    }

    int offset_idx = 0;

    // Get Symbol
    if(sub_line.size() == 3){
        _symbol = sub_line[0];
        offset_idx = 1;
    }

    // Get OPCODE
    _origin_opcode = sub_line[ 0 + offset_idx];
    _op_length = _getFormat(_origin_opcode);

    // Get OPERAND
    if(sub_line.size() >= 2){
        _origin_operand = sub_line[1 + offset_idx];
    }


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
        re = 0;
    }
    return re;
}

void PassOne::clear() {
    _symbol = "";
    _origin_opcode = "";
    _opcode = "";
    _origin_operand = "";
    _operand1 = "";
    _operand2 = "";

    _flag_extended = false;
    _op_length = 0, _operand_count = 0;
}

int PassOne::_byte_length(const std::string &operand) const {
    if(operand[0] == 'X'){
        return 1;
    }
    else{
        int idx = 1, len = 0;
        for(; idx < operand.length(); idx++, len++) {
            if (operand[idx] == '\'') {
                return len;
            }
        }
    }

    // Parse operand error
    cerr<<"Error, perform _byte_length, input is "<<operand<<endl;
    return 0;
}

void PassOne::_append_to_outfile(int loc, const std::string &in) {
    _outfile<<setw(4)<<setfill('0')<<hex<<loc<<"    "<<in<<endl;
}
