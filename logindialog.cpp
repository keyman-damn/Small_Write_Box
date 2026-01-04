#include "logindialog.h"
#include "ui_logindialog.h"
#include "QStyle"
#include "newaccount.h"
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QRegularExpression>
#include <QGraphicsDropShadowEffect>
#include <QDateTime>
#include "databasemanager.h"
#include "form.h"
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setStyleSheet(
            "QLineEdit { "
            "   color: black; "
            "}"
                );
        setWindowFlags(Qt::FramelessWindowHint);  // 无边框
        setAttribute(Qt::WA_TranslucentBackground);  // 透明背景
        setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

        ui->Account->setPlaceholderText("请输入手机号码");
        ui->Code->setPlaceholderText("请输入密码");
        ui->stackedWidget->setCurrentWidget(ui->page);
        setupConnections();

        }



LoginDialog::~LoginDialog()
{
    delete ui;
}
void LoginDialog::setupConnections(){
    connect(ui->close_Btn,&QPushButton::clicked,this,&LoginDialog::close);
    connect(ui->new_At_Btn,&QPushButton::clicked,this,[this](){
        NewAccount *nA = new NewAccount(this);
                nA->setWindowTitle("用户注册");

                // 显示模态对话框
                if (nA->exec() == QDialog::Accepted) {
                    // 注册成功，获取注册的手机号
                    QString registeredPhone = nA->getRegisteredPhone();  // 注意方法名变化
                    if (!registeredPhone.isEmpty()) {
                        ui->Account->setText(registeredPhone);
                        ui->Code->setFocus();
                        QMessageBox::information(this, "注册成功",
                            "账号注册成功！手机号已自动填充。");
                    }
                }

                delete nA;  // 清理内存
            });
    connect(ui->Login_Btn, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
}
QString LoginDialog::getPhoneNumber() const
{
    return ui->Account->text().trimmed();
}
QString LoginDialog::getPassword() const
{
    return ui->Code->text();
}
bool LoginDialog::validateInput()
{
    // 获取输入
    QString username = ui->Account->text().trimmed();
    QString password = ui->Code->text();

    // 验证用户名
    if (username.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "用户名不能为空！");
        ui->Account->setFocus();
        return false;
    }

    // 验证密码
    if (password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "密码不能为空！");
        ui->Code->setFocus();
        return false;
    }


    return true; // 所有验证通过
}

void LoginDialog::onLoginClicked()
{
    if (!validateInput()) {
        return;
    }
    QString phone = getPhoneNumber();
        QString password = getPassword();


        // 1. 打开数据库连接
        if (!DatabaseManager::instance().openDatabase()) {
            QMessageBox::critical(this, "数据库错误", "无法连接数据库，请检查！");
            return;
        }

        // 2. 验证登录
        bool loginSuccess = DatabaseManager::instance().loginUser(phone, password);

        if (loginSuccess) {
            // 3. 登录成功：保存登录状态
            saveLoginState();

            // 4. 显示成功消息
            QMessageBox::information(this, "登录成功", "欢迎回来！");

            // 5. 关闭对话框（返回 QDialog::Accepted）
            accept();
            emit Login();

        } else {
            // 登录失败
            QMessageBox::warning(this, "登录失败",
                "手机号或密码错误！\n"
                "请检查输入或注册新账号。");

            // 清空密码框，重新聚焦
            ui->Code->clear();
            ui->Code->setFocus();
        }


}
void LoginDialog::saveLoginState()
{
    QString phone = getPhoneNumber();

    QSettings settings;
    settings.setValue("login/lastPhone", phone);
    settings.setValue("login/lastLoginTime", QDateTime::currentDateTime().toString());

    qDebug() << "保存登录状态 - 手机号:" << phone;
}
void LoginDialog::loadLastLoginInfo()
{
    QSettings settings;
    QString lastPhone = settings.value("login/lastPhone").toString();
    if (!lastPhone.isEmpty()) {
        ui->Account->setText(lastPhone);
        ui->Code->setFocus();  // 焦点放到密码框
    }
}
