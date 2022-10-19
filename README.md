# SDLE First Assignment

SDLE First Assignment of group T&lt;m&gt;&lt;n&gt;G&lt;p&gt;&lt;q&gt;.

Group members:

1. &lt;Bruno&gt; &lt;Rosendo&gt; (&lt;up201906334@fe.up.pt&gt;)
2. &lt;Henrique&gt; &lt;Nunes&gt; (&lt;up201906852@fe.up.pt&gt;)
3. &lt;João&gt; &lt;Mesquita&gt; (&lt;up201906682@fe.up.pt&gt;)
4. &lt;Rui&gt; &lt;Alves&gt; (&lt;up201905853@fe.up.pt&gt;)

## How to run

First of all, you must build the project by simply running `make` under the `src/` folder.

### Broker
To run broker you simple need to run "./broker_exec"

### Client
To run client, you must specify the operation, client id, topic id and an optional message, accordint to the operation:

- ./client \<op> clientId topicId \<message>
- ./client get 1 topic1
- ./client put 1 topic1 "this is a nice message"

## Data

### Client data
The data stored in the client is contained in folders whose names are the client IDs.
Those folders have files whose name is the topic's ID and its
information is the current message ID of the missing message on that topic.
