#include <iostream>
#include <vector>
#include "TopicMessage.hpp"

class TopicManager {
    private:
        std::vector<TopicMessage> topics;
    public:
        TopicManager();
        void addTopic(TopicMessage topic);
        void removeTopic();
        TopicMessage getTopic(int index);
        TopicMessage getTopic(std::string topicName);
        std::vector<TopicMessage> getTopics();
};