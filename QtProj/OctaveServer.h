#ifndef OCTAVESERVER_H
#define OCTAVESERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class octave_server: public QObject
{
    Q_OBJECT
public:
    explicit octave_server(quint16 port, QObject *parent = nullptr);
    ~octave_server();
Q_SIGNALS:
    void closed();

public slots:
    void octave_server_newconnection();
    void octave_server_send_data();

private:
    QTcpServer tcp_server;
    QTcpSocket *tcp_socket;

public:
    int client_num = 0;
    QString message_send = "";
};

#endif
