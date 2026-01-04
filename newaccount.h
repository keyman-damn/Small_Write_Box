#ifndef NEWACCOUNT_H
#define NEWACCOUNT_H

#include <QDialog>

namespace Ui {
class NewAccount;
}

class NewAccount : public QDialog
{
    Q_OBJECT

public:
    explicit NewAccount(QWidget *parent = nullptr);
    ~NewAccount();
    QString getRegisteredPhone() const;
private slots:
    void onRegisterClicked();// 注册按钮（如果存在）
private:
    Ui::NewAccount *ui;
    QString registeredPhone;  // 存储注册的用户名

};

#endif // NEWACCOUNT_H
