#ifndef FILESRVDLG_H
#define FILESRVDLG_H

#include <QDialog>
#include<QMessageBox>
#include<QFile>
#include<QFileDialog>
#include<QTime>
#include<QTcpServer>
#include<QTcpSocket>
namespace Ui {
class FileSrvDlg;
}

class FileSrvDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileSrvDlg(QWidget *parent = nullptr);
    ~FileSrvDlg();
    void cntRefused();  //被客户端拒绝后的处理方法
    QString getServerAdress();

protected:
   // void closeEvent(QCloseEvent *event);    //重写窗口关闭事件
private slots:
//    void sndChatMsg();      //发送消息方法
//    void refreshProgress(int num);  //刷新服务器进度条方法


    void on_select_Btn_clicked();

    void on_send_btn_clicked();

    void sendFile();

private:
    Ui::FileSrvDlg *ui;
    QTcpServer* fd;
    QTcpSocket* cfd;
    unsigned short port;

    QFile *myLocalPathFile; //文件对象指针
    QString myPathFile;     //含路径的本地待发送文件名
    QString myFileName;     //文件名

    int myTotalBytes;       //总共要发送的字节数
    int mySendBytes;        //已发送的字节数
    int myBytesTobeSend;    //余下的字节数
    int myPayloadSize;      //有效载荷
    QByteArray myOutputBlock;   //缓存一次发送的数据
    QTime mytime;


    QHostAddress ip;
    QString path;
signals:
    void sendFileName(QString name);
    void over();
};

#endif // FILESRVDLG_H
