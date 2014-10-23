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

#include "socketclient.h"

SocketClient::SocketClient(QObject* parent)
    : QObject(parent),
    mNotif(NULL),
    mCallback(NULL)
{
  client.setReadBufferSize(512*1024);
  connect(&client, SIGNAL(connected()),
    this, SLOT(startTransfer()));

}

SocketClient::~SocketClient()
{
  delete mNotif;
  client.close();
}

void SocketClient::waitForMessages(IMessageReceived* callback)
{
    mCallback = callback;
    mNotif = new QSocketNotifier(client.socketDescriptor(), QSocketNotifier::Read, this);
    QObject::connect(mNotif, SIGNAL(activated(int)), this, SLOT(received()));
    mNotif->setEnabled(true);
}

QString SocketClient::sendCommandSizedReturn(QString& address, quint16 port, QString command)
{
    QString resp;
    QHostAddress addr(address);
    client.connectToHost(addr, port);
    bool conn = client.waitForConnected();
    mCommand = command;
    if (conn) {
        client.write(command.toUtf8().data(), command.length());
        client.flush();
        client.waitForReadyRead();
        QString r1 = client.readLine(32);
        int len = r1.toInt();
        char* buf = new char[len];
        char* ptr = buf;

        int read = -1;
        while (len > 0 && read !=0) {
            client.waitForReadyRead();
            read = client.read(ptr, len);

            len -= read;
            ptr += read;
        }
        resp = QString(buf);
        delete [] buf;
    }
    return resp;

}

std::string SocketClient::readSizedString()
{
    while (!client.canReadLine()) {
        client.waitForReadyRead(-1);
    }
    QString r1 = client.readLine();
    QStringList strs = r1.split(" ");
    int frameNum = strs[1].toInt();
    int len = strs[2].toInt();
    if (len>0)
    {
        char* buf = new char[len];
        char* ptr = buf;

        int read = -1;
        while (len > 0 && read !=0) {
            client.waitForReadyRead(-1);
            read = client.read(ptr, len);

            len -= read;
            ptr += read;
        }
        std::string resp = std::string(buf);
        delete [] buf;
        return resp;
    }
    return "";
}

void SocketClient::sendCommand(QString& address, quint16 port, QString command)
{
    QString resp;
    QHostAddress addr(address);
    client.connectToHost(addr, port);
    bool conn = client.waitForConnected();
    mCommand = command;
    if (conn) {
        client.write(command.toUtf8().data(), command.length());
        client.flush();
    }
}

void SocketClient::startTransfer()
{
}

void SocketClient::received()
{
    client.waitForReadyRead(1);
    while (client.bytesAvailable()>0)
    {
        QString r1 = client.readLine();
        int len = r1.toInt();
        char* buf = new char[len];
        char* ptr = buf;

        int read = -1;
        while (len > 0 && read !=0) {
            client.waitForReadyRead(1);
            read = client.read(ptr, len);

            len -= read;
            ptr += read;
        }
        std::string resp = std::string(buf);
        delete [] buf;
        if (mCallback != NULL)
        {
//            qDebug() << QString(resp.c_str());//"received";
            mCallback->MessageReceived(resp);
        }
        client.waitForReadyRead(1);
    }
}

