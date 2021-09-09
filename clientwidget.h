#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <QMessageBox>

namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();

private slots:
    void on_buttonConnect_clicked();

private:
    Ui::ClientWidget *ui;

    QTcpSocket *tcpSocket;

    bool getHead = false;

    QString fileName;
    quint64 fileSize;
    quint64 receivedSize;

    QFile file;

};

#endif // CLIENTWIDGET_H
