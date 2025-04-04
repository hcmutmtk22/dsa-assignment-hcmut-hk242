#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "test/tc_dlinkedlist.h"
#include "test/tc_xarraylist.h"
#include "test/tc_inventory.h"

using namespace std;

void (*func_ptr[17])() = {
    dlistDemo1,
    dlistDemo2,
    dlistDemo3,
    dlistDemo4,
    dlistDemo5,
    dlistDemo6,
    xlistDemo1,
    xlistDemo2,
    xlistDemo3,
    xlistDemo4,
    tc_inventory1001,
    tc_inventory1002,
    tc_inventory1003,
    tc_inventory1004,
    tc_inventory1005,
    tc_inventory1006,
    tc_inventory1007
};

void run(int func_idx)
{
    func_ptr[func_idx]();
}

int main(int argc, char **argv)
{
    // cout << "Assignment-1" << endl;
    if (argc == 1) {
        tc_inventory1006();
    } else {
        for (int i=1;i<argc; i++) {
            cout << endl << "====== TESTCASE " << argv[i] << ": =======" << endl;
            run(atoi(argv[i]));
        }
    }
    
    return 0;
}