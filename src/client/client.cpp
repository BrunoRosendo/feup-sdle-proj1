#include "message.h"
#include "operations.h"


using namespace std;

int main(int argc, char** argv) {
    // ./client <op> clientId topicId <"message">
    if (argc < 4) {
        cout << "Usage: ./client <op> clientId topicId <\"message\">" << "\n";
        return 1;
    }

    string op = argv[1];

    if (op != GET_MSG && op != SUBSCRIBE_MSG && op != UNSUBSCRIBE_MSG) {
        if (op != PUT_MSG) {
            cout << "Operand should be 'get', 'put', 'sub' or 'unsub'" << "\n";
            return 1;
        }
        else if (argc < 5) {
            cout << "Usage: ./client <op> clientId topicId <\"message\">" << "\n";
            return 1;
        }
    }

    string message = parse_message(argc, argv);

    try {
        string response = send_message(message);
        cout << "Server response: " << response << endl;
    } catch (string msg) {
        cout << "ERROR: " << msg << endl;
    }

    return 0;
}
