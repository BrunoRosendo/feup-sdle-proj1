//
//  Lazy Pirate client
//  Use zmq_poll to do a safe request-reply
//  To run, start piserver and then randomly kill/restart it
//
#include <zmqpp/zmqpp.hpp>
#include <sstream>

#define REQUEST_TIMEOUT     2500    //  msecs, (> 1000!)
#define REQUEST_RETRIES     3       //  Before we abandon

//  Helper function that returns a new configured socket
//  connected to the Hello World server
//
static zmqpp::socket_t s_client_socket (zmqpp::context_t & context) {
    std::cout << "I: connecting to server..." << std::endl;
    zmqpp::socket_t client(context, zmqpp::socket_type::req);
    client.connect("tcp://localhost:5555");

    //  Configure socket to not wait at close time
    int linger = 0;
    client.set(zmqpp::socket_option::linger, linger);
    return client;
}

int main () {
    zmqpp::context_t context;

    // TODO: CHECK IF THERE's any issue with this being a copy
    zmqpp::socket_t client = s_client_socket(context);

    int sequence = 0;
    int retries_left = REQUEST_RETRIES;

    while (retries_left) {
        std::stringstream request;
        request << ++sequence;
        client.send(request.str());
        sleep(1);

        bool expect_reply = true;
        while (expect_reply) {
            //  Poll socket for a reply, with timeout
            zmqpp::poller poller;
            poller.add(client);
            poller.poll(REQUEST_TIMEOUT);

            //  If we got a reply, process it
            if (poller.events(client) & ZMQ_POLLIN) {
                //  We got a reply from the server, must match sequence
                std::string reply;
                client.receive(reply);
                if (sequence == std::stoi(reply)) {
                    std::cout << "I: server replied OK (" << reply << ")" << std::endl;
                    retries_left = REQUEST_RETRIES;
                    expect_reply = false;
                } else {
                    std::cout << "E: malformed reply from server: " << reply << std::endl;
                }
            } else if (--retries_left == 0) {
                std::cout << "E: server seems to be offline, abandoning" << std::endl;
                expect_reply = false;
            } else {
                std::cout << "W: no response from server, retrying..." << std::endl;

                //  Old socket is confused; close it and open a new one
                client.close();
                client = s_client_socket(context);
                //  Send request again, on new socket
                client.send(request.str());
            }
        }
    }
    
    return 0;
}