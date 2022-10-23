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
To run the broker you simple need to run "./broker_exec"

### Client
To run the client, you must specify the operation, client id, topic id and an optional message, according to the operation:

- client_exec <operation> <clientId> <topicId> [message]
- client_exec get 1 topic1
- client_exec put 1 topic1 "this is a nice message"

## Data

### Client data
The data stored in the client is contained in folders whose names are the client IDs.
Those folders have files whose name is composed by the operation and the topic's ID. Its
information is the current message ID of the missing message on that topic.


### Broker’s topic_manager.txt file structure:
```
numberTopics
for each topic:
	topicName
	numberMessages
	for each message:
		messageId
		message
	numberSubscribers
	for each subscriber:
		clientId
		lastMessageId
		lastReadMsgIndex
numberTopics
for each topic:
	topicName
	numberTopicMessages
	for each message:
		messageId
numberTopics
for each topic:
	topicName
	lastMessagesTopicSize
	for each client:
		clientId
		lastSubMsgId
		lastUnsubMsgId
```

### Clients’ saved files structure:

File name Explanation:

- The Client folder contains a folder for each clientID (named after its id).
- Inside a Client’s folder, they can have the following files:
    - get_<topic_name>.txt
    - put_<topic_name>.txt
    - sub_<topic_name>.txt
    - unsub_<topic_name>.txt
- Each file contains a `MessageID`
