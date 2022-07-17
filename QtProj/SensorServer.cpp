#include "SensorServer.h"
#include <QJsonDocument>
#include <QJsonObject>

QT_USE_NAMESPACE

sensor_server::sensor_server(quint16 port, sensor_struct_orie *orien_struct, sensor_struct_coor *accel_struct,sensor_struct_coor *magne_struct, QObject *parent) :
    QObject(parent),
    ws_server(new QWebSocketServer(QStringLiteral("Sensor Server"), QWebSocketServer::NonSecureMode, this))
{
    sensor_server_orie_struct_ptr = orien_struct;
    sensor_server_acce_struct_ptr = accel_struct;
    sensor_server_magn_struct_ptr = magne_struct;
    sensor_server_acce_struct_ptr->timestamp = 0;
    sensor_server_acce_struct_ptr->x = 0;
    sensor_server_acce_struct_ptr->y = 0;
    sensor_server_acce_struct_ptr->z = 0;
    sensor_server_magn_struct_ptr->timestamp = 0;
    sensor_server_magn_struct_ptr->x = 0;
    sensor_server_magn_struct_ptr->y = 0;
    sensor_server_magn_struct_ptr->z = 0;
    sensor_server_orie_struct_ptr->timestamp = 0;
    sensor_server_orie_struct_ptr->azimuth = 0;
    sensor_server_orie_struct_ptr->pitch = 0;
    sensor_server_orie_struct_ptr->roll = 0;

    if (ws_server->listen(QHostAddress::Any, port)) {
        connect(ws_server, &QWebSocketServer::newConnection,
                this, &sensor_server::sensor_server_newconnect);
        connect(ws_server, &QWebSocketServer::closed, this, &sensor_server::closed);
    }
}

sensor_server::~sensor_server()
{
    ws_server->close();
    qDeleteAll(ws_sensors.begin(), ws_sensors.end());
}

void sensor_server::sensor_server_newconnect()
{
    QWebSocket *pSocket = ws_server->nextPendingConnection();
    connect(pSocket, &QWebSocket::textMessageReceived, this, &sensor_server::sensor_server_newdata);
    connect(pSocket, &QWebSocket::disconnected, this, &sensor_server::sensor_server_disconnected);
    ws_sensors << pSocket;
}

void sensor_server::sensor_server_newdata(QString message)
{
    QJsonParseError json_err;
    QJsonDocument  sensor_raw = QJsonDocument::fromJson(message.toUtf8(), &json_err);
    if(json_err.error != QJsonParseError::NoError) return;
    else
    {
        QJsonObject sensor_info = sensor_raw.object();
        QString sensor_name = sensor_info.value("SensorName").toString();
        qint64 sensor_timestamp = sensor_info.value("Timestamp").toString().toLongLong();
        if(sensor_name == "Accelerometer")
        {
            sensor_server_acce_struct_ptr->timestamp = sensor_timestamp;
            sensor_server_acce_struct_ptr->x = sensor_info.value("x").toString().toDouble();
            sensor_server_acce_struct_ptr->y = sensor_info.value("y").toString().toDouble();
            sensor_server_acce_struct_ptr->z = sensor_info.value("z").toString().toDouble();
        }
        else if(sensor_name == "Magnetometer")
        {
            sensor_server_magn_struct_ptr->timestamp = sensor_timestamp;
            sensor_server_magn_struct_ptr->x = sensor_info.value("x").toString().toDouble();
            sensor_server_magn_struct_ptr->y = sensor_info.value("y").toString().toDouble();
            sensor_server_magn_struct_ptr->z = sensor_info.value("z").toString().toDouble();
        }
        else if(sensor_name == "Orientation")
        {
            sensor_server_orie_struct_ptr->timestamp = sensor_timestamp;
            sensor_server_orie_struct_ptr->azimuth = sensor_info.value("azimuth").toString().toDouble();
            sensor_server_orie_struct_ptr->pitch = sensor_info.value("pitch").toString().toDouble();
            sensor_server_orie_struct_ptr->roll = sensor_info.value("roll").toString().toDouble();
        }
        else return;
    }
    return;
}

void sensor_server::sensor_server_disconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        ws_sensors.removeAll(pClient);
        pClient->deleteLater();
    }
}
