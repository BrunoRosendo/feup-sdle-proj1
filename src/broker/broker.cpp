#include <zmqpp/zmqpp.hpp>
#include <unistd.h>
#include <sstream>
#include "message.h"
#include "TopicManager.h"

using namespace std;

int main ()
{
    srandom ((unsigned) time (NULL));

    zmqpp::context_t context;
    zmqpp::socket_t server(context, zmqpp::socket_type::reply);
    server.bind(SERVER_URL);
    cout << "I: listening on " << SERVER_URL << endl;
 
    TopicManager topicManager = TopicManager();

    while (true) {
        string reply = SUCCESS_MSG;
        string request;
        zmqpp::message message;
        server.receive(message);
        message >> request;

        stringstream ss(request);
        string messageId;
        string op;
        string clientId;
        string topicId;

        getline(ss, messageId);
        getline(ss, op);
        getline(ss, clientId);
        getline(ss, topicId);

        cout << "Received operation " << op << " from client " << clientId << " with id " << messageId << " for topic " << topicId << endl;
        try {
            if (op == PUT_MSG) {
                string payload;
                getline(ss, payload);
                topicManager.handlePut(topicId, messageId, payload);
            } else if (op == SUBSCRIBE_MSG) {
                topicManager.handleSubscription(topicId, clientId, messageId);
            } else if (op == UNSUBSCRIBE_MSG) {
                topicManager.handleUnsubscription(topicId, clientId, messageId);
            } else if (op == GET_MSG) {
                string payload = topicManager.handleGet(topicId, clientId, messageId);
                reply = payload;
            } else {
                string err = "Invalid operation";
                throw err;
            }
            topicManager.serialize(); // save state
        }
        catch (string error){
            stringstream ss;
            ss << FAIL_MSG << " " << error;
            reply = ss.str();
        }

        server.send(reply);       
    }
    return 0;
}
