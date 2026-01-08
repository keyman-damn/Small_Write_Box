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
#include "usermanager.h"
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
                        QMessageBox regBox(QMessageBox::Information, "注册成功", "账号注册成功！手机号已自动填充。", QMessageBox::Ok, this);
                                regBox.setStyleSheet("QLabel{ color: #333333; } QPushButton{ color: #333333; }");
                                regBox.exec();
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
    // 1. 基础校验（非空判断）
    if (!validateInput()) {
        return;
    }

    QString phone = getPhoneNumber();
    QString password = getPassword();

    // 2. 确保数据库连接正常
    if (!DatabaseManager::instance().openDatabase()) {
        QMessageBox::critical(this, "数据库错误", "无法连接数据库，请检查！");
        return;
    }

    // 3. 验证登录（手机号 + 密码）
    bool loginSuccess = DatabaseManager::instance().loginUser(phone, password);

    if (loginSuccess) {
        // --- 【核心修改开始】 ---

        // 我们需要把手机号(QString) 转换成 数据库ID(int)
        int actualUserId = -1;

        QSqlQuery query;
        // 注意：这里的表名 users 和 列名 phone/id 需要和你数据库实际名称一致
        query.prepare("SELECT id FROM users WHERE phone = ?");
        query.addBindValue(phone);

        if (query.exec() && query.next()) {
            actualUserId = query.value(0).toInt(); // 获取真实的 int 类型 ID
        } else {
            // 如果验证成功却查不到ID（理论上不该发生），按错误处理
            QMessageBox::critical(this, "错误", "无法获取用户信息，请重试");
            return;
        }

        // 4. 将真实的 int ID 存入全局单例
        // 参数1：int id, 参数2：QString name
        UserManager::instance()->setUserInfo(actualUserId, phone);

        // --- 【核心修改结束】 ---

        // 5. 后续处理
        saveLoginState();
        QMessageBox welcomeBox(QMessageBox::Information, "登录成功",
                                       "欢迎回来，" + phone + "！",
                                       QMessageBox::Ok, this);

                // 强制设置文字颜色为深色，背景为浅色（防止被全局样式污染）
                welcomeBox.setStyleSheet(R"(
                    QLabel {
                        color: #333333;
                        font-size: 14px;
                    }
                    QPushButton {
                        color: #333333;
                        background-color: #f0f0f0;
                        border: 1px solid #ccc;
                        padding: 5px 15px;
                        border-radius: 4px;
                    }
                )");

                welcomeBox.exec(); // 显示对话框

        accept();      // 关闭对话框并返回 Accepted
        emit Login();  // 发射自定义登录信号

    } else {
        // 登录失败处理
        QMessageBox::warning(this, "登录失败", "手机号或密码错误！\n请重新输入。");
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
