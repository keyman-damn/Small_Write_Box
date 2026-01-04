#include "newaccount.h"
#include "ui_newaccount.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "databasemanager.h"
NewAccount::NewAccount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewAccount)

{
    ui->setupUi(this);
    this->setStyleSheet(
            "QLineEdit { "
            "   color: black; "
            "}"
                );
    setWindowFlags(Qt::FramelessWindowHint);  // 无边框
    setAttribute(Qt::WA_TranslucentBackground);  // 透明背景
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->pre_Account->setPlaceholderText("请输入手机号码");
    ui->pre_Code->setPlaceholderText("请输入密码");
    ui->sure_Code->setPlaceholderText("再次确定密码");

    //添加注册按钮的连接
    connect(ui->register_Btn, &QPushButton::clicked,
         this, &NewAccount::onRegisterClicked);
    connect(ui-> close_Btn_2, &QPushButton::clicked,
         this, &NewAccount::close);

    QRegularExpression regExp("^[0-9]{11}$");
        QRegularExpressionValidator *validator = new QRegularExpressionValidator(regExp, this);
        ui->pre_Account->setValidator(validator);
        ui->pre_Code->setEchoMode(QLineEdit::Password);
        ui->sure_Code->setEchoMode(QLineEdit::Password);
}

NewAccount::~NewAccount()
{
    delete ui;
}
// 获取注册的用户名
QString NewAccount::getRegisteredPhone() const
{
    return registeredPhone;
}
// 注册按钮的槽函数（如果按钮存在）

void NewAccount::onRegisterClicked()
{
    QString phone = ui->pre_Account->text().trimmed();
    QString password = ui->pre_Code->text();
    QString confirmPassword = ui->sure_Code->text();

    // 验证输入
    if (phone.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "所有字段不能为空！");
        return;
    }

    // 验证手机号格式
    if (phone.length() != 11) {
        QMessageBox::warning(this, "输入错误", "手机号必须是11位数字！");
        ui->pre_Account->setFocus();
        return;
    }

    // 验证密码
    if (password.length() < 6) {
        QMessageBox::warning(this, "输入错误", "密码长度不能少于6位！");
        ui->pre_Code->setFocus();
        return;
    }

    // 验证两次密码是否一致
    if (password != confirmPassword) {
        QMessageBox::warning(this, "输入错误", "两次输入的密码不一致！");
        ui->sure_Code->clear();
        ui->sure_Code->setFocus();
        return;
    }
    // 打开数据库
    if (!DatabaseManager::instance().openDatabase()) {
        QMessageBox::critical(this, "数据库错误", "无法连接数据库！");
        return;
    }

    // 调用数据库注册
    if (DatabaseManager::instance().registerUser(phone, password)) {
        registeredPhone = phone;
        QMessageBox::information(this, "注册成功", "账号注册成功！");
        accept();  // 关闭对话框并返回 Accepted
    }
    }
