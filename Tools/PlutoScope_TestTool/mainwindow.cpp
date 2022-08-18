#include <fftw3.h>
#include "libpluto.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "flags.h"
#include "functions.h"

QTimer *refresh_info_timer;
QTimer *reget_info_timer;
QTimer *rx_timer;

fftw_complex *in, *out;
fftw_plan fft_plan;

int pos_x_fft, pos_y_fft, pos_xm_fft, pos_ym_fft;
long long freq_start, freq_end, freq_inc;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->fft_view->chart()->setGeometry(ui->fft_view->geometry());
    pos_x_fft = ui->fft_view->pos().x() + 30;
    pos_y_fft = ui->fft_view->pos().y();
    pos_xm_fft = ui->fft_view->pos().x() + ui->fft_view->width() - 30;
    pos_ym_fft = ui->fft_view->pos().y() + ui->fft_view->height();

    QString filename = ":/logo/logo.jpg";
    QImage image(filename);
    image = image.scaled(ui->logo_label->width(), ui->logo_label->height());
    ui->logo_label->setPixmap(QPixmap::fromImage(image));

    refresh_info_timer = new QTimer(this);
    connect(refresh_info_timer, &QTimer::timeout, this, &MainWindow::refresh_info);
    refresh_info_timer->start(refresh_info_time);

    reget_info_timer = new QTimer(this);
    connect(reget_info_timer, &QTimer::timeout, this, &MainWindow::reget_info);
    reget_info_timer->start(reget_info_time);

    rx_timer = new QTimer(this);
    connect(rx_timer, &QTimer::timeout, this, &MainWindow::rx_refresh);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::rx_refresh()
{
    void *point;
    double gain;
    size_t index = 0;
    device_busy = true;
    pluto_get_rx_data();
    if(!agc_set)
    {
        gain = (double)pluto_get_rx_gain();
        gain = exp(gain / 10.0);
    }
    else gain = 1.0;
    for(point = pluto_receiver.rx_ptr_start; point < pluto_receiver.rx_ptr_end; point = (void *)((ptrdiff_t)point + pluto_receiver.rx_ptr_inc))
    {
        const int16_t i = ((int16_t*)point)[0];
        const int16_t q = ((int16_t*)point)[1];
        const double i_d = i / gain;
        const double q_d = q / gain;
        in[index][0] = i_d;
        in[index][1] = q_d;
        ++index;
    }
    fftw_execute(fft_plan);
    long long  freq_point = freq_start;
    QLineSeries *line_s = new QLineSeries();
    for(index =0; index < buffersize_set; ++index)
    {
        const double i_f = out[index][0];
        const double q_f = out[index][1];
        double abs_f = (i_f * i_f) + (q_f * q_f);
        abs_f = sqrt(abs_f);
        line_s->append(functions_freq_long_to_double(freq_point), abs_f);
        freq_point += freq_inc;
    }
    line_s->removePoints(0,1);
    ui->fft_view->chart()->legend()->hide();
    ui->fft_view->chart()->removeAllSeries();
    ui->fft_view->chart()->addSeries(line_s);

    device_busy = false;
}

void MainWindow::on_stop_button_clicked()
{
    rx_timer->stop();
    device_busy = true;
    fftw_destroy_plan(fft_plan);
    fftw_free(in);
    fftw_free(out);
    start_status = false;
    pluto_set_rx_en(false);
    pluto_destroy();
    status_message = "Stoped.";
}

void MainWindow::error_catch(int code)
{
    if(code != 0)
    {
        device_busy = true;
        start_status = false;
        pluto_set_rx_en(false);
        rx_timer->stop();
        fftw_destroy_plan(fft_plan);
        fftw_free(in);
        fftw_free(out);
        pluto_destroy();
    }
}

void MainWindow::on_start_button_clicked()
{
    device_busy = true;
    iio_uri_set = ui->iiouri_edit->text();
    buffersize_set = ui->buffersize_edit->text().toLongLong();
    double freq_pre_set = ui->freq_edit->text().toDouble();
    freq_unit_set = freq_unit_flag;
    for(unsigned int index = 0; index < freq_unit_set; ++index) freq_pre_set *= 1000;
    samprate_set = ui->samprate_edit->text().toDouble() * 1000000;
    bandwidth_set = ui->bandwidth_edit->text().toDouble() * 1000000;
    freq_set = freq_pre_set - (samprate_set / 2);
    agc_set = ui->agc_check->isChecked();
    int rt_code = 0;
    if(!start_status)
    {
        if(pluto_init((char *)iio_uri_set.toStdString().c_str()) != 0) {status_message = "Init failed."; rt_code = 1;}
        if(pluto_init_sensors() != 0) {status_message = "Sensors init failed."; rt_code = 1;}
        if(pluto_init_receiver(buffersize_set) != 0) {status_message = "Receiver init failed."; rt_code = 1;}
        in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * buffersize_set);
        out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * buffersize_set);
        fft_plan = fftw_plan_dft_1d(buffersize_set, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    }
    if(pluto_set_rx_freq(freq_set) != 0) {status_message = "Freq setting failed."; rt_code = 1;}
    if(pluto_set_rx_samprate(samprate_set) != 0) {status_message = "Samprate setting failed."; rt_code = 1;}
    if(pluto_set_rx_bandwidth(bandwidth_set) != 0) {status_message = "Bandwidth setting failed."; rt_code = 1;}
    if(pluto_set_rx_en(true) != 0) {status_message = "Receiver starting failed."; rt_code = 1;}
    if(!rt_code)
    {
        start_status = true;
        device_busy = false;
        status_message = "Initialized.";
        rx_timer->start(rx_time);
    }
    else error_catch(rt_code);
}

void MainWindow::reget_info()
{
    reget_info_timer->stop();
    if(start_status)
        if(!device_busy)
        {
            QList<QHostAddress> addr_list = QNetworkInterface::allAddresses();
            QString pluto_net = "";
            QString pluto_uri_char = iio_uri_set;
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
                    ip_get = addr.toString();
                }
            }
            samprate_get = pluto_get_rx_samprate();
            bandwidth_get = pluto_get_rx_bandwidth();
            freq_get = pluto_get_rx_freq() + (samprate_get / 2);
            reget_info_timer->start(reget_info_time);
            agc_get = agc_set;
        }
        else
            reget_info_timer->start();
   else
        reget_info_timer->start(reget_info_time);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    int pos_x = event->pos().x();
    int pos_y = event->pos().y();
    event->accept();
    if(start_status)
        if(pos_x > pos_x_fft && pos_x < pos_xm_fft && pos_y > pos_y_fft && pos_y < pos_ym_fft)
        {
            pos_x -= pos_x_fft;
            int index = pos_x * buffersize_set / (pos_xm_fft - pos_x_fft);
            long long freq_x = freq_start + (pos_x * (freq_end - freq_start) / (pos_xm_fft - pos_x_fft));
            ui->fftfreq_info->setText(functions_freq_long_to_str(freq_x));
            double i_f = out[index][0];
            double q_f = out[index][1];
            double abs_f = (i_f * i_f) + (q_f * q_f);
            abs_f = sqrt(abs_f);
            abs_f = 10 * log(abs_f) / log(10);
            ui->fftamp_info->setText(QString::number(abs_f).append(" dB"));
        }
}


void MainWindow::refresh_info()
{
    if(start_status)
    {
        freq_start = (freq_get == 0? freq_set : freq_get);
        freq_end = (freq_get == 0? freq_set : freq_get) + ((samprate_get == 0)? samprate_set : samprate_get);
        freq_inc = (freq_end - freq_start) / buffersize_set;
    }
    ui->start_button->setText(start_status? "Set" : "Start");
    ui->iiouri_edit->setEnabled(!start_status);
    ui->buffersize_edit->setEnabled(!start_status);
    ui->is_running->setChecked(start_status);
    ui->freq_info->setText(functions_freq_long_to_str(freq_get));
    ui->samprate_info->setText(functions_freq_long_to_str(samprate_get));
    ui->bandwidth_info->setText(functions_freq_long_to_str(bandwidth_get));
    ui->is_agc->setChecked(agc_get);
    ui->ip_info->setText(ip_get);
    ui->status_edit->setText(status_message);
    ui->stop_button->setEnabled(start_status);
}

void MainWindow::on_Hz_button_clicked()
{
    ui->Hz_button->setChecked(true);
    ui->kHz_button->setChecked(false);
    ui->MHz_button->setChecked(false);
    ui->GHz_button->setChecked(false);
    freq_unit_flag = 0;
}


void MainWindow::on_kHz_button_clicked()
{
    ui->Hz_button->setChecked(false);
    ui->kHz_button->setChecked(true);
    ui->MHz_button->setChecked(false);
    ui->GHz_button->setChecked(false);
    freq_unit_flag = 1;
}


void MainWindow::on_MHz_button_clicked()
{
    ui->Hz_button->setChecked(false);
    ui->kHz_button->setChecked(false);
    ui->MHz_button->setChecked(true);
    ui->GHz_button->setChecked(false);
    freq_unit_flag = 2;
}


void MainWindow::on_GHz_button_clicked()
{
    ui->Hz_button->setChecked(false);
    ui->kHz_button->setChecked(false);
    ui->MHz_button->setChecked(false);
    ui->GHz_button->setChecked(true);
    freq_unit_flag = 3;
}
