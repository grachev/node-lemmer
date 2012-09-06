#include <string>
#include <vector>

#ifndef MODULE_NAME_H
#define MODULE_NAME_H

#define PROCESS_OK   0
#define PROCESS_FAIL 1

using namespace std;

string execute_hello( const string &val ) {
    return string("Hello node, coffeescript and c++ ") + val;
}


#endif 