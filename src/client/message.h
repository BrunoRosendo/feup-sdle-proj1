#ifndef MESSAGE_H
#define MESSAGE_H

#include <sstream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <unistd.h>
#include <iostream>
#include <fstream>

#define REQUEST_TIMEOUT     2500    //  msecs, (> 1000!)
#define GET_WAIT_TIME       3          //  secs
#define REQUEST_RETRIES     3       //  Before we abandon
#define ABORT_ON_FAILURE   false    
#define SERVER_URL "tcp://localhost:5555"

#define ACK_MSG "[SUCCESS]"
#define NACK_MSG "[FAIL]"
#define NO_MSGS_MSG "[FAIL] No new messages for the client"
#define GET_MSG "get"
#define PUT_MSG "put"
#define UNSUBSCRIBE_MSG "unsub"
#define SUBSCRIBE_MSG "sub"


using namespace std;

const string FOLDER_PATH = "../data/client/";
const string GET_PATH = "get_";
const string PUT_PATH = "put_";
const string SUB_PATH = "sub_";
const string UNSUB_PATH = "unsub_";

 
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
string parse_message(int size, char** raw_msg, string* lastId);

/**
 * Gets the last message id that the client wanted to send to a topic and failed
 * (this message is stored in a file)
 * @param clientId client's id
 * @param topicId topic's id
 * @param op operation to be performed
 * @return last message id
*/
string get_last_message_id(string clientId, string topicId, string op);

/**
 * Saves the message that the client wanted to send to a topic and failed in the corresponding
 * file of the clientId, topicId and operation
 * @param clientId client's id
 * @param topicId topic's id
 * @param op operation to be performed
 * @param message message to be saved
*/
string save_message_id(string clientId, string topicId, string op, string message);

/**
 * Gets the filename of the corresponding clientId, topicId and operation
 * @param clientId client's id
 * @param topicId topic's id
 * @param op operation to be performed
 * @return filename
*/
string get_filename(string clientId, string topicId, string op);

string get_hash(string msg);

/**
 * Creates a new message with the given parameters
 * @param clientId client id
 * @param topicId topic id
 * @param op operation to be performed
 * @param msg message to be sent
 * @return message
*/
string create_message(string message_id, string client_id, string topicId, string op, string msg = "");

#endif
