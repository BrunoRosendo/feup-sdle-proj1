#include <zmqpp/zmqpp.hpp>
#include <sstream>

#include "TopicMessage.hpp"
#include "TopicManager.hpp"

#define within(num) (int) ((float)((num) * random ()) / (RAND_MAX + 1.0))

using namespace std;

int main() {
    zmqpp::context_t context;
    zmqpp::socket_t server(context, zmqpp::socket_type::reply);
    server.bind("tcp://*:5555");
    cout << "I: listening on tcp://*:5555" << endl;

    TopicManager topicManager = TopicManager();

    while (1) {
        string request;
        zmqpp::message message;
        server.receive(message);
        message >> request;
        
        cout << "I: normal request (" << request << ")" << endl;

        stringstream ss(request);
        
        string method;
        getline(ss, method);
        if (method == "put") {
            // TODO: assuming that the request comes in the format:
            // method (one line)
            // topic (one line)
            // content (several lines)

            string topic;
            string content;
            getline(ss, topic);
            content = ss.str();

            // TODO: considere id for each topic

            TopicMessage topicMessage(topic, content);
            // TODO: check if topic already exists
            topicManager.addTopic(topicMessage);
        }


        //  Send reply back to client
        zmqpp::message reply;
        reply << "OK";
        server.send(reply);
        
    }
    return 0;

}