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

    vector<string> argVector = vector<string>(argv + 1, argv + argc);
    string lastMessage = get_last_message(clientId, topicId, op);
    // if lost a message then send it to broker
    if (!lastMessage.empty()) {
        argVector.push_back(lastMessage);
    }

    string message = parse_message(argc, argv);

    try {
        string response = send_message(message);
        cout << "Server response: " << response << endl;
        // if the message was sent successfully, then delete the last message from file
        string fileName = get_filename(clientId, topicId, op);
        remove(fileName.c_str());
    }
    catch (string msg)
    {
        cout << "ERROR: " << msg << endl;
        string messageId = message.substr(0, message.find("\n"));
        // if the message was not sent successfully, then save it to file
        save_error_message(clientId, topicId, op, messageId);
    }

    return 0;
}
