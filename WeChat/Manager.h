#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>
#include<QSet>
// 声明全局变量
extern QSet<QString> onlineUsers;   //管理在线人数
namespace Ui {
class Manager;
}

class Manager : public QWidget
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();

private:
    Ui::Manager *ui;

private slots:
    void addOnlineUsers(QString userName);
};

#endif // MANAGER_H
