#include "dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{   setWindowTitle(tr("demo1"));

    label1=new QLabel(tr("工作参数设置"));
    bt1=new QPushButton(tr("读取异常"));
    bt2=new QPushButton(tr("清除警告"));
    gbox1=new QGridLayout(this);

    gbox1->addWidget(label1,0,0);
    gbox1->addWidget(bt1,0,1);
    gbox1->addWidget(bt2,0,2);

    gbox1->setColumnStretch(0,1);
    gbox1->setColumnStretch(1,3);
}

Dialog::~Dialog()
{

}
