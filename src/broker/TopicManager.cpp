#include "TopicManager.h"

#include <climits>


TopicManager::TopicManager() {
    ifstream fin(DATA_FILE);
    if (!fin.is_open()) return;
    fin >> *this;
    fin.close();
}

void TopicManager::createTopic(string topicName) {
    if (topics.find(topicName) == topics.end()) {
        topics[topicName] = make_pair(msgs(), subscribers());
        messagesIds[topicName] = set<string>();
        lastMessages[topicName] = map<string, pair<string, string>>();
    }
}
void TopicManager::deleteTopic(string topicName) {
    if (topics.find(topicName) != topics.end()) {
        topics.erase(topicName);
        messagesIds.erase(topicName);
        lastMessages.erase(topicName);
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

void TopicManager::handleSubscription(string topicName, string clientId, string messageId) {
    // check if the topic exists
    if (topics.find(topicName) == topics.end()) {
        // if not, create it
        createTopic(topicName);
    }

    // add the client to the topic's subscribers
    if (topics[topicName].second.find(clientId) == topics[topicName].second.end()) {
        topics[topicName].second[clientId] = make_pair("_", topics[topicName].first.size());
        if (lastMessages[topicName].find(clientId) == lastMessages[topicName].end()) {
            lastMessages[topicName][clientId] = make_pair("_", "_");
        }
        lastMessages[topicName][clientId].first = messageId;
    }
    else if (messageId == lastMessages[topicName][clientId].first) {
        // if the client is already subscribed to the topic and the messageId is the same as the lastSubMsgId, do nothing
        return;
    }
    else {
        string err = "Client already subscribed to topic";
        throw err;
    }
}
void TopicManager::handleUnsubscription(string topicName, string clientId, string messageId) {
    // check if the topic exists
    if (topics.find(topicName) == topics.end()) {
        string err = "Topic does not exist";
        throw err;
    }

    // remove the client from the topic's subscribers
    if (topics[topicName].second.find(clientId) != topics[topicName].second.end()) {
        topics[topicName].second.erase(clientId);
        if (lastMessages[topicName].find(clientId) == lastMessages[topicName].end()) {
            lastMessages[topicName][clientId] = make_pair("_", "_");
        }
        lastMessages[topicName][clientId].second = messageId;
    }
    else if (messageId == lastMessages[topicName][clientId].second) {
        // if the client is already unsubscribed from the topic and the messageId is the same as the lastUnsubMsgId, do nothing
        return;
    }
    else {
        string err = "Client not subscribed to topic";
        throw err;
    }

    garbageCollectTopics(topicName);
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
    uint lastReadMsgIndex = topics[topicName].second[clientId].second;
    string prevMsgId = topics[topicName].second[clientId].first;

    if (messageId != prevMsgId && prevMsgId != "_") {
        lastReadMsgIndex++;
    }

    // check if there are new messages
    if (lastReadMsgIndex >= topics[topicName].first.size()) {
        string err = "No new messages for the client";
        throw err;
    }

    // update the client's next read message index
    topics[topicName].second[clientId].second = lastReadMsgIndex;
    topics[topicName].second[clientId].first = messageId;

    // get the new message
    string msg = topics[topicName].first[lastReadMsgIndex].second;
    
    garbageCollectMessages(topicName);

    return msg;
}

void TopicManager::garbageCollectMessages(string topicName) {
    uint min = UINT_MAX;
    if (topics.find(topicName) != topics.end()) {
        for (auto& client : topics[topicName].second) {
            if (client.second.second < min) {
                min = client.second.second;
            }
        }

        for (uint i = 0; i < min; i++) {
            removeTopicMsg(topicName);
        }

        for (auto& client : topics[topicName].second) {
            client.second.second -= min;
        }
    }
}
void TopicManager::garbageCollectTopics(string topicName) {
    if (topics.find(topicName) != topics.end()) {
        if (topics[topicName].second.empty()) {
            deleteTopic(topicName);
        }
    }
}

void TopicManager::serialize() {
    if (!filesystem::is_directory(DATA_PATH) || !filesystem::exists(DATA_PATH))
        filesystem::create_directories(DATA_PATH);

    ofstream fout(DATA_FILE);
    fout << *this;
    fout.close();
}

// Serialize TopicManager fields to os
ostream& operator<< (ostream& os, TopicManager& tm) {
    os << tm.topics.size() << endl;
    for (auto& topic : tm.topics) {
        os << topic.first << endl;
        os << topic.second.first.size() << endl;
        for (auto& msg : topic.second.first) {
            os << msg.first << endl;
            os << msg.second << endl;
        }
        os << topic.second.second.size() << endl;
        for (auto& subscriber : topic.second.second) {
            os << subscriber.first << endl;
            os << subscriber.second.first << endl;
            os << subscriber.second.second << endl;
        }
    }

    os << tm.messagesIds.size() << endl;
    for (auto& topic : tm.messagesIds) {
        os << topic.first << endl;
        os << topic.second.size() << endl;
        for (auto& msgId : topic.second) {
            os << msgId << endl;
        }
    }

    os << tm.lastMessages.size() << endl;
    for (auto& topic : tm.lastMessages) {
        os << topic.first << endl;
        os << topic.second.size() << endl;
        for (auto& client : topic.second) {
            os << client.first << endl;
            os << client.second.first << endl;
            os << client.second.second << endl;
        }
    }

    return os;
}

// Deserialize TopicManager fields from is
istream& operator>> (istream& is, TopicManager& tm) {
    
    uint topicsSize;
    is >> topicsSize;
    is.ignore(99999, '\n');

    for (uint i = 0; i < topicsSize; i++) {
        string topicName;
        getline(is, topicName);

        tm.topics[topicName] = make_pair(msgs(), subscribers());

        uint msgsSize;
        is >> msgsSize;
        is.ignore(99999, '\n');

        for (uint j = 0; j < msgsSize; j++) {
            string msgId, msg;
            getline(is, msgId);

            getline(is, msg);

            tm.topics[topicName].first.push_back(make_pair(msgId, msg));
        }

        uint subscribersSize;
        is >> subscribersSize;
        is.ignore(99999, '\n');

        for (uint j = 0; j < subscribersSize; j++) {
            string clientId, msgId;
            uint lastReadMsgIndex;
            getline(is, clientId);

            getline(is, msgId);

            is >> lastReadMsgIndex;
            is.ignore(99999, '\n');

            tm.topics[topicName].second[clientId] = make_pair(msgId, lastReadMsgIndex);
        }
    }


    uint messagesIdsSize;
    is >> messagesIdsSize;
    is.ignore(99999, '\n');

    for (uint i = 0; i < messagesIdsSize; i++) {
        string topicName;
        getline(is, topicName);

        tm.messagesIds[topicName] = set<string>();

        uint msgIdsSize;
        is >> msgIdsSize;
        is.ignore(99999, '\n');

        for (uint j = 0; j < msgIdsSize; j++) {
            string msgId;
            getline(is, msgId);
            tm.messagesIds[topicName].insert(msgId);
        }
    }

    uint lastMessagesSize;
    is >> lastMessagesSize;
    is.ignore(99999, '\n');

    for (uint i = 0; i < lastMessagesSize; i++) {
        string topicName;
        getline(is, topicName);

        tm.lastMessages[topicName] = map<string, pair<string, string>>();

        uint clientsSize;
        is >> clientsSize;
        is.ignore(99999, '\n');

        for (uint j = 0; j < clientsSize; j++) {
            string clientId, lastSubMsgId, lastUnsubMsgId;
            getline(is, clientId);

            getline(is, lastSubMsgId);

            getline(is, lastUnsubMsgId);

            tm.lastMessages[topicName][clientId] = make_pair(lastSubMsgId, lastUnsubMsgId);
        }
    }

    return is;
}
