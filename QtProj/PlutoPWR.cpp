#include <QtCore/QCoreApplication>
#include <qtconcurrentrun.h>
#include <QTcpServer>
#include <QNetworkInterface>
#include <QCommandLineParser>
#include <QDebug>
#include <QTimer>
#include <math.h>
#include <stdio.h>
#include "SensorServer.h"
#include "PlutoDevice.h"
#include "OctaveServer.h"
pluto_sdr pluto;

char *pluto_uri = (char *)"ip:192.168.2.1";
char *output_file_name = (char *)"./PlutoPWR_Output.txt";
quint16 sensor_server_port = 5000;
quint16 octave_server_port = 5001;
long long pluto_samprate = pluto.pluto_MHz(2.40);
long long pluto_freq = pluto.pluto_MHz(2147.483647);
long long pluto_bandwidth = pluto.pluto_kHz(300);
size_t pluto_buffer_size = 4096;
QString pluto_net = "192.168.2.";

void sensor_run();
void pluto_run();
void octave_run();
void progress();

FILE *output_file = NULL;
sensor_struct_orie sensor_orien;
sensor_struct_coor sensor_accel;
sensor_struct_coor sensor_magne;
double pluto_power;
double lst_coor_phi = 90;
double lst_coor_theta = 0;
double coor_phi;        // Angle with up;
double coor_theta;      // Angle with north;
octave_server *oct_server;
sensor_server *sen_server;
QTcpSocket *oct_socket = nullptr;

bool program_broken = false;

using namespace QtConcurrent;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("ADALM-Pluto Powerscope: Umaru Aya");
    parser.addHelpOption();

    QCommandLineOption plt_uri_opt(QStringList() << "u" << "pluto_uri",
                               QCoreApplication::translate("main", "URI for Pluto IIO [default: ip:192.168.2.1]."),
                               QCoreApplication::translate("main", "pluto_uri"), QLatin1String("ip:192.168.2.1"));
    parser.addOption(plt_uri_opt);
    QCommandLineOption output_file_opt(QStringList() << "o" << "output_file",
                               QCoreApplication::translate("main", "Output file [default: ./PlutoPWR_Output.txt]."),
                               QCoreApplication::translate("main", "output_file"), QLatin1String("./PlutoPWR_Output.txt"));
    parser.addOption(output_file_opt);
    QCommandLineOption sensor_port_opt(QStringList() << "s" << "sensor_port",
                               QCoreApplication::translate("main", "Sensor server port [default: 5000]."),
                               QCoreApplication::translate("main", "sensor_port"), QLatin1String("5000"));
    parser.addOption(sensor_port_opt);
    QCommandLineOption octave_port_opt(QStringList() << "O" << "octave_port",
                               QCoreApplication::translate("main", "Octave server port [default: 5001]."),
                               QCoreApplication::translate("main", "octave_port"), QLatin1String("5001"));
    parser.addOption(octave_port_opt);
    QCommandLineOption samprate_opt(QStringList() << "S" << "pluto_samprate",
                               QCoreApplication::translate("main", "Pluto sampling rate in [2.083334, 61.439999]MHz [default: 2.40]."),
                               QCoreApplication::translate("main", "pluto_samprate"), QLatin1String("2.40"));
    parser.addOption(samprate_opt);
    QCommandLineOption freq_opt(QStringList() << "f" << "pluto_freq",
                               QCoreApplication::translate("main", "Pluto frequency in [70.000001, 5999.999999]MHz [default: 2147.483647]."),
                               QCoreApplication::translate("main", "pluto_freq"), QLatin1String("2147.483647"));
    parser.addOption(freq_opt);
    QCommandLineOption bandwidth_opt(QStringList() << "b" << "pluto_bandwidth",
                               QCoreApplication::translate("main", "Pluto bandwidth in [0.200001, 55.999999]MHz [default: 0.26]."),
                               QCoreApplication::translate("main", "pluto_bandwidth"), QLatin1String("0.26"));
    parser.addOption(bandwidth_opt);
    QCommandLineOption buffersize_opt(QStringList() << "B" << "pluto_buffersize",
                               QCoreApplication::translate("main", "Pluto buffer size [default: 4096]."),
                               QCoreApplication::translate("main", "pluto_buffersize"), QLatin1String("4096"));
    parser.addOption(buffersize_opt);

    parser.process(a);
    pluto_uri = (char *)parser.value(plt_uri_opt).toStdString().c_str();
    output_file_name = (char *)parser.value(output_file_opt).toStdString().c_str();
    sensor_server_port = parser.value(sensor_port_opt).toInt();
    octave_server_port = parser.value(octave_port_opt).toInt();
    pluto_samprate = pluto.pluto_MHz(parser.value(samprate_opt).toDouble());
    pluto_freq = pluto.pluto_MHz(parser.value(freq_opt).toDouble());
    pluto_bandwidth = pluto.pluto_MHz(parser.value(bandwidth_opt).toDouble());
    pluto_buffer_size = parser.value(buffersize_opt).toInt();

    QString local_addr = "127.0.0.1";
    QFuture<void> octave_thread = run(octave_run);
    qDebug("You can check options by : PlutoPWR --help");
    qDebug("Please set Octave client connect to localhost port: %d", octave_server_port);
    while(oct_server->client_num == 0);
    qDebug("Octave client connected.");
    QList<QHostAddress> addr_list = QNetworkInterface::allAddresses();
    QString pluto_net = "";
    QString pluto_uri_char = pluto_uri;
    QList<QString> pluto_net_splited;
    pluto_net_splited = pluto_uri_char.split(":");
    pluto_uri_char = pluto_net_splited[1];
    pluto_net_splited = pluto_uri_char.split(".");
    pluto_net.append(pluto_net_splited[0]); pluto_net.append(".");
    pluto_net.append(pluto_net_splited[1]); pluto_net.append(".");
    pluto_net.append(pluto_net_splited[2]); pluto_net.append(".");
    foreach(QHostAddress addr, addr_list)
    {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
        {
            if(addr.toString().contains("127.0.")) continue;
            if(addr.toString().contains(pluto_net)) continue;
            local_addr = addr.toString();
        }
    }
    if(local_addr == "127.0.0.1")
    {
        qDebug("Can not found network interface.");
        return 1;
    }
    QFuture<void> sensor_thread = run(sensor_run);
    qDebug("Please set Sensor Stream port to: %s:%d", local_addr.toStdString().c_str(), sensor_server_port);
    QFuture<void> pluto_thread = run(pluto_run);

    output_file = fopen(output_file_name, "w");
    if(output_file == NULL)
    {
        qDebug("Can not open output file.");
        return 1;
    }
    fprintf(output_file, "Frequency\tPower\tTheta\tPhi\n");

    pluto_thread.waitForFinished();
    qDebug("Pluto disconnected.");
    sen_server->destroyed();
    delete sen_server;
    sensor_thread.waitForFinished();
    qDebug("Sensor server offline.");
    oct_server->destroyed();
    delete oct_server;
    octave_thread.waitForFinished();
    qDebug("Octave server offline.");
    qDebug("System exit.");

    return 0;
}

void progress()
{
    double theta, phi;
    QString output_line;

    theta = sensor_orien.azimuth;
    if(sensor_orien.pitch <= 90.0 && sensor_orien.pitch >= 0.0) phi = sensor_orien.pitch + 90.0;
    else if(sensor_orien.pitch >= 270.0 && sensor_orien.pitch <= 360.0) phi = sensor_orien.pitch - 270.0;
    else return;
    coor_theta = theta;
    coor_phi = phi;

    if(coor_theta == lst_coor_theta && coor_phi == lst_coor_phi) return;
    output_line = "";
    output_line.append(QString::number(pluto_freq));
    output_line.append("\t");
    output_line.append(QString::number(pluto_power));
    output_line.append("\t");
    output_line.append(QString::number(coor_theta));
    output_line.append("\t");
    output_line.append(QString::number(coor_phi));
    output_line.append("\n");
    oct_server->message_send=output_line;
    fprintf(output_file, "%s", output_line.toStdString().c_str());
    qDebug("%s", (char *)output_line.toStdString().c_str());
    lst_coor_theta = theta;
    lst_coor_phi = phi;
}

void pluto_run()
{
    void *point;
    pluto.pluto_init(pluto_uri);
        printf("%s/n", pluto.pluto_message.c_str());
    pluto.pluto_set_rx_samprate(pluto_samprate);
        printf("%s/n", pluto.pluto_message.c_str());
    pluto.pluto_set_rx_freq(pluto_freq);
        printf("%s/n", pluto.pluto_message.c_str());
    pluto.pluto_set_rx_bandwidth(pluto_bandwidth);
        printf("%s/n", pluto.pluto_message.c_str());
    pluto.pluto_set_tx_en(true);
        printf("%s/n", pluto.pluto_message.c_str());
    pluto.pluto_init_receiver(pluto_buffer_size);
        printf("%s/n", pluto.pluto_message.c_str());

    long long pluto_gain;
    long long continue_flag = 0;
    while (true) {
        continue_flag += pluto.pluto_get_rx_gain(&pluto_gain); if(continue_flag != 0) break;
        continue_flag += pluto.pluto_set_rx_samprate(pluto_samprate); if(continue_flag != 0) break;
        continue_flag += pluto.pluto_set_rx_freq(pluto_freq); if(continue_flag != 0) break;
        continue_flag += pluto.pluto_set_rx_bandwidth(pluto_bandwidth); if(continue_flag != 0) break;
        continue_flag += pluto.pluto_get_rx_data(); if(continue_flag != 0) break;
        double sum = 0;
        long long cnt = 0;
        for (point = pluto.pluto_receiver.rx_ptr_start; point < pluto.pluto_receiver.rx_ptr_end; point = (void *)((ptrdiff_t)point + pluto.pluto_receiver.rx_ptr_inc))
        {
            const int16_t i = ((int16_t*)point)[0];
            const int16_t q = ((int16_t*)point)[1];
            sum += sqrt(pow(abs(i), 2) + pow(abs(q), 2));
            ++cnt;
        }
        pluto_power = (sum / cnt) * pow(10.0, (70.0 - pluto_gain) / 20.0);
        progress();
        if(program_broken) break;
    }
    program_broken = true;
    if(continue_flag != 0) printf("%s/n", pluto.pluto_message.c_str());
    pluto.pluto_destroy();
        printf("%s/n", pluto.pluto_message.c_str());
    oct_server->disconnect();
    fclose(output_file);
}

void sensor_run()
{
    int argc;
    char **argv = nullptr;
    sen_server = new sensor_server(sensor_server_port, &sensor_orien, &sensor_accel, &sensor_magne);
    QCoreApplication a(argc, argv);
    QObject::connect(sen_server, &sensor_server::destroyed, &a, &QCoreApplication::quit);
    a.exec();
    program_broken = true;
}

void octave_run()
{
    int argc;
    char **argv = nullptr;
    oct_server = new octave_server(octave_server_port);
    QCoreApplication a(argc, argv);
    QObject::connect(oct_server, &octave_server::destroyed, &a, &QCoreApplication::quit);
    a.exec();
    program_broken = true;
}
