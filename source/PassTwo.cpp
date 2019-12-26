//
// Created by Guan Ying Chen on 2019/12/26.
//

#include "PassTwo.h"
#include "OP_Table.h"
#include "Register_Table.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <iomanip>

using namespace std;

void PassTwo::perform() {
    string read;

    // Read first line
    getline(_infile, read);
    cout<<"now read line : "<<read<<endl;
    _parseLine(read);

    if(_opcode != "START"){
        cerr<<"Error, START not found on first line.\n";
        return;
    }

    // Generate text record first line
    _out_txrecord << "H" << setw(6) << left << _symbol << right << setw(6) << setfill('0') << hex << _LOCCTR << setw(6) << shared_program_length << endl;

    while (getline(_infile, read)){
        cout<<"now read line : "<<read<<endl;
        _parseLine(read);

        if(_opcode != "END"){

        }
        else{
            _write_to_txrecord();
            break;
        }

    }

}

void PassTwo::_parseLine(const std::string &line) {
    PassOne::_parseLine(line);
    string cp_line(line);
    string temp;
    stringstream streamline(cp_line);

    vector<string> sub_line;
    // 0009 CLOOP +JSUB RDREC
    while (streamline >> temp){
        sub_line.push_back(temp);
    }

    int offset_idx = 0;

    // Get LOC
    if(sub_line.size() > 2){
        stringstream ss;
        ss << std::hex << sub_line[0];
        ss >> _LOCCTR;
        offset_idx ++;
    }

    // Get Symbol
    if(sub_line.size() == 4){
        _symbol = sub_line[offset_idx];
        offset_idx++;
    }

    // Get OPCODE
    _origin_opcode = sub_line[offset_idx];
    _op_length = _getFormat(_origin_opcode);
    offset_idx++;

    // If have OPERAND, get OPERAND
    if(offset_idx < sub_line.size()){
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

PassTwo::PassTwo(const std::string &inUrl, const std::string &outUrl, const std::string &out_resultUrl): PassOne(inUrl, outUrl), _out_txrecord(out_resultUrl) {}

void PassTwo::_write_to_txrecord() {

    // Is line full
    if(69 - _line_buffer.length() < 2 * _op_length){

        stringstream buf;
        buf<<"T000000"<<hex<<(_line_buffer.length())/2;

        // Write to output txrecord file
        _out_txrecord << buf.rdbuf() <<_line_buffer<<endl;

        // Clear buf
        buf.str("");
        buf << "T" << setw(6)<<setfill('0')<<hex<<_LOCCTR<<_obj_code;

        _line_buffer += buf.str();

    }

    else if(_opcode == "END"){
        stringstream buf;
        buf<<"T000000"<<hex<<(_line_buffer.length())/2;

        // Write to output txrecord file
        _out_txrecord << buf.rdbuf() <<_line_buffer<<endl;
    }
    else{
        _line_buffer += _obj_code;

    }

}

void PassTwo::clear() {
    PassOne::clear();
    _obj_code = "";
}


void PassTwo::_generate_object_code() {
    int n, i, x, b, p, e;
    int displcement = 0, BASE = 0;

    int format = _getFormat(_opcode);
    int opcode_value = shared_table[_opcode].opcode;
    if(format == 1){
        _obj_code = to_object_str(opcode_value, format);
    }
    else if(format == 2){
        // Shift left 8 bits
        opcode_value <<= 8;

        if(!_operand2.empty()){
            // Two OPERAND
            opcode_value += (Register_Table::get(_operand1) << 4) | (Register_Table::get(_operand2));
        }
        else{
            // One OPERAND
            opcode_value += (Register_Table::get(_operand1) << 4);
        }

        _obj_code = to_object_str(opcode_value, format);
    }
    else if(format == 3){
        // Immediate addressing
        if(_operand1[0] == '#'){
            n = 0;
            i = 1;
        }
        //Indirect addressing
        else if(_operand1[0] == '@'){
            n = 1;
            i = 0;
        }
        else{
            i = 1;
            n = 1;
        }

        // Calculate displacement
        if(_operand1[0] == '#' && isdigit(_operand1[1])){
            displcement = strtol(_operand1.c_str() + 1, NULL, 10);
            b = 0;
            p = 0;
        }
        else if(_is_relative_mod(displcement, BASE)){
            // PC relative
            b = 0;
            p = 1;
        }
        else{
            // BASE relative
            b = 1;
            p = 0;
        }

        // Is index address
        if( !_operand2.empty() && _operand2[0] == 'X'){
            x = 1;
        }
        else{
            x = 0;
        }
        e = 0;

        if(_opcode == "RSUB"){
            n = 1;
            i = 1;
            x = 0;
            b = 0;
            p = 0;
            e = 0;
        }

        opcode_value <<=  16;
        opcode_value += ((n << 17) + (i << 16) + (x << 15) + (b << 14) + (p << 13) + (e << 12));
        opcode_value |= (displcement & 0x00000FFF) ;
        to_object_str(opcode_value, format);
    }


}

string PassTwo::to_object_str(int int_opcode, int format, int length) const{

    stringstream objBuf;

    switch (format){
        case 1:{
            objBuf << setw(2) << setfill('0') << hex << int_opcode;
            return objBuf.str();
        }
        case 2:{
            objBuf << setw(4) << setfill('0') << hex << int_opcode;
            return objBuf.str();
        }
        case 3:{
            objBuf << setw(6) << setfill('0') << hex << int_opcode;
            return objBuf.str();
        }
        case 4:{
            objBuf << setw(8) << setfill('0') << hex << int_opcode;
            return objBuf.str();
        }
        case 5:
        case 6:{
            if(length == 1){
                objBuf << setw(2) << setfill('0') << hex << int_opcode;
                return objBuf.str();
            }
            else if(length == 2){
                objBuf << setw(4) << setfill('0') << hex << int_opcode;
                return objBuf.str();
            }
            else{
                objBuf << setw(6) << setfill('0') << hex << int_opcode;
                return objBuf.str();
            }
        }
        default: {
            cerr << "Error, in fuction to_object_str\n";
            return "";
        }
    }
}

bool PassTwo::_is_relative_mod(int &disp, int BASE) {

    string cp_operand1;
    if(_operand1[0] == '@' || _operand1[0] == '#'){
        cp_operand1.assign(_operand1, 1, _operand1.length() - 1 );
    }
    else{
        cp_operand1 = _operand1;
    }



    if(_shared_symbolTable.count(cp_operand1) == 0){
        cerr<<"Error, symbol not defined.\n";
        return false;
    }
    int program_counter = _LOCCTR + _op_length;

    // PC relative
    disp = _shared_symbolTable[cp_operand1] - program_counter;
    if(disp >= -2048 && disp <= 2048){
        return true;
    }

    // Base relative
    disp = _shared_symbolTable[cp_operand1] - BASE;
    if(disp < 4096){
        return false;
    }
    else{
        cerr<<"Error, addressing mod is invalid\n";
        return false;
    }

}


