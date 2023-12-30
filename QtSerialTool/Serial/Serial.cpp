#include "Serial.h"

//����ͨ�ź���
void Serial::USART(QString port, QString baud, QString data,QString stop,QString check) {
    QSerialPort::BaudRate Baud;     //������
    QSerialPort::DataBits Data;     //����λ
    QSerialPort::StopBits Stop;     //ֹͣλ
    QSerialPort::Parity Check;      //У��λ

    if (baud == "4800")  Baud = QSerialPort::Baud4800;
    else if (baud == "9600") Baud = QSerialPort::Baud9600;
    else if (baud == "19200")  Baud = QSerialPort::Baud19200;
    
    if (data == "8") Data = QSerialPort::Data8;

    if (stop == "1") Stop = QSerialPort::OneStop;
    else if (stop == "1.5")Stop = QSerialPort::OneAndHalfStop;
    else if (stop == "2") Stop = QSerialPort::TwoStop;

    if (check == QString::fromLocal8Bit("��")) Check = QSerialPort::NoParity;
    else if (check == QString::fromLocal8Bit("��У��")) Check = QSerialPort::OddParity;
    else if (check == QString::fromLocal8Bit("żУ��")) Check = QSerialPort::EvenParity;

    serialPort = new QSerialPort(this);
    //Ϊ������������
    serialPort->setBaudRate(Baud);
    serialPort->setPortName(port);
    serialPort->setDataBits(Data);
    serialPort->setParity(Check);
    serialPort->setStopBits(Stop);
    //�򿪴���
    if (serialPort->open(QSerialPort::ReadWrite)) {
        //�����źŲ�,һ���յ�������ʼ��ȡ
        QObject::connect(serialPort, &QSerialPort::readyRead, [&]() {
            auto data = serialPort->readAll();
            if (receiveMode->currentText() == "HEX") {      //�ֽ�ģʽ
                QString hex = data.toHex(' ');
                receiveAera->appendPlainText(hex);
            }
            else {                                          //�ı�ģʽ
                QString str = QString(data);
                receiveAera->appendPlainText(str);
            }
            });
    }else {
        QMessageBox::critical(this, QString::fromLocal8Bit("���ڴ�ʧ��"), QString::fromLocal8Bit("��ȷ�ϴ����Ƿ���ȷ����"));
    }
}

//ˢ�¿��ô���
void Serial::RefreshPort(void) {
    QVector<QString>temp;
    //��ȡ��ǰ���ô��ں�
    for (const QSerialPortInfo& info : QSerialPortInfo::availablePorts()) {
        temp.push_back(info.portName());
    }
    //�������еĴ��ں�,���ڱȽϺ�ԭ�еĲ��
    qSort(temp.begin(), temp.end());
    if (temp != this->ports) {  //������ô��ں��б仯
        this->portNumber->clear();  //���ԭ���б�
        this->ports = temp;         //���´����б�
        for (auto& a : ports) {     //�����´���
            this->portNumber->addItem(a);
        }
    }
}

//��������ʼ��
void Serial::ReceiveAeraInit(QWidget* parent) {
    receiveAera = new QPlainTextEdit(parent);
    receiveAera->setFixedSize(800,400);
    receiveAera->move(30,20);
    receiveAera->setReadOnly(true);     //��������Ϊֻ��

    QPushButton* clearReceive = new QPushButton(QString::fromLocal8Bit("��ս�����"),parent);
    clearReceive->setFixedSize(150,50);
    clearReceive->move(680,430);
    //Ϊ��ս����������źŲ�
    QObject::connect(clearReceive, &QPushButton::clicked, [&]() {
        receiveAera->clear();
        });
}

//��������ʼ��
void Serial::SendAeraInit(QWidget* parent) {
    sendAera = new QPlainTextEdit(parent);
    sendAera->setFixedSize(800,100);
    sendAera->move(30,500);

    QPushButton* clearSend = new QPushButton(QString::fromLocal8Bit("��շ�����"), parent);
    clearSend->setFixedSize(150, 50);
    clearSend->move(680, 630);
    QObject::connect(clearSend, &QPushButton::clicked, [&]() {
        sendAera->clear();
        });

    sendButton = new QPushButton(QString::fromLocal8Bit("����"), parent);
    sendButton->setFixedSize(150, 50);
    sendButton->move(500, 630);
    sendButton->setDisabled(true);
    QObject::connect(sendButton, &QPushButton::clicked, [&]() {
        QString data = sendAera->toPlainText();
        if (sendMode->currentText() == "HEX") {
            QByteArray arr;
            for (int i = 0; i < data.size(); i++){
                if (data[i] == ' ') continue;
                int num = data.mid(i, 2).toUInt(nullptr, 16);       //������תΪ16����
                i++;
                arr.append(num);
            }
            serialPort->write(arr);
        }else {
            serialPort->write(data.toLocal8Bit().data());           //תΪutf-8��ʽ�ַ���д��
        }
        });
}

//��ʱ�¼�
void Serial::timerEvent(QTimerEvent* e) {
    RefreshPort();      //���¶˿�     
}

//�������ó�ʼ��
void Serial::SetupInit(QWidget* parent) {
    this->portNumber = new QComboBox(parent);
    this->baudRate = new QComboBox(parent);
    this->dataSize = new QComboBox(parent);
    this->stopSize = new QComboBox(parent);
    this->check = new QComboBox(parent);
    this->receiveMode = new QComboBox(parent);
    this->sendMode = new QComboBox(parent);

    this->baudRate->addItem("4800");
    this->baudRate->addItem("9600");
    this->baudRate->addItem("19200");
    this->dataSize->addItem("8");
    this->stopSize->addItem("1");
    this->stopSize->addItem("1.5");
    this->stopSize->addItem("2");
    this->check->addItem(QString::fromLocal8Bit("��"));
    this->check->addItem(QString::fromLocal8Bit("��У��"));
    this->check->addItem(QString::fromLocal8Bit("żУ��"));
    this->receiveMode->addItem(QString::fromLocal8Bit("HEX"));
    this->receiveMode->addItem(QString::fromLocal8Bit("�ı�"));
    this->sendMode->addItem(QString::fromLocal8Bit("HEX"));
    this->sendMode->addItem(QString::fromLocal8Bit("�ı�"));

    QLabel* portLabel = new QLabel(QString::fromLocal8Bit("���ں�"), parent);
    QLabel* baudLabel = new QLabel(QString::fromLocal8Bit("������"),parent);
    QLabel* dataLabel = new QLabel(QString::fromLocal8Bit("����λ"),parent);
    QLabel* stopLabel = new QLabel(QString::fromLocal8Bit("ֹͣλ"),parent);
    QLabel* checkLabel = new QLabel(QString::fromLocal8Bit("У��λ"),parent);
    QLabel* receiveModeLabel = new QLabel(QString::fromLocal8Bit("���ո�ʽ"),parent);
    QLabel* sendModeLabel = new QLabel(QString::fromLocal8Bit("���͸�ʽ"),parent);

    QVector<QComboBox*>setups;
    setups.push_back(portNumber);
    setups.push_back(baudRate);
    setups.push_back(dataSize);
    setups.push_back(stopSize);
    setups.push_back(check);
    setups.push_back(receiveMode);
    setups.push_back(sendMode);

    QVector<QLabel*>labels;
    labels.push_back(portLabel);
    labels.push_back(baudLabel);
    labels.push_back(dataLabel);
    labels.push_back(stopLabel);
    labels.push_back(checkLabel);
    labels.push_back(receiveModeLabel);
    labels.push_back(sendModeLabel);

    for (int i = 0; i < setups.size(); ++i) {
        setups[i]->setFixedSize(200, 50);
        setups[i]->move(850, 20 + i * 80);
        labels[i]->move(1080,25+i*80);
    }

}

//��������
void Serial::BeginUSART(QWidget* parent) {
    startUSART = new QPushButton(QString::fromLocal8Bit("��������"),parent);
    endUSART = new QPushButton(QString::fromLocal8Bit("�Ͽ�����"),parent);
    endUSART->setFixedSize(150, 50);
    endUSART->move(1000, 600);
    startUSART->setFixedSize(150, 50);
    startUSART->move(850,600);
    endUSART->setDisabled(true);        //һ��ʼû�����Ӵ���,��˹رհ�ť��ʼ��Ϊ��Ч
    //Ϊ�ر����Ӱ�ť�����źŲ�
    QObject::connect(endUSART, &QPushButton::clicked, [&]() {
        endUSART->setDisabled(true);        //ʹ�ر����Ӱ�ťʧЧ
        startUSART->setDisabled(false);     //ʹ���Ӱ�ť��Ч
        sendButton->setDisabled(true);      //ʹ���Ͱ�ťʧЧ
        serialPort->close();                //�Ͽ���������
        });
    //Ϊ���Ӱ�ť�����źŲ�
    QObject::connect(startUSART, &QPushButton::clicked, [&]() {
        QString port = portNumber->currentText();
        QString baud = baudRate->currentText();
        QString data = dataSize->currentText();
        QString stop = stopSize->currentText();
        QString ch = check->currentText();
        QString receive = receiveMode->currentText();
        QString send = sendMode->currentText();
        if (port != "") {       //�����ںŲ�Ϊ��,����Чʱ
            endUSART->setDisabled(false);   //ʹ�ر����Ӱ�ť��Ч
            sendButton->setDisabled(false); //ʹ���Ͱ�ť��Ч
            startUSART->setDisabled(true);  //ʹ���Ӱ�ťʧЧ
            USART(port,baud,data,stop,ch);  //���Ӵ���
        }
        });
}

Serial::Serial(QWidget *parent): QMainWindow(parent){
    this->setFixedSize(1200,750);
    this->setWindowTitle(QString::fromLocal8Bit("��������"));
    ReceiveAeraInit(this);
    SendAeraInit(this);
    SetupInit(this);
    BeginUSART(this);
    this->startTimer(1000);         //����1��Ķ�ʱ������ɨ����ô���
}

Serial::~Serial(){

}
