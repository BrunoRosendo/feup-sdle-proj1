#include "TopicManager.hpp"

using namespace std;

TopicManager::TopicManager() {
    topics = vector<TopicMessage>();
}
void TopicManager::addTopic(TopicMessage topic) {
    topics.push_back(topic);
}
void TopicManager::removeTopic() {
    topics.erase(topics.begin());
}
TopicMessage TopicManager::getTopic(int index) {
    return topics[index];
}
TopicMessage TopicManager::getTopic(string topicName) {
    for (int i = 0; i < topics.size(); i++) {
        if (topics[i].getName() == topicName) {
            return topics[i];
        }
    }
    return TopicMessage("","");
}
vector<TopicMessage> TopicManager::getTopics() {
    return topics;
}



