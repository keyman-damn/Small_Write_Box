#include "reportmanager.h"
#include "ui_reportmanager.h"
#include "reportitem.h"
#include <QVBoxLayout>
#include <QDebug>

ReportManager::ReportManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportManager)
{
    ui->setupUi(this);
    qDebug() << "=== ReportManager 初始化开始 ===";

        // 1. 获取scrollArea中已有的widget
        QWidget* contentWidget = ui->scrollArea->widget();
        if (!contentWidget) {
            qDebug() << "创建新的内容widget";
            contentWidget = new QWidget();
            ui->scrollArea->setWidget(contentWidget);
        }

        // 2. 获取或创建布局
        QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(contentWidget->layout());
        if (!mainLayout) {
            mainLayout = new QVBoxLayout(contentWidget);
        }

        // 3. 清除布局中UI文件预置的占位widget
        QLayoutItem* child;
        while ((child = mainLayout->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->deleteLater();
            }
            delete child;
        }

        // 4. 设置布局属性
        mainLayout->setSpacing(15);
        mainLayout->setContentsMargins(20, 20, 20, 20);
        mainLayout->setAlignment(Qt::AlignTop);

        // 5. 添加测试举报数据
        QStringList reasons = {
            "语言侮辱：发表不当言论",
            "垃圾广告：频繁发布广告信息",
            "色情内容：包含不适宜内容",
            "人身攻击：对其他用户进行攻击",
            "违规行为：违反社区规定"
        };

        QStringList contents = {
            "你这个人是真的垃圾，不会玩就别玩！",
            "加VX：12345678，最新游戏外挂，稳定不封号",
            "这里有一些不适合公开的内容链接...",
            "你就是个废物，操作这么差还玩游戏",
            "使用外挂程序进行游戏，影响公平性"
        };

        for (int i = 0; i < 5; i++) {
            ReportItem* item = new ReportItem(contentWidget);

            // 设置举报数据
            item->setReportData(
                QString("举报人%1").arg(i+1),    // 举报人用户名
                reasons[i % reasons.size()],     // 举报原因
                contents[i % contents.size()]    // 被举报内容
            );

            mainLayout->addWidget(item);

            qDebug() << "已添加举报项:" << i+1;
        }

        // 6. 添加拉伸空间
        mainLayout->addStretch();

        // 7. 设置scrollArea属性
        ui->scrollArea->setWidgetResizable(true);
        ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        // 8. 重要：更新尺寸
        contentWidget->adjustSize();

        qDebug() << "=== ReportManager 初始化完成 ===";
        qDebug() << "ScrollArea尺寸:" << ui->scrollArea->size();
        qDebug() << "内容区域尺寸:" << contentWidget->size();
        qDebug() << "布局子项数:" << mainLayout->count();
}

ReportManager::~ReportManager()
{
    delete ui;
}
