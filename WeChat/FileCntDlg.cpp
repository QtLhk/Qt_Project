#include "FileCntDlg.h"
#include "ui_FileCntDlg.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QTcpSocket>
#include <QTcpSocket>
#include <qtcpsocket.h>
#include"FileSrvDlg.h"
FileCntDlg::FileCntDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileCntDlg)
{
    ui->setupUi(this);
    port=8388;
    cfd=new QTcpSocket(this);
}

FileCntDlg::~FileCntDlg()
{
    delete ui;
}

void FileCntDlg::getLocalPath(QString path)
{
    this->path=path;
}

void FileCntDlg::getSrcAddr(QHostAddress hostip)
{
    this->ip=hostip;
}

void FileCntDlg::recvFile()
{
    qDebug()<<"客户端连接ip："<<ip<<endl;
    //连接服务器
    ui->lineEdit->setText ("127.0.0.1");
    QString ip1=ui->lineEdit->text ();
    cfd->connectToHost (QHostAddress(ip1),port);
    connect (cfd,&QTcpSocket::connected,this,[=](){
        //连接上服务器后发送文件
        qDebug()<<"与服务器连接上"<<endl;
    });

    connect(cfd,&QTcpSocket::readyRead,this,[=](){
//        //接收数据
//        QFile* fp=new QFile(path);
//        fp->open (QFile::WriteOnly);
//        qDebug()<<"路径："+path<<endl;
//        qDebug()<<"开始接收数据"<<endl;
//        QByteArray  allFile=cfd->readAll ();
//        fp->write (allFile);
//        fp->close ();
//        fp->deleteLater ();

//        qint64 fileSize;
//        QDataStream in(cfd);
//        in >> fileSize;

//        QFile receivedFile(path);
//        if (receivedFile.open(QIODevice::WriteOnly)) {
//            QByteArray fileData = cfd->read(fileSize);
//            receivedFile.write(fileData);

//            receivedFile.close();
//        }

        int fileSize=0;
        QDataStream in(cfd);
        in>>fileSize;       //从服务器端读取文件大小

        QFile fp(path);
        if (fp.open(QIODevice::WriteOnly)) { // 尝试以只写模式打开文件
            int bytesReceived = 0; // 记录已接收的字节数

            const int chunkSize = 1024 * 1024; // 每块大小（1MB）

            while (bytesReceived < fileSize) {
                QByteArray chunk = cfd->read(qMin(chunkSize, fileSize - bytesReceived)); // 从服务器端读取一块文件内容
                qint64 bytesWritten = fp.write(chunk); // 将块写入本地文件
                bytesReceived += bytesWritten; // 更新已接收字节数
            }

            fp.close(); // 关闭文件
        } else {
            // 处理文件保存失败，可能是无法写入或文件已存在
        }

    });
}

