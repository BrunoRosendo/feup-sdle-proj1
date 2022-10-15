#ifndef MESSAGE_H
#define MESSAGE_H

#include <sstream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <unistd.h>

#define REQUEST_TIMEOUT     2500    //  msecs, (> 1000!)
#define REQUEST_RETRIES     3       //  Before we abandon
#define SERVER_URL "tcp://localhost:5555"

#define ACK_MSG "ACK"
#define GET_MSG "get"
#define PUT_MSG "put"
#define UNSUBSCRIBE_MSG "unsub"
#define SUBSCRIBE_MSG "sub"

using namespace std;
 
/*
  Helper function that returns a new configured socket
  connected to the Hello World server
*/
zmqpp::socket_t s_client_socket (zmqpp::context_t & context);

/*
  Sends a message to the defined server and receives a response
*/
string send_message(string msg);

/**
  Parses the cmd line arguments into a formatted message
*/
string parse_message(int size, char** raw_msg);

string get_hash(string msg);

#endif
