#include "operations.h"
#include "message.h"

using namespace std;

void parse_op(int argc, string op) {
  if (op != GET_MSG && op != SUBSCRIBE_MSG && op != UNSUBSCRIBE_MSG) {
    if (op != PUT_MSG) {
        throw string("Operand should be 'get', 'put', 'sub' or 'unsub'");
    }
    else if (argc < 5) {
        throw string("Usage: client_exec <op> <clientId> <topicId> [\"message\"]");
    }
  } else if (argc > 4) {
      throw string("On 'get', 'sub' and 'unsub' operations, the message should not be specified");
  }
}


bool process_operation(string op, string msg) {
  if (op == GET_MSG) {
    if (!get_operation(msg)) return false;
  } else if (op == PUT_MSG || op == UNSUBSCRIBE_MSG || op == SUBSCRIBE_MSG) {
    if (!confirmed_operation(msg)) return false;
  }

  cout << "Operation successful!" << endl;
  return true;
}

bool get_operation(string msg) {
  string res = send_message(msg);
  if (res.rfind(NACK_MSG, 0) == 0) {
    cout << "ERROR (server): " << res << endl;
    return false;
  }

  cout << "Received message from the topic: " << res << endl;
  return true;
}

bool confirmed_operation(string msg) {
  string res = send_message(msg);
  if (res.rfind(NACK_MSG, 0) == 0) {
    cout << "ERROR (server): " << res << endl;
    return false;
  } else if (res != ACK_MSG) {
    cout << "ERROR: server has the wrong specification!" << endl;
    return false;
  }
  return true;
}


void check_subscription(string clientId, string topicId, string op) {
  // if the current operation is one of this, we dont need to restore.
  if (op == SUBSCRIBE_MSG || op == UNSUBSCRIBE_MSG)
    return;

  string lastSubId = get_last_message_id(clientId, topicId, SUBSCRIBE_MSG);
  string lastUnsubId = get_last_message_id(clientId, topicId, UNSUBSCRIBE_MSG);

  string fileName;
  if (!lastSubId.empty()) {
    string subMsg = create_message(lastSubId, clientId, topicId, SUBSCRIBE_MSG);
    if (!process_operation(SUBSCRIBE_MSG, subMsg)) {
      string err = "ERROR: could not subscribe to the topic";
      throw err;
    }

    fileName = get_filename(clientId, topicId, SUBSCRIBE_MSG);
  } else if (!lastUnsubId.empty()) {
    string unsubMsg = create_message(lastUnsubId, clientId, topicId, UNSUBSCRIBE_MSG);
    if (process_operation(UNSUBSCRIBE_MSG, unsubMsg)) {
      string err = "ERROR: could not unsubscribe from topic";
      throw err;
    }
    fileName = get_filename(clientId, topicId, UNSUBSCRIBE_MSG);
  } else {
    // there are no subscription messages to be made
    return;
  }

  remove(fileName.c_str());
}