#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>

using namespace std;

/**
 * Checks if the operation is valid. Throws exception if the input is invalid
 * @param argc number of arguments
 * @param op operation to be performed
*/
void parse_op(int argc, string op);

void process_operation(string op, string msg);
string get_operation(string msg);
bool confirmed_operation(string msg);

#endif
