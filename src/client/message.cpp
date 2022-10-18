#include "message.h"
#include <ctime>
#include <filesystem>
#include <functional>

using namespace std;

/*
  Helper function that returns a new configured socket
  connected to the Hello World server
*/
zmqpp::socket_t s_client_socket (zmqpp::context_t & context) {
    zmqpp::socket_t client(context, zmqpp::socket_type::req);
    client.connect(SERVER_URL);

    //  Configure socket to not wait at close time
    int linger = 0;
    client.set(zmqpp::socket_option::linger, linger);
    return client;
}
 
string send_message(string msg) {
    zmqpp::context_t context;
    zmqpp::socket_t client = s_client_socket(context);

    int retries_left = REQUEST_RETRIES;

    while (true) {
        client.send(msg);
        sleep(1); // TODO what is this for?

        bool expect_reply = true;
        while (expect_reply) {
            //  Poll socket for a reply, with timeout
            zmqpp::poller poller;
            poller.add(client);
            poller.poll(REQUEST_TIMEOUT);

            //  If we got a reply, process it
            if (poller.events(client) & ZMQ_POLLIN) {
                string reply;
                client.receive(reply);
                return reply;
            } else if (--retries_left == 0) {
                client.close();
                throw string("server seems to be offline, abandoning");
            }
            else
            {
                cout << "WARNING: no response from server, retrying..." << endl;

                //  Old socket is confused; close it and open a new one
                client.close();
                client = s_client_socket(context);
                //  Send request again, on new socket
                client.send(msg);
            }
        }
    }
}


string parse_message(int size, char** raw_msg) {
    stringstream ss_msg;
    for (int i = 1; i < size; i++) {
        ss_msg << raw_msg[i] << "\n";
    }
    string message = ss_msg.str();
    message = message.substr(0, message.length() - 1);

    string id = get_hash(message);
    return id + "\n" + message;
}

// TODO Should this be done for sub/unsub as well? Maybe do for every message?
string get_filename(string clientId, string topicId, string op) {
    if (op == GET_MSG) {
        return FOLDER_PATH + clientId + "/" + GET_PATH + topicId + ".txt";
    } else if (op == PUT_MSG) {
        return FOLDER_PATH + clientId + "/" + PUT_PATH + topicId + ".txt";
    }

    return "";
}

string get_last_message(string clientId, string topicId, string op){
    ifstream file;
    string fileName = get_filename(clientId, topicId, op);

    string lastMessage;
    file.open(fileName);
    if (file.is_open()) {
      if (file.peek() != EOF) {
          getline(file, lastMessage);
      }
      file.close();
    }

    return lastMessage;
}

void save_error_message(string clientId, string topicId, string op, string message) {
    string directory = FOLDER_PATH + clientId;;
    if (!filesystem::is_directory(directory) || !filesystem::exists(directory))
        filesystem::create_directories(directory);

    ofstream file;
    string fileName = get_filename(clientId, topicId, op);

    file.open(fileName); // TODO is truncate right?
    file << message;
    file.close();
}

string get_hash(string msg) {
    time_t result = time(nullptr);
    msg = msg + asctime(localtime(&result));
    size_t str_hash = hash<string>{}(msg);
    return to_string(str_hash);
}
