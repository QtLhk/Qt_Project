#include "Manager.h"
#include "ui_Manager.h"
#include"Login.h"

QSet<QString> onlineUsers; // 定义全局变量

Manager::Manager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Manager)
{
    ui->setupUi(this);
    setWindowIcon (QIcon(":/icon/wechat.png"));
    setWindowTitle ("微信客户端启动项");
    ui->bt->setText ("启动微信");
    ui->bt->setAutoDefault (true);
    QSet<QString> onlineUsers; // 定义全局变量
    connect(ui->bt,&QPushButton::clicked,this,[=](){
        Login *login=new Login;
        login->show ();
        connect(login,&Login::Logined,this,&Manager::addOnlineUsers);
    });
}

Manager::~Manager()
{
    delete ui;
}

void Manager::addOnlineUsers(QString userName)
{
    onlineUsers.insert (userName);
}
