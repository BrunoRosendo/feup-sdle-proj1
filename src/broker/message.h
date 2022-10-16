#ifndef MESSAGE_H
#define MESSAGE_H

#include <sstream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <unistd.h>

#define REQUEST_TIMEOUT     2500    //  msecs, (> 1000!)
#define REQUEST_RETRIES     3       //  Before we abandon
#define SERVER_URL "tcp://localhost:5555"


#define SUCCESS_MSG "SUCCESS"
#define FAIL_MSG "FAIL"

#define ACK_MSG "ACK"
#define GET_MSG "get"
#define PUT_MSG "put"
#define UNSUBSCRIBE_MSG "unsub"
#define SUBSCRIBE_MSG "sub"
 
#endif
