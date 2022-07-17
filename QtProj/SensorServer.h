#ifndef SENSORSERVER_H
#define SENSORSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QDebug>
#include <string>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

typedef struct {
    long long timestamp;
    double azimuth, pitch, roll;
}sensor_struct_orie;

typedef struct {
    long long timestamp;
    double x, y, z;
}sensor_struct_coor;

class sensor_server: public QObject
{
Q_OBJECT
public:
    explicit sensor_server(quint16 port, sensor_struct_orie *orien_struct, sensor_struct_coor *accel_struct,sensor_struct_coor *magne_struct, QObject *parent = nullptr);
    ~sensor_server();

Q_SIGNALS:
        void closed();

private Q_SLOTS:
    void sensor_server_newconnect();
    void sensor_server_disconnected();
    void sensor_server_newdata(QString message);

private:
    QWebSocketServer *ws_server;
    QList<QWebSocket *> ws_sensors;
    sensor_struct_orie *sensor_server_orie_struct_ptr;
    sensor_struct_coor *sensor_server_acce_struct_ptr;
    sensor_struct_coor *sensor_server_magn_struct_ptr;

public:
    std::string sensor_server_message;
    int sensor_num = 0;
};

#endif
