//
// Created by Guan Ying Chen on 2019/12/26.
//

#ifndef SOURCE_PASSTWO_H
#define SOURCE_PASSTWO_H

#include <fstream>
#include "PassOne.h"


class PassTwo : public PassOne{
public:
    PassTwo(const std::string &inUrl, const std::string &outUrl, const std::string &out_resultUrl);

    void perform() override ;
    void clear() override ;
private:
    const int _line_length = 70;
    std::string _line_buffer;
    std::string _obj_code;

    void _generate_object_code();
    std::string to_object_str(int int_opcode, int format, int length = 0) const;
    void _write_to_txrecord();
    void _parseLine(const std::string &line) override ;
    std::ofstream _out_txrecord;
};


#endif //SOURCE_PASSTWO_H
