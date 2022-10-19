#include "message.h"
#include "operations.h"

#include <iostream>
#include <stdio.h>

using namespace std;


int main(int argc, char** argv) {
    // ./client <op> clientId topicId <"message">
    if (argc < 4) {
        cout << "Usage: client_exec <op> <clientId> <topicId> [message]" << "\n";
        exit(1);
    }

    string op = argv[1];
    string clientId = argv[2];
    string topicId = argv[3];

    try {
        parse_op(argc, op);
    } catch (string msg) {
        cout << msg << "\n";
        exit(1);
    }

    try {
        check_subscription(clientId, topicId, op);
    } catch (string msg) {
        cout << msg << "\n";
        cout << "ERROR: could not restore subscription state from failure" << "\n";
        exit(1);
    }

    string messageId = get_last_message_id(clientId, topicId, op);
    string message = parse_message(argc, argv, &messageId);
    // Save in case of crash
    string fileName = save_message_id(clientId, topicId, op, messageId);

    try {
        cout << "message ID:  " << messageId << endl;
        process_operation(op, message);
        // if the message was sent successfully, then delete the last message from file
        remove(fileName.c_str());
    }
    catch (string msg)
    {
        cout << "error for: " << messageId << endl;
        cout << "ERROR: " << msg << endl;
        exit(1);
    }

    return 0;
}
