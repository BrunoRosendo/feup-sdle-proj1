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
        topics[topicName].second[clientId] = make_pair("", 0);
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

void TopicManager::handlePut(string topicName, string messageId, string message) {
    // check if the topic exists
    if (topics.find(topicName) == topics.end()) {
        string err = "Topic does not exist";
        throw err;
    }

    // add the message to the topic's queue
    addTopicMsg(topicName, messageId, message);
}
string TopicManager::handleGet(string topicName, string clientId, string messageId) {
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
    uint nextReadMsgIndex = topics[topicName].second[clientId].second;
    string prevMsgId = topics[topicName].second[clientId].first;

    if (messageId != prevMsgId && prevMsgId != "") {
        nextReadMsgIndex++;
    }

    cout << "nextReadMsgIndex: " << nextReadMsgIndex << endl;
    // check if there are new messages
    if (nextReadMsgIndex >= topics[topicName].first.size()) {
        string err = "No new messages for the client";
        throw err;
    }

    // update the client's next read message index
    topics[topicName].second[clientId].second = nextReadMsgIndex;
    topics[topicName].second[clientId].first = messageId;

    // get the new message
    string msg = topics[topicName].first[nextReadMsgIndex].second;

    return msg;
}


void TopicManager::createTopic(string topicName) {
    if (topics.find(topicName) == topics.end()) {
        topics[topicName] = make_pair(msgs(), subscribers());
        messagesIds[topicName] = set<string>();
    }
}
void TopicManager::deleteTopic(string topicName) {
    if (topics.find(topicName) != topics.end()) {
        topics.erase(topicName);
        messagesIds.erase(topicName);
    }
}
void TopicManager::addTopicMsg(string topicName, string messageId, string msg) {
    if (topics.find(topicName) != topics.end() && messagesIds[topicName].find(messageId) == messagesIds[topicName].end()) {
        topics[topicName].first.push_back(make_pair(messageId, msg));
        messagesIds[topicName].insert(messageId);
    }
}
void TopicManager::removeTopicMsg(string topicName) {
    if (topics.find(topicName) != topics.end()) {
        string messageId = topics[topicName].first.front().first;
        topics[topicName].first.pop_front();
        messagesIds[topicName].erase(messageId);
    }
}




