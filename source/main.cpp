
#include "PassOne.h"
#include "PassTwo.h"

using namespace std;


int main() {
    string in_file_url, out_file_paas1, out_file_pass2, out_texr_record;
#if defined(_WIN64) || defined(_WIN32)
    in_file_url = "..\\..\\test_sic_code\\sic_code.txt";
    out_file_paas1 = "pass1_result.txt";
    out_file_pass2 = "pass2_result.txt";
    out_texr_record = "text_record.txt";
#else
    in_file_url = "../../test_sic_code/sic_code.txt";
    out_file_paas1 = "pass1_result.txt";
    out_file_pass2 = "pass2_result.txt";
    out_texr_record = "text_record.txt";
#endif

    PassOne ps1(in_file_url, out_file_paas1);
    ps1.perform();
    PassTwo ps2(out_file_paas1, out_file_pass2, out_texr_record);
    ps2.perform();

    return 0;
}
