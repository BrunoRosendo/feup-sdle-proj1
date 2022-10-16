#ifndef TOPICMANAGER_H
#define TOPICMANAGER_H

#include <iostream>
#include <map>
#include <deque>

using namespace std;

class TopicManager {
    private:
        map<string, pair<deque<string>, map<string, uint>>> topics;
    public:
        TopicManager();
        void createTopic(string topicName);
        /**
         * @brief Topics are deleted when they have no subscribers
        */
        void deleteTopic(string topicName);
        void addTopicMsg(string topicName, string msg);
        void removeTopicMsg(string topicName);
        
        void handleSubscription(string topicName, string clientId);
        void handleUnsubscription(string topicName, string clientId);
        void handlePut(string topicName, string message);
        string handleGet(string topicName, string clientId);
};

#endif