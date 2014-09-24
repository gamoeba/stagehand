#ifndef MESSAGERECEIVED_H
#define MESSAGERECEIVED_H

class IMessageReceived
{
public:
    virtual void MessageReceived(std::string recv) = 0;
};

#endif // MESSAGERECEIVED_H
