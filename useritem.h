#ifndef USERITEM_H
#define USERITEM_H

#include <QWidget>

namespace Ui {
class UserItem;
}

class UserItem : public QWidget
{
    Q_OBJECT

signals:
    void userBanned();     // 用户被封禁信号
    void userWarned();     // 用户被警告信号
    void userUnbanned();   // 用户解封信号

public:
    explicit UserItem(QWidget *parent = nullptr);
    ~UserItem();

    // 设置用户数据
    void setUserData(const QString &userId,
                         const QString &username,
                         const QString &registerDate,
                         const QString &status = "正常",
                         int reportCount = 0,
                         const QString &email = "");

    // 获取用户ID
        QString getUserId() const { return userId; }  // 添加这行

private:
    Ui::UserItem *ui;
    QString currentStatus;  // 当前用户状态
    QString userId;         // 用户ID

    // 根据状态更新按钮显示
    void updateButtonsByStatus();
};

#endif // USERITEM_H
