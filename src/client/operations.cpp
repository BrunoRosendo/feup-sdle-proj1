#include "operations.h"
#include "message.h"

using namespace std;

int parse_op(int argc, string op) {
  if (op != GET_MSG && op != SUBSCRIBE_MSG && op != UNSUBSCRIBE_MSG) {
    if (op != PUT_MSG) {
        cout << "Operand should be 'get', 'put', 'sub' or 'unsub'" << "\n";
        return 1;
    }
    else if (argc < 5) {
        cout << "Usage: client_exec <op> <clientId> <topicId> [\"message\"]" << "\n";
        return 1;
    }
  } else if (argc > 4) {
      cout << "On 'get', 'sub' and 'unsub' operations, the message should not be specified" << "\n";
      return 1;
  }

  return 0;
}


void process_operation(string op, string msg) {
  if (op == GET_MSG) {
    get_operation(msg);
  } else if (op == PUT_MSG || op == UNSUBSCRIBE_MSG || op == SUBSCRIBE_MSG) {
    if (!confirmed_operation(msg)) return;
  }

  cout << "Operation successful!" << endl;
}

string get_operation(string msg) {
  string res = send_message(msg);
  cout << "Received message from the topic: " << res << endl;
  return res;
}

bool confirmed_operation(string msg) {
  string res = send_message(msg);
  if (res != ACK_MSG) {
    cout << "ERROR: server has the wrong specification!" << endl;
    return false;
  }
  return true;
}
