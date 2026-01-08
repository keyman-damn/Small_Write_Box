#ifndef MANAGER_H
#define MANAGER_H
#include <QVBoxLayout>
#include <QWidget>
#include <QStackedWidget>

namespace Ui {
class Manager;
}

class Manager : public QWidget
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();

private slots:
    void onContentAuditClicked();    // 内容审核按钮点击
    void onReportHandlingClicked();  // 举报处理按钮点击
    void onUserManagementClicked();  // 用户管理按钮点击

private:
    Ui::Manager *ui;
    QVBoxLayout *commentLayout;
    QStackedWidget *stackedWidget;   // 用于切换不同内容

    // 初始化各个界面
    void initContentAudit();    // 初始化内容审核
    void initReportHandling();  // 初始化举报处理
    void initUserManagement();  // 初始化用户管理

};

#endif // MANAGER_H
