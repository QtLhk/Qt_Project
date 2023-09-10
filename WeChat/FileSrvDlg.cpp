#include "FileSrvDlg.h"
#include "ui_FileSrvDlg.h"

#include<QDataStream>
FileSrvDlg::FileSrvDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSrvDlg)
{
    ui->setupUi(this);
    fd=new QTcpServer(this);
    port=8388;
    connect(fd,&QTcpServer::newConnection,this,&FileSrvDlg::sendFile);
}

FileSrvDlg::~FileSrvDlg()
{
    delete ui;
}

QString FileSrvDlg::getServerAdress()
{
    return fd->serverAddress ().toString ();
}


void FileSrvDlg::on_select_Btn_clicked()
{
    path=QFileDialog::getOpenFileName (this);
    if(path.isEmpty ()){
        QMessageBox::warning (this,"打开文件","选择的文件路径不可为空!");
        return;
    }
    ui->fileName_ldt->setText (path);
    emit sendFileName (path);
}

void FileSrvDlg::on_send_btn_clicked()
{
    //设置服务器监听
    if(!fd->listen (QHostAddress::Any,port)){
        QMessageBox::warning (0,QObject::tr ("异常"),"打开Tcp端口出错");
        close ();
        return;
    }
    if(fd->isListening ()){
        qDebug()<<"服务器进入监听状态"<<endl;
    }
    emit over();
    connect(fd,&QTcpServer::newConnection,this,[=](){
       qDebug()<<"有新的客户端连接"<<endl;
    });

}

void FileSrvDlg::sendFile()
{
    qDebug()<<"建立新连接后发送文件"<<endl;
    //       //获取通信套接字
           cfd=fd->nextPendingConnection ();
//           QFile *sendFile=new QFile(path);
//           sendFile->open (QIODevice::ReadOnly|QIODevice::Text);
//           QByteArray allFile=sendFile->readAll ();
//           cfd->write (allFile);

       QFile sendFile(path);
       if (sendFile.open(QIODevice::ReadOnly)) {

           int bytesSent=0;      //记录已发送的字节数

           int sendFileSize = sendFile.size();

           // 发送文件大小
           QDataStream out(cfd);
           out << sendFileSize;
           const int chunkSize = 1024 * 1024; // 每块大小（1MB）

           while(bytesSent<sendFileSize){
               QByteArray chunk=sendFile.read (chunkSize);//读取一块文件内容
               bytesSent+=cfd->write (chunk);             //将块发送给客户端并更新已发送字节数
           }


           sendFile.close();}
       qDebug()<<"文件发送完毕"<<endl;
}
