#include "TopicMessage.hpp"

using namespace std;


TopicMessage::TopicMessage(string name, string content) {
    this->name = name;
    this->content = content;
}
string TopicMessage::getContent() {
    return content;
}
string TopicMessage::getName() {
    return name;
}
void TopicMessage::setContent(string content) {
    this->content = content;
}
