#include <iostream>

class TopicMessage {
    private:
        std::string name;
        std::string content;
    public:
        TopicMessage(std::string name, std::string content);
        std::string getName();
        std::string getContent();
        void setContent(std::string content);
};