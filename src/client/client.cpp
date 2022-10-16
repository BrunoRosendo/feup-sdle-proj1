#include "message.h"
#include "operations.h"

#include <iostream>
#include <stdio.h>


using namespace std;


int main(int argc, char** argv) {
    // ./client <op> clientId topicId <"message">
    if (argc < 4) {
        cout << "Usage: client_exec <op> <clientId> <topicId> [\"message\"]" << "\n";
        return 1;
    }

    string op = argv[1];
    string clientId = argv[2];
    string topicId = argv[3];

    if (parse_op(argc, op)) {
        exit(1);
    }

    string lastMessage = get_last_message(clientId, topicId, op);
    // if lost a message then send it to broker
    if (lastMessage.length() > 0) {
        argc++;
        argv[argc - 1] = (char*) lastMessage.c_str();
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
