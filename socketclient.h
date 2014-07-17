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

class SocketClient : public QObject
{
Q_OBJECT
public:
    SocketClient(QObject* parent = 0);
    QString sendCommandSizedReturn(QString &address, quint16 port, QString command);
    ~SocketClient();
    void sendCommand(QString &address, quint16 port, QString command);
public slots:
    void startTransfer();
private:
    QString mCommand;
    QTcpSocket client;
};

#endif // SOCKETCLIENT_H
