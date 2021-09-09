#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QIODevice>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWidget; }
QT_END_NAMESPACE

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();

    void sendData();

private slots:
    void on_buttonFile_clicked();
    void on_buttonSend_clicked();

private:
    Ui::ServerWidget *ui;

    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

    QString filename;
    quint64 fileSize;
    quint64 sendSize;

    QFile file;
    QTimer myTimer;
};
#endif // SERVERWIDGET_H
