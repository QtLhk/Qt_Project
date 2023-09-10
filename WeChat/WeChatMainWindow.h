#ifndef WECHATMAINWINDOW_H
#define WECHATMAINWINDOW_H

#include "FileCntDlg.h"
#include "FileSrvDlg.h"
#include "Manager.h"
#include <QMainWindow>
#include<QUdpSocket>
#include <QFileDiaLog>
class FileSrvDlg;
class FileCntDlg;
namespace Ui {
class WeChatMainWindow;
}
class WeChatMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WeChatMainWindow(QString name="newUser",QWidget *parent = nullptr);
    ~WeChatMainWindow();
    //消息类型设计
    enum ChatMsgType{
        ChatMsg,        //聊天内容
        OnLine,         //用户上线
        OffLine,        //用户离线
        SfileName,      //要传输的文件名
        RefFile         //拒收文件
    };



    //初始化用户基本信息
    void InitUserInfo(QString name);
    //图标初始化
    void InitIcon();
    //初始化UDP广播
    void InitUDP();
    //广播UDP消息
    void sendMsg(ChatMsgType type);
    //接收UDP广播
    void recvMsg();
    //获取聊天消息
    QString getMsg();
    //获取本地机器主机IP地址
    QString getLocHostIp();
    //处理用户上线通知
    void onLine(QString name,QString time);
    //处理用户下线通知
    void offLine(QString name,QString time);
    //文件处理
    void recvFileName(QString name,QString hostip,QString rmtname,QString filename);
signals:
    void winclosed();
protected:
    //重写关闭窗口方法获取信号
    void closeEvent(QCloseEvent *event);
private slots:
    void on_bold_btn_clicked(bool checked);

    void on_italic_btn_clicked(bool checked);

    void on_underline_btn_clicked(bool checked);

    void on_save_btn_clicked();

    void on_clear_btn_clicked();

    void on_exit_btn_clicked();

    void getSfileName(QString name);


    void on_color_btn_clicked();

private:
    Ui::WeChatMainWindow *ui;
    QString userName;
    QUdpSocket *fd;
    unsigned short port=8383;   //初始化端口号
    QString myFileName;
    FileSrvDlg* myfsrv;
    QString recvName;
};

#endif // WECHATMAINWINDOW_H
