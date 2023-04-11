#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    //左边
    QLabel *label1;
    QPushButton *bt1,*bt2;
    QGridLayout *gbox1;
};

#endif // DIALOG_H
