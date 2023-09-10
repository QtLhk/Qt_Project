#ifndef FILECNTDLG_H
#define FILECNTDLG_H

#include <QDialog>
#include <QTcpSocket>
#include<QHostAddress>
namespace Ui {
class FileCntDlg;
}

class FileCntDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileCntDlg(QWidget *parent = nullptr);
    ~FileCntDlg();
    void getLocalPath(QString path);
    void getSrcAddr(QHostAddress(hostip));
    void recvFile();
private:
    Ui::FileCntDlg *ui;
    QTcpSocket* cfd;
    QHostAddress ip;
    unsigned short port;
    QString path;

};

#endif // FILECNTDLG_H
