#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "newaccount.h"
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    QString getPhoneNumber() const;
    QString getPassword() const;
signals:
    //登陆成功信号
    void loginSuccess(const QString &phoneNumber);
    void Login();
    //请求注册信号
    void requestRegister();
private slots:
    void onLoginClicked();      // 登录按钮点击

private:
    Ui::LoginDialog *ui;
    bool validateInput();       // 验证输入
    void saveLoginState();      // 保存登录状态
    void setupConnections();    // 设置信号槽连接
    void loadLastLoginInfo();

};

#endif // LOGINDIALOG_H
