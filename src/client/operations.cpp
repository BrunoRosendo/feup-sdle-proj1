#include "operations.h"
#include "message.h"

using namespace std;

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
