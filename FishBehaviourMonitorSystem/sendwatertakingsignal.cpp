#include <Qtcore\qdebug.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\QLineEdit>
#include <QtWidgets\QComboBox>
#include <QtWidgets\QSpinBox>
#include <QtWidgets\QPushButton>
#include <QtWidgets\QGridLayout>

#include <QtSerialPort/QSerialPortInfo>
#include "SendWaterTakingSignal.h"

SendWaterTakingSignal::SendWaterTakingSignal() :_waitTimeout(1000)
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << info.portName();
        _serial_ports.push_back(info.portName());

    }

    if (!_serial_ports.empty()){
        this->_portName = _serial_ports[1];
    }
    else{
        qDebug() << "no chuankou";
    }

    connect(&_thread, &MasterThread::response, this, &SendWaterTakingSignal::emit_response);
    connect(&_thread, &MasterThread::error, this, &SendWaterTakingSignal::emit_error);
    connect(&_thread, &MasterThread::timeout, this, &SendWaterTakingSignal::emit_timeout);
}

void SendWaterTakingSignal::send(int seconds){
    QString request = "aad000002bb";
    QString str_seconds = "000000";
    if (seconds>999999){
        str_seconds = "999999";
    }
    else if (seconds <= 0){
        str_seconds = "000000";
    }
    else{

        char c[6];
        itoa(seconds, c, 10);

        QString temp(c);

        while (temp.size()<6){
            temp = '0' + temp;
        }
        str_seconds = temp;
    }
    request = "aad" + str_seconds + "bb";
    this->_thread.transaction(this->_portName,
        this->_waitTimeout,
        request);
}

//void SendWaterTakingSignal::set_baud_rate(int baudrate){
//    this->_thread.my_set_BaudRate(baudrate);
//}

void SendWaterTakingSignal::set_baud_rate(QString baudrate){
    this->_thread.my_set_BaudRate(baudrate.toInt());
    qDebug()<<baudrate.toInt();
}

void SendWaterTakingSignal::set_serial_port(QString serial_port){
    this->_portName = serial_port;
}

void SendWaterTakingSignal::set_waittime(int waitTimeout){
    this->_waitTimeout=waitTimeout;
}

//-------------------------------------------------------------------


//----------------------------------------------------------------

SendWaterTakingSignal_view::SendWaterTakingSignal_view(QWidget *parent, SendWaterTakingSignal *watertaking)
: QWidget(parent)
, transactionCount(0)
, serialPortLabel(new QLabel(tr("串口")))
, serialPortComboBox(new QComboBox())
, waitResponseLabel(new QLabel(tr("等待回应秒数")))
, waitResponseSpinBox(new QSpinBox())
, secondsLabel(new QLabel(tr("取水秒数")))
, secondsSpinBox(new QSpinBox())
, baud_rateLabel(new QLabel(tr("波特率：")))
, baud_rateLineEdit(new QLineEdit(tr("115200")))
, trafficLabel(new QLabel(tr("No traffic.")))
, statusLabel(new QLabel(tr("Status: Not running.")))
, runButton(new QPushButton(tr("Start")))
, send_water_taking(watertaking)
{
    if (send_water_taking){
        foreach(const QString &info, send_water_taking->get_serial_ports())
            serialPortComboBox->addItem(info);


        QGridLayout *mainLayout = new QGridLayout;
        mainLayout->addWidget(serialPortLabel, 0, 0);
        mainLayout->addWidget(serialPortComboBox, 0, 1);
        mainLayout->addWidget(waitResponseLabel, 1, 0);
        mainLayout->addWidget(waitResponseSpinBox, 1, 1);

        mainLayout->addWidget(secondsLabel, 2, 0);
        mainLayout->addWidget(secondsSpinBox, 2, 1);

        mainLayout->addWidget(runButton, 0, 3, 2.5, 1);
        mainLayout->addWidget(baud_rateLabel, 3, 0);
        mainLayout->addWidget(baud_rateLineEdit, 3, 1);
        mainLayout->addWidget(trafficLabel, 4, 0, 1, 4);
        mainLayout->addWidget(statusLabel, 5, 0, 1, 5);
        setLayout(mainLayout);

        setWindowTitle(tr("Blocking Master"));
        serialPortComboBox->setFocus();

        waitResponseSpinBox->setRange(0, 1000 * 180);
        waitResponseSpinBox->setValue(1000);

        secondsSpinBox->setRange(0, 30);
        secondsSpinBox->setValue(2);

        connect(runButton, &QPushButton::clicked, this, &SendWaterTakingSignal_view::transaction);
        connect(send_water_taking, &SendWaterTakingSignal::response, this, &SendWaterTakingSignal_view::showResponse);
        connect(send_water_taking, &SendWaterTakingSignal::error, this, &SendWaterTakingSignal_view::processError);
        connect(send_water_taking, &SendWaterTakingSignal::timeout, this, &SendWaterTakingSignal_view::processTimeout);

        connect(serialPortComboBox,&QComboBox::currentTextChanged,send_water_taking,&SendWaterTakingSignal::set_serial_port);
        send_water_taking->set_serial_port(serialPortComboBox->currentText());

        connect(baud_rateLineEdit,&QLineEdit::textChanged,send_water_taking,&SendWaterTakingSignal::set_baud_rate);
        send_water_taking->set_baud_rate("115200");
    }
}


void SendWaterTakingSignal_view::transaction()
{
    setControlsEnabled(false);

    send_water_taking->set_waittime(waitResponseSpinBox->value());//
    send_water_taking->send(secondsSpinBox->value());             //

    statusLabel->setText(tr("Status: Running, connected to port %1.")
        .arg(serialPortComboBox->currentText()));

}

void SendWaterTakingSignal_view::showResponse(const QString &s)
{
    setControlsEnabled(true);
    trafficLabel->setText(tr("Traffic, transaction #%1:"
        "\n\r-response: %2")
        .arg(++transactionCount)
        .arg(s));
}

void SendWaterTakingSignal_view::processError(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Not running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));
}

void SendWaterTakingSignal_view::processTimeout(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));
}

void SendWaterTakingSignal_view::setControlsEnabled(bool enable)
{
    runButton->setEnabled(enable);
    serialPortComboBox->setEnabled(enable);
    waitResponseSpinBox->setEnabled(enable);
    baud_rateLineEdit->setEnabled(enable);
}
