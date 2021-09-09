#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWidget)
{
    ui->setupUi(this);

   //first create a server to listen any network interface
   tcpServer = new QTcpServer(this);
   tcpServer->listen(QHostAddress::Any, 9999);

   //set the choose file button and send button to false since we dont have any connected client
   ui->buttonFile->setEnabled(false);
   ui->buttonSend->setEnabled(false);

   //if there is new connection, then we print the message, and enable the choose file button
   connect(tcpServer, &QTcpServer::newConnection, [=]()
   {
       tcpSocket = tcpServer->nextPendingConnection();

       QString ip = tcpSocket->peerAddress().toString();
       quint16 port = tcpSocket->peerPort();
       QString str = QString("[%1:%2] coonected successfully!").arg(ip).arg(port);
       ui->textEdit->setText(str);
       ui->buttonFile->setEnabled(true);
   });

   //the delay is to allow the head and the actual data can be sent seperately
   connect(&myTimer, &QTimer::timeout, [=]()
   {
       myTimer.stop();
       sendData();
   });
}

ServerWidget::~ServerWidget()
{
    delete ui;
}


void ServerWidget::on_buttonFile_clicked()
{
    //choose a file and get the file path
    QString filepath = QFileDialog::getOpenFileName(this, "Open file", "../");

    //if no file is chosen, then print the warning msg and quit
    if(filepath.isEmpty())
    {
        QMessageBox::information(this, "Warning!", "Error opening the file!");
        return;
    }

    //using QfileInfo class to get the file information
    QFileInfo info(filepath);

    filename = info.fileName();
    fileSize = info.size();
    sendSize = 0;

    //set the file path and open the file
    file.setFileName(filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, "Warning!", "Error opening the file!");
        return;
    }

    ui->textEdit->append(info.filePath() + " is ready to send!");

    //whenever we opened a file, we should set the choose file button to false
    //and enable the send button
    ui->buttonFile->setEnabled(false);
    ui->buttonSend->setEnabled(true);
}


void ServerWidget::on_buttonSend_clicked()
{
    //send the head of the file which contains the name and size of the file
    QString head = QString("%1##%2").arg(filename).arg(fileSize);

    quint64 len = tcpSocket->write(head.toStdString().data());

    if(len > 0)
    {
        //use timer to seperate the head and the actual data
        myTimer.start(20);
    }
    else
    {
        ui->textEdit->append("Unable to send the file!");
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
    }
}

void ServerWidget::sendData()
{
    quint64 len = 0;
    char buf[1024] = {0};
    do
    {
       //read the file data to buffer then write it to the socket
       len = file.read(buf, sizeof(buf));
       len = tcpSocket->write(buf, len);
       sendSize += len;
    }while(len > 0);

    //when the send size equals the file size, it marks complete
    if(sendSize == fileSize)
    {
        file.close();
        ui->textEdit->append("File successfully sent!");
        QMessageBox::information(this, "Message", "Done!");
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
        ui->buttonSend->setEnabled(false);
    }
}
