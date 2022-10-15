//
// Lazy Pirate server
// Binds REQ socket to tcp://*:5555
// Like hwserver except:
// - echoes request as-is
// - randomly runs slowly, or exits to simulate a crash.
//
#include <zmqpp/zmqpp.hpp>
#include <unistd.h>

#define within(num) (int) ((float)((num) * random ()) / (RAND_MAX + 1.0))
using namespace std;

int main ()
{
    srandom ((unsigned) time (NULL));

    zmqpp::context_t context;
    zmqpp::socket_t server(context, zmqpp::socket_type::reply);
    server.bind("tcp://*:5555");
    cout << "I: listening on tcp://*:5555" << endl;
 
    int cycles = 0;
    while (1) {
        string request;
        zmqpp::message message;
        server.receive(message);
        message >> request;
        cycles++;

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
        sleep (1); // Do some heavy work
        server.send(request);
    }
    return 0;
}
