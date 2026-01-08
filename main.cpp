// main.cpp
#include "form.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTimer>
#include <QDebug>
#include "logindialog.h"
#include "publish.h"
#include "page_profile.h"
#include "reportitem.h"
#include "reportmanager.h"
#include "useritem.h"
#include "manager.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
     //创建 MainWindow（主界面）
    MainWindow mainWindow;
    mainWindow.show();
    // 创建 登录界面
    LoginDialog dlg ;
    dlg.show();
    // 创建 Form（游戏偏好选择界面）
    Form form;

    // 连接 Logindialog 的登录信号到 Form 的显示
    QObject::connect(&dlg, &LoginDialog::Login, [&]() {
        qDebug() << "跳过选择，使用默认设置";
        dlg.close();           // 关闭 Logindialog 界面
        form.show();      // 显示 Form 界面
        form.activateWindow(); // 激活窗口
    });



    // 连接 Form 的跳过信号到 MainWindow 的显示
    QObject::connect(&form, &Form::skipClicked, [&]() {
        qDebug() << "跳过选择，使用默认设置";
        form.close();           // 关闭 Form 界面
        mainWindow.show();      // 显示 MainWindow 界面
        mainWindow.activateWindow(); // 激活窗口
    });

    // 新增：连接 Form 的确认选择信号
       QObject::connect(&form, &Form::confirmed, [&]() {
           qDebug() << "确认选择，获取选中的游戏";

           // 获取选中的游戏列表
           QStringList selectedGames = form.getSelectedGames();
           qDebug() << "选中的游戏：" << selectedGames;

           // 更新 MainWindow 的游戏按钮
           mainWindow.updateGameButtons(selectedGames);

           // 关闭 Form，显示 MainWindow
           form.close();
           mainWindow.show();
           mainWindow.activateWindow();
       });
    return a.exec();
}
