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

// Init static var
int PassOne::shared_program_length = 0;
std::map<std::string, int> PassOne::_shared_symbolTable;

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
        if(!_symbol.empty()){
            if(_shared_symbolTable.count(_symbol) == 0){
                _shared_symbolTable[_symbol] = _LOCCTR;
                cout<<"Add symbol "<<_symbol<<endl;
            }
            else{
                cerr << "ERROR! " <<_symbol<< " has already declared.\n";
            }
        }

        // Find OP code
        if(OP_Table::getInstance().find(_opcode)){
            _LOCCTR += _op_length;
        }
        else if(_opcode == "WORD"){
            _LOCCTR += 3; // 1 WORD 3 bytes
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

    shared_program_length = _LOCCTR;

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

    // Remove prfix '+'
    if(_origin_opcode[0] == '+'){
        _opcode.assign(_origin_opcode, 1, _origin_opcode.length()-1);
    }
    else{
        _opcode = _origin_opcode;
    }

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
            _operand1.assign(_origin_operand, 0, idx);
            _operand2.assign(_origin_operand, idx+1 , _origin_operand.length()-idx-1);
        }
        else{
            // Have one operand
            _operand1 = _origin_operand;
        }
    }

}

int PassOne::_getFormat(const std::string &code){

    int re = 0;
    string cp_code(code);
    // If OPCODE prefix is '+', then it is extended format
    if(cp_code[0] == '+'){
        _flag_extended = true;
        cp_code.assign(cp_code, 1, cp_code.length()-1);
    }

    if(OP_Table::getInstance().find(cp_code)){
        if(OP_Table::getInstance().get(cp_code).formate == 3){
            re = _flag_extended ? 4 : 3;
        }
        else{
            re = OP_Table::getInstance().get(cp_code).formate;
        }
    }
    else if(cp_code == "BASE"){
        re = -1; // BASE
    }
    else if(cp_code == "BYTE" || cp_code == "WORD"){
        re = -2;
    }
    // START RESW RESB END return 0

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
    _op_length = 0;
}

int PassOne::_byte_length(const std::string &operand) const {
    string cp_operand(operand);
    for(auto &i:cp_operand){
        i = toupper(i);
    }
    if(cp_operand[0] == 'X'){
        return 1;
    }
    else{
        int idx = 2, len = 1;
        for(; idx < cp_operand.length(); idx++, len++) {
            if (cp_operand[idx] == '\'') {
                return len - 1;
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
