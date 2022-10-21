# SDLE First Assignment

SDLE First Assignment of group T04G11.

Group members:

1. Bruno Rosendo (up201906334@fe.up.pt)
2. Henrique Nunes (up201906852@fe.up.pt)
3. João Mesquita (up201906682@fe.up.pt)
4. Rui Alves (up201905853@fe.up.pt)

## How to run

First of all, you must build the project by simply running `make` under the `src/` folder.

### Broker
To run broker you simple need to run "./broker_exec"

### Client
To run client, you must specify the operation, client id, topic id and an optional message, accordint to the operation:

- ./client_exec \<op> clientId topicId \<message>
- ./client_exec get 1 topic1
- ./client_exec put 1 topic1 "this is a nice message"

## Data

### Client data
The data stored in the client is contained in folders whose names are the client IDs.
Those folders have files whose name is composed by the operation and the topic's ID. Its
information is the current message ID of the missing message on that topic.
