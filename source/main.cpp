
#include "PassOne.h"
using namespace std;

int main() {
    PassOne ps1("..\\..\\test_sic_code\\sic_code.txt", "output.txt");
    ps1.perform();
    return 0;
}
