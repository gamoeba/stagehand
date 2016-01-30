/*
 * Copyright (c) 2014 Gamoeba Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H


// client.h
#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QTcpSocket>

#include "messagereceived.h"

class SocketClient : public QObject
{
Q_OBJECT
public:
    SocketClient(QObject* parent = 0);
    bool connectSocket(const QString &address, quint16 port);
    bool disconnectSocket();
    QString sendCommandSizedReturn(QString command);
    ~SocketClient();
    void sendCommand(QString command);
    void waitForMessages(IMessageReceived *callback);
    std::string readSizedString();
public slots:
    void startTransfer();
    void received();
private:
    QString mCommand;
    QTcpSocket client;
    QSocketNotifier* mNotif;
    IMessageReceived* mCallback;
    QString mHostAddress;
    quint16 mPort;
};

#endif // SOCKETCLIENT_H
