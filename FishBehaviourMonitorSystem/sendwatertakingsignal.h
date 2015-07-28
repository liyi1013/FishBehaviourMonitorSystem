#ifndef SENDWATERTAKINGSIGNAL_H
#define SENDWATERTAKINGSIGNAL_H

#include<QtWidgets\qwidget.h>
#include<QtSerialPort\qserialportinfo.h>
#include "MasterThread.h"

class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;

//class SendWaterTakingSignal_view;

class SendWaterTakingSignal:public QObject
{
    Q_OBJECT;

public:
    SendWaterTakingSignal();

public:

    void send(int seconds);

    std::vector<QString> get_serial_ports(){
        return this->_serial_ports;
    }

public slots:
    //void set_baud_rate(int baudrate);
    void set_baud_rate(QString baudrate);
    void set_serial_port(QString serial_port);
    void set_waittime(int waitTimeout);

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);
public:
    void emit_response(const QString &s){ emit response(s); }
    void emit_error(const QString &s){ emit error(s); }
    void emit_timeout(const QString &s){ emit timeout(s); }

private:
    MasterThread _thread;

    std::vector<QString> _serial_ports;
    QString _portName;
    QString _request;
    int _waitTimeout;
};


class SendWaterTakingSignal_view :public QWidget
{
    Q_OBJECT;
public:
    SendWaterTakingSignal_view(QWidget *parent = 0, SendWaterTakingSignal *watertaking = 0);

private slots:
    void transaction();
    void showResponse(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);

private:
    void setControlsEnabled(bool enable);

private:

    int transactionCount;

    QLabel    *serialPortLabel;
    QComboBox *serialPortComboBox;

    QLabel   *waitResponseLabel;
    QSpinBox *waitResponseSpinBox;

    QLabel    *secondsLabel;
    QSpinBox *secondsSpinBox;

    QLabel    *baud_rateLabel;
    QLineEdit *baud_rateLineEdit;

    QLabel *trafficLabel;
    QLabel *statusLabel;

    QPushButton *runButton;

    SendWaterTakingSignal *send_water_taking;
};
#endif // SENDWATERTAKINGSIGNAL_H
