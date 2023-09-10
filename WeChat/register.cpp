#include "Register.h"
#include "ui_Register.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include<QDebug>
#include <QShortCut>

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    setWindowTitle ("注册");
    setWindowIcon (QIcon(":/icon/wechat.png"));
    //注册完成点击事件
    connect(ui->register_btn,&QPushButton::clicked,this,&Register::registerOver);

    //Enter键绑定
    QShortcut *EnterKey=new QShortcut(Qt::Key_Enter,ui->register_btn);
    connect(EnterKey,&QShortcut::activated,ui->register_btn,&QPushButton::click);

    QShortcut *ReturnKey=new QShortcut(Qt::Key_Return,ui->register_btn);
    connect(ReturnKey,&QShortcut::activated,ui->register_btn,&QPushButton::click);

}

Register::~Register()
{
    delete ui;
}

void Register::registerOver()
{
    //将注册的账户密码存储到文件
    isExist=false;
    QString userName=ui->userName_register->text ();
    QString passWord=ui->passWord_register->text ();
    if(userName.isEmpty ()||passWord.isEmpty ()){
        QMessageBox::warning (this,"注册","注册的账户或密码为空，注册无效!");
        return;
    }
    QFile registerFile("user.txt");
    QTextStream read(&registerFile);
    if(!registerFile.open (QIODevice::ReadWrite|QIODevice::Text)){
        QMessageBox::information (this,"打开文件","打开文件失败!");
        return;
    }
    while (!read.atEnd ()) {
        QString line=read.readLine ();
        read.readLine(); // 读取密码行，但不使用
        if(QString("用户名:")+userName==line){
            isExist=true;
            break;
        }
    }
    registerFile.close ();
    if (isExist) {
        QMessageBox::warning(this, "注册", "该用户名已被注册，请重新注册!");
        return;
    }
    if(!registerFile.open (QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append)){
        QMessageBox::information (this,"打开文件","打开文件失败!");
        return;
    }
    QTextStream write(&registerFile);
    write << QString("用户名:") << userName << endl;
    write << QString("密码:") << passWord << endl;
    QMessageBox::information(this, "注册", "注册成功!");
    registerFile.close();
    this->close();










}
