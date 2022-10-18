#pragma once

#include <iostream>
#include <map>
#include <set>
#include <deque>
#include <filesystem>
#include <fstream>

#define DATA_PATH "../data/broker/"
#define DATA_FILE "../data/broker/topic_manager.txt"

using namespace std;

typedef map<string, pair<string, uint>> subscribers; // <clientId, <messageId, lastReadMsgIndex>>
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

        void serialize();
        friend istream& operator>> (istream& is, TopicManager& tm);
        friend ostream& operator<< (ostream& os, TopicManager& tm);
};

istream& operator>> (istream& is, TopicManager& tm);
ostream& operator<< (ostream& os, TopicManager& tm);
