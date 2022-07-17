#include "OctaveServer.h"

#include <QHostAddress>
#include <QAbstractSocket>

octave_server::octave_server(quint16 port, QObject *parent) :
    tcp_server(this)
{
    tcp_server.listen(QHostAddress::LocalHost, port);
    connect(&tcp_server, &QTcpServer::newConnection, this, &octave_server::octave_server_newconnection);
    connect(&tcp_server, &QTcpServer::destroyed, this, &octave_server::closed);
    client_num = 0;
}

octave_server::~octave_server()
{
    tcp_server.close();
}

void octave_server::octave_server_newconnection()
{
    tcp_socket = tcp_server.nextPendingConnection();
    connect(tcp_socket, &QTcpSocket::disconnected,
                tcp_socket, &QTcpSocket::deleteLater);
    connect(tcp_socket, &QIODevice::readyRead,
                this, &octave_server::octave_server_send_data);
    ++client_num;
}

void octave_server::octave_server_send_data()
{
    tcp_socket->readAll();
    tcp_socket->write(message_send.toStdString().c_str());
}
