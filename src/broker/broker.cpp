//
// Lazy Pirate server
// Binds REQ socket to tcp://*:5555
// Like hwserver except:
// - echoes request as-is
// - randomly runs slowly, or exits to simulate a crash.
//
#include <zmqpp/zmqpp.hpp>
#include <unistd.h>
#include <sstream>
#include "message.h"
#include "TopicManager.h"

#define within(num) (int) ((float)((num) * random ()) / (RAND_MAX + 1.0))

using namespace std;

int main ()
{
    srandom ((unsigned) time (NULL));

    zmqpp::context_t context;
    zmqpp::socket_t server(context, zmqpp::socket_type::reply);
    server.bind("tcp://*:5555");
    cout << "I: listening on tcp://*:5555" << endl;
 
    TopicManager topicManager = TopicManager();

    int cycles = 0;
    while (1) {
        string reply = SUCCESS_MSG;
        string request;
        zmqpp::message message;
        server.receive(message);
        message >> request;

        stringstream ss(request);
        string id;
        string op;
        string clientId;
        string topicId;

        getline(ss, id);
        getline(ss, op);
        getline(ss, clientId);
        getline(ss, topicId);

        cout << "Received operation " << op << " from client " << clientId << " with id " << id << " for topic " << topicId << endl;
        try {
            if (op == PUT_MSG) {
                string payload;
                getline(ss, payload);
                topicManager.handlePut(topicId, payload);
            } else if (op == SUBSCRIBE_MSG) {
                topicManager.handleSubscription(topicId, clientId);
            } else if (op == UNSUBSCRIBE_MSG) {
                topicManager.handleUnsubscription(topicId, clientId);
            } else if (op == GET_MSG) {
                string payload = topicManager.handleGet(topicId, clientId);
                reply += " " + payload;
            } else {
                string err = "Invalid operation";
                throw err;
            }
        }
        catch (string error){
            stringstream ss(FAIL_MSG);
            ss << " " << error;
            reply = ss.str();
        }
        
        sleep (1); // Do some heavy work
        server.send(reply);

        /*

        // Simulate various problems, after a few cycles
        if (cycles > 3 && within (3) == 0) {
            cout << "I: simulating a crash" << endl;
            break;
        }
        else if (cycles > 3 && within (3) == 0) {
            cout << "I: simulating CPU overload" << endl;
            sleep (2);
        }
        cout << "I: normal request (" << request << ")" << endl;
        */
        
    }
    return 0;
}
