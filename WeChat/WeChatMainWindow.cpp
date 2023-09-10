#include "WeChatMainWindow.h"
#include "ui_WeChatMainWindow.h"
#include"Login.h"
#include "FileSrvDlg.h"

#include <QFontDatabase>
#include <QMessageBox>
#include<QTextStream>
#include<QNetworkInterface>
#include<QFile>
#include <QDateTime>
#include <QColorDialog>
#include <QShortcut>

WeChatMainWindow::WeChatMainWindow(QString name,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WeChatMainWindow)
{
    ui->setupUi(this);
    //获取用户名
    this->userName=name;
    //图片初始化
    InitIcon();
    //初始化UDP广播
    InitUDP();
    //InitUserInfo (userName);
    //初始化上线信息
    sendMsg (OnLine);
    connect(ui->sendMsg_btn,&QPushButton::clicked,this,[=](){
        sendMsg (ChatMsg);
    });
    // 创建一个回车键的快捷键，并连接到按钮的点击槽函数
    // 创建 crtl+Enter 快捷键
    QKeySequence ctrlEnterKey(Qt::CTRL + Qt::Key_Enter);
    QShortcut* enterShortcut = new QShortcut(ctrlEnterKey, ui->sendMsg_btn);
    QShortcut* returnShortcut = new QShortcut(Qt::CTRL + Qt::Key_Return, ui->sendMsg_btn);
    connect(enterShortcut, &QShortcut::activated, ui->sendMsg_btn, &QPushButton::click);
    connect(returnShortcut, &QShortcut::activated, ui->sendMsg_btn, &QPushButton::click);

    QShortcut* escShortcut = new QShortcut(Qt::Key_Escape, ui->exit_btn); // 可以将 this 替换为你想要的父窗口或控件
    connect(escShortcut, &QShortcut::activated, ui->exit_btn, &QPushButton::click);

    //接受完消息处理消息
    connect (fd,&QUdpSocket::readyRead,this,&WeChatMainWindow::recvMsg);
    //离线信号事件处理
    connect(this,&WeChatMainWindow::winclosed,this,[=](){
       qDebug()<<"接收信号处理离线事件"<<endl;
        sendMsg (OffLine);
    });

     myfsrv=new FileSrvDlg();
    //获取发送文件名
    connect(myfsrv,&FileSrvDlg::sendFileName,this,&WeChatMainWindow::getSfileName);


    //文件发送按钮点击事件
    connect(ui->sendfile_btn,&QPushButton::clicked,this,[=](){
        if(ui->user_table->selectedItems ().isEmpty ()){
            QMessageBox::warning (0,tr("选择好友"),tr("请先选择文件接收方"),QMessageBox::Ok);
            return;
        }

        recvName=ui->user_table->currentItem ()->text ();
        myfsrv->show ();
        connect(myfsrv,&FileSrvDlg::over,this,[=](){
            sendMsg (SfileName);
        });
    });


}

WeChatMainWindow::~WeChatMainWindow()
{
    delete ui;
}

void WeChatMainWindow::InitUserInfo(QString name)
{
    ui->user_table->insertRow (0);
    QTableWidgetItem* item=new QTableWidgetItem(QIcon(QString(":/userIcon/%1.jpg").arg (userName)),userName);
    ui->user_table->setItem (0,0,item);

}
//图片初始化
void WeChatMainWindow::InitIcon()
{
    //初始化按钮图片
    ui->bold_btn->setIcon (QIcon(":/icon/bold.png"));
    ui->italic_btn->setIcon (QIcon(":/icon/italic.png"));
    ui->underline_btn->setIcon ((QIcon(":/icon/under.png")));
    ui->color_btn->setIcon ((QIcon(":/icon/color.png")));
    ui->sendfile_btn->setIcon ((QIcon(":/icon/send.png")));
    ui->save_btn->setIcon ((QIcon(":/icon/save.png")));
    ui->clear_btn->setIcon ((QIcon(":/icon/clear.png")));

    //初始化按钮字体提示
    ui->bold_btn->setToolTip ("加粗");
    ui->italic_btn->setToolTip ("倾斜");
    ui->underline_btn->setToolTip ("下划线");
    ui->color_btn->setToolTip ("颜色");
    ui->sendfile_btn->setToolTip ("发送文件");
    ui->save_btn->setToolTip ("保存聊天");
    ui->clear_btn->setToolTip ("清空");

    //初始化按钮选中样式
    ui->bold_btn->setCheckable (true);
    ui->italic_btn->setCheckable (true);
    ui->underline_btn->setCheckable (true);



    //初始化字体样式
    QFontDatabase fontDataBase;
    QStringList fontFamilies=fontDataBase.families ();
    ui->font->addItems (fontFamilies);


    //设置字体样式
    connect(ui->font,&QComboBox::currentTextChanged,this,[=](){
        QFont font=ui->font->currentText ();
        ui->message_edit->setCurrentFont (font);
        ui->message_edit->setFocus ();
    });

    //初始化字体大小
    QStringList fontSize;
    for(int i=2;i<34;i=i+2){
        fontSize<<QString::number (i);
    }
    ui->fontSize->addItems (fontSize);

    //设置字体大小
    connect (ui->fontSize,&QComboBox::currentTextChanged,this,[=](){
        ui->message_edit->setFontPointSize (ui->fontSize->currentText ().toInt ());
    });

    //初始化表
    ui->user_table->setColumnCount (1);
    ui->user_table->setHorizontalHeaderItem (0,new QTableWidgetItem("在线用户:"));
}
//初始化UDP广播
void WeChatMainWindow::InitUDP()
{
    fd=new QUdpSocket(this);
    //绑定端口
    fd->bind (port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
}
//使用数据流广播UDP消息
void WeChatMainWindow::sendMsg(ChatMsgType type)
{
    QByteArray array;
    QDataStream write(&array,QIODevice::WriteOnly);
    QString locHostIp=getLocHostIp ();
    qDebug()<<locHostIp<<endl;
    write<<type;
    switch (type) {
        case ChatMsg:
        if(ui->message_edit->toPlainText ().isEmpty ()){
            QMessageBox::warning (this,"警告","发送内容不能为空,请重新输入");
            return;
        }
            write<<userName<<locHostIp<<getMsg ();
        break;
        case OnLine:
            write<<userName<<locHostIp;
        break;
        case OffLine:
        write<<userName;
        break;
        case SfileName:
            write<<userName<<locHostIp<<recvName<<myFileName;
        break;
        case RefFile:
            write<<userName<<locHostIp<<recvName;
        break;
    }
    //消息处理完毕进行广播
    fd->writeDatagram (array,array.length (),QHostAddress::Broadcast,port);
}

void WeChatMainWindow::recvMsg()
{
    QByteArray array;
    //判断是否接收到数据包
    if(fd->hasPendingDatagrams ())
        array.resize (fd->pendingDatagramSize ());
    else
        qDebug()<<"未接受到数据包"<<endl;
    fd->readDatagram (array.data (),array.size ());
    QDataStream read(&array,QIODevice::ReadOnly);
    int msgType;
    read>>msgType;
    QString name,hostip,chatmsg,rname,fname;
    QString currentTime=QDateTime::currentDateTime ().toString ("yyyy-MM-dd hh:mm:ss");
    switch (msgType) {
        case ChatMsg:
            read>>name>>hostip>>chatmsg;
            ui->dialog_browser->append ("["+name+"]"+currentTime);
            ui->dialog_browser->append (chatmsg);
        break;
        case OnLine:
            read>>name>>hostip;
            onLine (name,currentTime);
        break;
        case OffLine:
            read>>name;
            offLine (name,currentTime);
        break;
        case SfileName:
            read>>name>>hostip>>rname>>fname;
            recvFileName (name,hostip,rname,fname);
        break;
        case RefFile:
            read>>name>>hostip>>rname;
            QMessageBox::information (this,"文件发送","文件被拒收");
        break;
    }
}

QString WeChatMainWindow::getMsg()
{
    QString msg=ui->message_edit->toPlainText ();
    ui->message_edit->clear();
    ui->message_edit->setFocus ();
    return msg;
}
//获取本机IP
QString WeChatMainWindow::
getLocHostIp()
{
    QList<QHostAddress> addrlist=QNetworkInterface::allAddresses ();
    for(QHostAddress addr:addrlist){
        if(addr.protocol ()==QAbstractSocket::IPv4Protocol)
            return addr.toString ();
    }
}

void WeChatMainWindow::onLine(QString name,QString time)
{
    bool isExist=ui->user_table->findItems (name,Qt::MatchExactly).isEmpty ();
    if(isExist){
        ui->user_table->insertRow (0);
        //初始化窗口用户信息
        setWindowTitle (name);
        QTableWidgetItem* item;
        QString iconPath(tr(":/userIcon/%1.jpg").arg (name));
        QFile file(iconPath);
        if(file.exists ()){
            setWindowIcon (QIcon(iconPath));
            item=new QTableWidgetItem(QIcon(iconPath),name);
        }
            else {
            setWindowIcon (QIcon(":/icon/wechat.png"));
            item=new QTableWidgetItem(QIcon(QString(":/icon/wechat.png")),name);
        }
        ui->user_table->setItem (0,0,item);
        ui->dialog_browser->append (tr("%1 %2 上线!").arg (time).arg (name));
        ui->usernum_lbl->setText (QString("在线用户人数:%1").arg (ui->user_table->rowCount ()));
        sendMsg (OnLine);
    }
}

void WeChatMainWindow::offLine(QString name, QString time)
{
    bool isExist=ui->user_table->findItems (name,Qt::MatchExactly).isEmpty ();
    if(!isExist){
        int row=ui->user_table->findItems (name,Qt::MatchExactly).first ()->row ();
        ui->user_table->removeRow (row);
        ui->dialog_browser->append (tr("%1 %2 离线!").arg (time).arg (name));
        ui->usernum_lbl->setText (QString("在线用户人数:%1").arg (ui->user_table->rowCount ()));
    }

}
//判断是否接收文件
void WeChatMainWindow::recvFileName(QString name, QString hostip, QString rmtname, QString filename)
{
    qDebug()<<QHostAddress(hostip).toString ();
    if(userName==rmtname){
        int result=QMessageBox::information (this,"收到文件",tr("好友%1给您发送文件%2,是否接收").arg (name).arg (filename),QMessageBox::Yes,QMessageBox::No);
        if(result==QMessageBox::Yes){
            QString fname=QFileDialog::getSaveFileName (this,"保存文件",filename);
            qDebug()<<"文件名"+filename<<endl;
            if(!fname.isEmpty ()){
                FileCntDlg *fcnt=new FileCntDlg(this);
                fcnt->getLocalPath (fname);
                qDebug()<<fname<<endl;
                fcnt->getSrcAddr (QHostAddress(hostip));
                fcnt->recvFile ();
            }
            else
                sendMsg (RefFile);
        }
    }
}
//重写关闭窗口方法获取信号
void WeChatMainWindow::closeEvent(QCloseEvent *event)
{
    onlineUsers.remove (userName);
    emit winclosed ();
    this->deleteLater ();
}

//设置粗体
void WeChatMainWindow::on_bold_btn_clicked(bool checked)
{
    if(checked)
        ui->message_edit->setFontWeight(QFont::Bold);
    else
        ui->message_edit->setFontWeight (QFont::Normal);
    ui->message_edit->setFocus ();
}
//设置斜体
void WeChatMainWindow::on_italic_btn_clicked(bool checked)
{
    if(checked)
        ui->message_edit->setFontItalic (true);
    else
        ui->message_edit->setFontItalic (false);
    ui->message_edit->setFocus ();
}
//设置下划线
void WeChatMainWindow::on_underline_btn_clicked(bool checked)
{
    if(checked)
        ui->message_edit->setFontUnderline (true);
    else
        ui->message_edit->setFontUnderline (false);
    ui->message_edit->setFocus ();
}

void WeChatMainWindow::on_save_btn_clicked()
{
    if(ui->dialog_browser->toPlainText ().isEmpty ()){
    QMessageBox::warning (this,"保存聊天记录","请勿保存空记录");
    return;
    }
    QString fileName=QFileDialog::getSaveFileName (this,"保存聊天记录","聊天记录","*.txt");
    QFile file(fileName);
    file.open (QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream<<ui->dialog_browser->toPlainText ();
    file.close ();
}

void WeChatMainWindow::on_clear_btn_clicked()
{
    ui->dialog_browser->clear ();
}

void WeChatMainWindow::on_exit_btn_clicked()
{
    this->close ();
    QMessageBox::information (this,"退出系统","您已成功退出本系统");
}

void WeChatMainWindow::getSfileName(QString fname)
{
    myFileName=fname;
}


void WeChatMainWindow::on_color_btn_clicked()
{
    QColor color=QColorDialog::getColor (Qt::red,this,"颜色选择");
    if(color.isValid ()){
        ui->message_edit->setTextColor (color);
        //获取焦点以便能够之继续编辑
        ui->message_edit->setFocus ();
}
}
