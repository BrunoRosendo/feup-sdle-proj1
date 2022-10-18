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

/**
 * Processes de operation by sending the message with the corresponding operation
 * @param op operation to be performed
 * @param message message to be sent
 * @return true if got response from the server (success), false otherwise (failure)
*/
bool process_operation(string op, string msg);
bool get_operation(string msg);
bool confirmed_operation(string msg);

/**
 * Checks if there is a pending subscription or unsubscription. 
 * If there is, make it.
 * @param clientId client id
 * @param topicId topic id
*/
void check_subscription(string clientId, string topicId);

#endif
