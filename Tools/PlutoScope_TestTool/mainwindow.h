#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QNetworkInterface>
#include <QLineSeries>
#include <QChart>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Hz_button_clicked();

    void on_kHz_button_clicked();

    void on_MHz_button_clicked();

    void on_GHz_button_clicked();

    void refresh_info();

    void reget_info();

    void rx_refresh();

    void on_start_button_clicked();

    void on_stop_button_clicked();

    void mouseMoveEvent(QMouseEvent* event);

private:
    Ui::MainWindow *ui;
    void error_catch(int code);
};
#endif // MAINWINDOW_H
