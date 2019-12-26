
#include "PassOne.h"
using namespace std;


int main() {
    string in_file_url, out_file_url;
#if defined(_WIN64) || defined(_WIN32)
    in_file_url = "..\\..\\test_sic_code\\sic_code.txt"";
    out_file_url = "pass1_result.txt";
#else
    in_file_url = "../../test_sic_code/sic_code.txt";
    out_file_url = "pass1_result.txt";
#endif
    PassOne ps1(in_file_url, out_file_url);
    ps1.perform();
    return 0;
}
