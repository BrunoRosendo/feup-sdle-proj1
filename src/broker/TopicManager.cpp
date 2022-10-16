#include "TopicManager.h"

// define the methods of the TopicManager class
TopicManager::TopicManager() {
    // nothing to do here
}

void TopicManager::handleSubscription(string topicName, string clientId) {
    // check if the topic exists
    if (topics.find(topicName) == topics.end()) {
        // if not, create it
        createTopic(topicName);
    }

    // add the client to the topic's subscribers
    if (topics[topicName].second.find(clientId) == topics[topicName].second.end()) {
        topics[topicName].second[clientId] = 0;
    }
    else {
        string err = "Client already subscribed to topic";
        throw err;
    }
}
void TopicManager::handleUnsubscription(string topicName, string clientId) {
    // check if the topic exists
    if (topics.find(topicName) == topics.end()) {
        string err = "Topic does not exist";
        throw err;
    }

    // remove the client from the topic's subscribers
    if (topics[topicName].second.find(clientId) != topics[topicName].second.end()) {
        topics[topicName].second.erase(clientId);
    }
    else {
        string err = "Client not subscribed to topic";
        throw err;
    }
}

void TopicManager::handlePut(string topicName, string message) {
    // check if the topic exists
    if (topics.find(topicName) == topics.end()) {
        string err = "Topic does not exist";
        throw err;
    }

    // add the message to the topic's queue
    addTopicMsg(topicName, message);
}
string TopicManager::handleGet(string topicName, string clientId) {
    // check if the topic exists
    if (topics.find(topicName) == topics.end()) {
        string err = "Topic does not exist";
        throw err;
    }

    // check if the client is subscribed to the topic
    if (topics[topicName].second.find(clientId) == topics[topicName].second.end()) {
        string err = "Client is not subscribed to the topic";
        throw err;
    }

    // get the client's next read message index
    uint nextReadMsgIndex = topics[topicName].second[clientId];

    // check if there are new messages
    if (nextReadMsgIndex >= topics[topicName].first.size()) {
        string err = "No new messages for the client";
        throw err;
    }

    // get the new message
    string msg = topics[topicName].first[nextReadMsgIndex];

    // TODO this should be done after receiving ACK from the client
    // update the client's next read message index
    // topics[topicName].second[clientId] = nextReadMsgIndex + 1;

    return msg;
}


void TopicManager::createTopic(string topicName) {
    if (topics.find(topicName) == topics.end()) {
        topics[topicName] = make_pair(deque<string>(), map<string, uint>());
    }
}
void TopicManager::deleteTopic(string topicName) {
    if (topics.find(topicName) != topics.end()) {
        topics.erase(topicName);
    }
}
void TopicManager::addTopicMsg(string topicName, string msg) {
    if (topics.find(topicName) != topics.end()) {
        topics[topicName].first.push_back(msg);
    }
}
void TopicManager::removeTopicMsg(string topicName) {
    if (topics.find(topicName) != topics.end()) {
        topics[topicName].first.pop_front();
    }
}




