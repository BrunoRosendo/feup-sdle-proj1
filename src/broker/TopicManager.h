#ifndef TOPICMANAGER_H
#define TOPICMANAGER_H

#include <iostream>
#include <map>
#include <set>
#include <deque>

using namespace std;

typedef map<string, pair<string, uint>> subscribers; // <clientId, <messageId, nextReadMsgIndex>>
typedef deque<pair<string, string>> msgs; // <messageId, message>

class TopicManager {
    private:
        map<string, pair<msgs, subscribers>> topics; // <topicName, <messages, subscribers>>
        map<string, set<string>> messagesIds; // <topicName, <messageIds>>
    public:
        TopicManager();
        void createTopic(string topicName);
        /**
         * @brief Topics are deleted when they have no subscribers
        */
        void deleteTopic(string topicName);
        void addTopicMsg(string topicName, string messageId, string msg);
        void removeTopicMsg(string topicName);
        
        void handleSubscription(string topicName, string clientId);
        void handleUnsubscription(string topicName, string clientId);
        void handlePut(string topicName, string messageId, string message);
        string handleGet(string topicName, string clientId, string messageId);
};

#endif
