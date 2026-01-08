#ifndef MESSAGE_RECIEVE_H
#define MESSAGE_RECIEVE_H

#include <QWidget>

namespace Ui {
class message_recieve;
}

class message_recieve : public QWidget
{
    Q_OBJECT

public:
    explicit message_recieve(QWidget *parent = nullptr);
    ~message_recieve();
    void  turnToHome();
private:
    Ui::message_recieve *ui;
};

#endif // MESSAGE_RECIEVE_H
