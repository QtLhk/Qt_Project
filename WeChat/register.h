#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();
private slots:
    void registerOver();
private:
    Ui::Register *ui;
    bool isExist;
};

#endif // REGISTER_H
