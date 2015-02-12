#ifndef MESSAGERECEIVED_H
#define MESSAGERECEIVED_H

class IMessageReceived
{
public:
    virtual void MessageReceived(QString& recv) = 0;
};

#endif // MESSAGERECEIVED_H
