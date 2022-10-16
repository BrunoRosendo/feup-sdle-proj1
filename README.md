# SDLE First Assignment

SDLE First Assignment of group T&lt;m&gt;&lt;n&gt;G&lt;p&gt;&lt;q&gt;.

Group members:

1. &lt;first name&gt; &lt;family name&gt; (&lt;email address&gt;)
2. &lt;first name&gt; &lt;family name&gt; (&lt;email address&gt;)
3. &lt;first name&gt; &lt;family name&gt; (&lt;email address&gt;)
4. &lt;first name&gt; &lt;family name&gt; (&lt;email address&gt;)

## How to run


### Broker
To run broker you simple need to run "./broker_exec"

### Client
To run client, you must specify the operation, client id, topic id and an optional message, accordint to the operation:

- ./client \<op> clientId topicId \<message>
- ./client get 1 ola
- ./client put 1 ola "ola topic1"

## Data

### Client data
The data stored in Get data are folders which the name are the clientId.
Those folders have files which the name is topicId and its
information is the current messageId of the missing message on that topic.
