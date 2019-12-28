//
// Created by Guan Ying Chen on 2019/12/26.
//

#include "PassTwo.h"
#include "OP_Table.h"
#include "Register_Table.h"
#include <iostream>
#include <sstream>
#include <cctype>
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
    _outfile<<read<<endl;

    while (getline(_infile, read)){
        clear();
        cout<<"now read line : "<<read<<endl;
        _parseLine(read);

        if(_opcode != "END"){
            if(_generate_object_code()){
                _write_to_txrecord();
                _outfile<<setw(4)<<setfill('0')<<hex<<right<<_LOCCTR<<"    "<<setfill(' ')<<setw(6)<<left<<_symbol<<"    "<<setw(6)<<_origin_opcode<<"    "<<setw(10)<<_origin_operand<<"    "<<setw(8)<<_obj_code<<endl;
            }
            else{
                if(_opcode == "BASE"){
                    _outfile<<"    "<<"    "<<setfill(' ')<<setw(6)<<left<<_symbol<<"    "<<setw(6)<<_origin_opcode<<"    "<<setw(10)<<_origin_operand<<"    "<<setw(8)<<_obj_code<<endl;
                }
                else{
                    _outfile<<setw(4)<<setfill('0')<<hex<<right<<_LOCCTR<<"    "<<setfill(' ')<<setw(6)<<left<<_symbol<<"    "<<setw(6)<<_origin_opcode<<"    "<<setw(10)<<_origin_operand<<"    "<<endl;
                }
            }
        }
        else{
            _outfile<<read<<endl;
            _write_to_txrecord();
            break;
        }
    }

    // Write format4 vec
    for(auto i : _format4_vec){
        _out_txrecord<<"M"<<setfill('0')<<setw(6)<<hex<<i<<"05"<<endl;
    }
    _out_txrecord<<"E000000";

    _infile.close();
    _outfile.close();
    _out_txrecord.close();

}

void PassTwo::_parseLine(const std::string &line) {
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
    if(sub_line.size() == 2){
        // If is hex digit, get loc
        if(sub_line[0] != "BASE" && sub_line[0] != "END"){
            _LOCCTR = strtol(sub_line[0].c_str(), NULL, 16);
            offset_idx ++;
        }
    }
    else{
        _LOCCTR = strtol(sub_line[offset_idx].c_str(), NULL, 16);
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
    // Remove prfix '+'
    if(_origin_opcode[0] == '+'){
        _opcode.assign(_origin_opcode, 1, _origin_opcode.length()-1);
    }
    else{
        _opcode = _origin_opcode;
    }
    offset_idx++;

    // If have OPERAND, get OPERAND
    if(offset_idx < sub_line.size()){
        _origin_operand = sub_line[offset_idx];
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

PassTwo::PassTwo(const std::string &inUrl, const std::string &outUrl, const std::string &out_resultUrl): PassOne(inUrl, outUrl), _out_txrecord(out_resultUrl) {}

void PassTwo::_write_to_txrecord() {
    static int start_addr = 0;
    static int pre_state = 1;
    // Is line full
    if(60 - _line_buffer.length() < 2 * _op_length){

        stringstream buf;
        buf<<"T"<<setw(6)<<setfill('0')<<hex<<start_addr<<(_line_buffer.length())/2;

        string tmp;
        buf >> tmp;
        for(auto &i:tmp){
            i = toupper(i);
        }
        // Write to output txrecord file
        _out_txrecord << tmp <<_line_buffer<<endl;

        // New Line
        _line_buffer = _obj_code;
        start_addr = _LOCCTR;

    }
    else if(pre_state == 1){
        _line_buffer = _obj_code;
        start_addr = _LOCCTR;
        pre_state = 0;
    }
    else if(pre_state == 2){
        stringstream buf;
        buf<<"T"<<setw(6)<<setfill('0')<<hex<<start_addr<<(_line_buffer.length())/2;

        string tmp;
        buf >> tmp;
        for(auto &i:tmp){
            i = toupper(i);
        }

        // Write to output txrecord file
        _out_txrecord << tmp <<_line_buffer<<endl;

        // New Line
        _line_buffer = _obj_code;
        start_addr = _LOCCTR;

        pre_state = 0;
    }
    else if(_opcode == "END"){
        stringstream buf;
        buf<<"T"<<setw(6)<<setfill('0')<<hex<<start_addr<<(_line_buffer.length())/2;

        string tmp;
        buf >> tmp;
        for(auto &i:tmp){
            i = toupper(i);
        }
        // Write to output txrecord file
        _out_txrecord <<tmp <<_line_buffer<<endl;
    }
    else{
        _line_buffer += _obj_code;

        if(_symbol == "EOF"){
            pre_state = 2;
        }
    }

}

void PassTwo::clear() {
    PassOne::clear();
    _obj_code = "";
}


bool PassTwo::_generate_object_code() {
    int n, i, x, b, p, e;
    int displcement = 0;
    static int BASE = 0;

    int format = _getFormat(_opcode);

    int opcode_value = 0;

    if(format == 1){
        if(!OP_Table::getInstance().find(_opcode)){
            cerr<<"Error, OPCODE not found,\n";
            return false;
        }
        else{
            opcode_value = OP_Table::getInstance().get(_opcode).opcode;
        }
        _obj_code = to_object_str(opcode_value, format);
        return true;
    }
    if(format == 2){
        if(!OP_Table::getInstance().find(_opcode)){
            cerr<<"Error, OPCODE not found,\n";
            return false;
        }
        else{
            opcode_value = OP_Table::getInstance().get(_opcode).opcode;
        }
        // Shift left 8 bits
        opcode_value <<= 8;

        if(!_operand2.empty()){
            // Two OPERAND
            opcode_value += (Register_Table::getInstance().get(_operand1) << 4) | (Register_Table::getInstance().get(_operand2));
        }
        else{
            // One OPERAND
            int temp = Register_Table::getInstance().get(_operand1);
            opcode_value += (temp << 4);
        }

        _obj_code = to_object_str(opcode_value, format);
        return true;
    }
    if(format == 3){
        if(!OP_Table::getInstance().find(_opcode)){
            cerr<<"Error, OPCODE not found,\n";
            return false;
        }
        else{
            opcode_value = OP_Table::getInstance().get(_opcode).opcode;
        }
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
        _obj_code = to_object_str(opcode_value, format);
        return true;
    }
    if(format == 4){
        if(!OP_Table::getInstance().find(_opcode)){
            cerr<<"Error, OPCODE not found,\n";
            return false;
        }
        else{
            opcode_value = OP_Table::getInstance().get(_opcode).opcode;
        }
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
        else{
            // Direct addressing
            string cp_operand1 = _operand1;
            if((_operand1[0] == '@') || (_operand1[0] == '#')){
                cp_operand1.assign(cp_operand1, 1, cp_operand1.length()-1);
            }

            displcement = _shared_symbolTable[cp_operand1];
            _format4_vec.push_back(_LOCCTR+1);
            b = 0;
            p = 0;

        }
        // Is index address
        if( !_operand2.empty() && _operand2[0] == 'X'){
            x = 1;
        }
        else{
            x = 0;
        }
        e = 1;

        opcode_value <<=  24;
        opcode_value += ((n << 25) + (i << 24) + (x << 23) + (b << 22) + (p << 21) + (e << 20));
        opcode_value += displcement;
        _obj_code = to_object_str(opcode_value, format);
        return true;
    }
    if(_opcode == "BYTE"){
        int s_length = 1;
        if(_operand1[0] == 'C'){
            opcode_value = (int)_operand1[2];
            for(int j = 3; _operand1[j] != '\''; j++, s_length++){
                opcode_value = (int)_operand1[j] + (opcode_value << 8);
            }
        }
        else{
            string tmp;
            tmp.assign(_operand1, 2, 2);
            opcode_value = strtol(tmp.c_str(), NULL, 16);
        }
        _obj_code = to_object_str(opcode_value, format, s_length);
        return true;

    }
    if(_opcode == "WORD"){
        opcode_value = strtol(_operand1.c_str(), NULL, 10);
        int s_length = 1;
        _obj_code = to_object_str(opcode_value, format, s_length);
        return true;
    }
    // OPCODE == "BASE"
    if(format == 7){
        BASE = _shared_symbolTable[_operand1];
        return false;
    }

    if(format == 0){
        return false;
    }
    cerr<<"Error, in _generate_object_code\n";
    return false;



}

string PassTwo::to_object_str(int int_opcode, int format, int length) const{

    stringstream objBuf;
    switch (format){
        case 1:{
            objBuf << setw(2) << setfill('0') << hex << int_opcode;
            break;
        }
        case 2:{
            objBuf << setw(4) << setfill('0') << hex << int_opcode;
            break;
        }
        case 3:{
            objBuf << setw(6) << setfill('0') << hex << int_opcode;
            break;
        }
        case 4:{
            objBuf << setw(8) << setfill('0') << hex << int_opcode;
            break;
        }
        case 5:
        case 6:{
            if(length == 1){
                objBuf << setw(2) << setfill('0') << hex << int_opcode;
                break;
            }
            else if(length == 2){
                objBuf << setw(4) << setfill('0') << hex << int_opcode;
                break;
            }
            else{
                objBuf << setw(6) << setfill('0') << hex << int_opcode;
                break;
            }
        }
        default: {
            cerr << "Error, in fuction to_object_str\n";
            break;
        }
    }
    string re;
    objBuf >> re;
    for(auto &i:re){
        i = toupper(i);
    }
    return re;
}

bool PassTwo::_is_relative_mod(int &disp, int BASE) const {

    string cp_operand1;
    if(_operand1[0] == '@' || _operand1[0] == '#'){
        cp_operand1.assign(_operand1, 1, _operand1.length() - 1 );
    }
    else{
        cp_operand1 = _operand1;
    }


    if(cp_operand1.empty()){
        return false;
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


