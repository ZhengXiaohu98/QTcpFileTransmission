#include "clientwidget.h"
#include "ui_clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0); //set the progress bar to 0;

    tcpSocket = new QTcpSocket(this);

    //whenever we get the connected signal, we print a msg in the msg box
    connect(tcpSocket, &QTcpSocket::connected, [=]()
    {
        ui->textEdit->setText("Connected to the host successfully!");
    });

    //if there is data to read, we will decied if its the head/actual data
    connect(tcpSocket, &QTcpSocket::readyRead, [=]()
    {
        QByteArray data = tcpSocket->readAll();
        //if we didn't get the head, then get the head information first
        if(!getHead)
        {
            getHead = true;

            //use section to seperate the name and the size
            fileName = QString(data).section("##", 0, 0);
            fileSize = QString(data).section("##", 1, 1).toInt();
            receivedSize = 0;

            //open the file for write
            file.setFileName(fileName);
            if(!file.open(QIODevice::WriteOnly))
            {
                QMessageBox::information(this, "Warning!", "Error opening the file!");
                ui->textEdit->setText("Cannot open the file to write!");
                tcpSocket->disconnectFromHost();
                tcpSocket->close();
            }

            //set the progress bar min and max
            ui->progressBar->setMinimum(0);
            ui->progressBar->setMaximum(fileSize/1024);
            ui->progressBar->setValue(0);
        }
        else
        {
            //write the data to the file
            quint64 len = file.write(data);
            receivedSize += len;

            //set the current progress
            ui->progressBar->setValue(receivedSize/1024);

            if(receivedSize == fileSize)
            {
                QMessageBox::information(this, "Message!", "Done receiving the file!");
                ui->textEdit->setText("1 file received!");
                file.close();
                tcpSocket->disconnectFromHost();
                tcpSocket->close();
            }
        }
    });
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

//when the connect button is clicked, we request a connnection to the host
void ClientWidget::on_buttonConnect_clicked()
{
    QString ip = ui->lineEditIP->text();
    quint16 port = ui->lineEditPort->text().toInt();

    tcpSocket->connectToHost(ip, port);
}

