#include "reportitem.h"
#include "ui_reportitem.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>

ReportItem::ReportItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportItem)
{
    ui->setupUi(this);
    // 确保文本可以换行
    ui->label->setWordWrap(true);

    qDebug() << "=== 开始创建ReportItem ===";

        // 1. 清除所有控件的固定geometry
        ui->userName->setGeometry(QRect());
        ui->label->setGeometry(QRect());
        ui->label_2->setGeometry(QRect());
        ui->label_3->setGeometry(QRect());
        ui->pushButton->setGeometry(QRect());
        ui->pushButton_2->setGeometry(QRect());

        // 2. 清除可能存在的旧布局
        if (this->layout()) {
            qDebug() << "删除ReportItem旧布局";
            delete this->layout();
        }

        // 3. 设置大小策略
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

        // 4. 创建新的布局
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(12);
        mainLayout->setContentsMargins(15, 15, 15, 15);

        // 5. 用户信息行（水平布局）
        QHBoxLayout *userInfoLayout = new QHBoxLayout();

        // 头像
        if (ui->label_3) {
            ui->label_3->setFixedSize(40, 40);
            ui->label_3->setScaledContents(true);
            userInfoLayout->addWidget(ui->label_3);
        }

        // 用户名
        if (ui->userName) {
            ui->userName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            ui->userName->setStyleSheet("font-weight: bold; font-size: 14px; color: #2c3e50;");
            userInfoLayout->addWidget(ui->userName);
        }

        // 举报原因
        if (ui->label_2) {
            ui->label_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            ui->label_2->setStyleSheet("font-size: 13px; color: #e74c3c; font-weight: bold;");
            userInfoLayout->addWidget(ui->label_2);
        }

        userInfoLayout->addStretch();
        mainLayout->addLayout(userInfoLayout);

        // 6. 被举报内容
        if (ui->label) {
            // 关键设置：允许换行
            ui->label->setWordWrap(true);
            ui->label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            ui->label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);

            ui->label->setStyleSheet(R"(
                QLabel {
                    font-size: 13px;
                    color: #34495e;
                    background-color: #f8f9fa;
                    border-left: 4px solid #e74c3c;
                    padding: 10px;
                    border-radius: 4px;
                    margin-top: 8px;
                    margin-bottom: 12px;
                }
            )");

            mainLayout->addWidget(ui->label);
        }

        // 7. 按钮行（水平布局）
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();

        // 通过按钮
        if (ui->pushButton) {
            ui->pushButton->setFixedSize(100, 36);
            ui->pushButton->setStyleSheet(R"(
                QPushButton {
                    background-color: #27ae60;
                    color: white;
                    border: none;
                    border-radius: 6px;
                    font-weight: bold;
                    padding: 8px;
                    font-size: 13px;
                }
                QPushButton:hover {
                    background-color: #229954;
                }
                QPushButton:pressed {
                    background-color: #1e8449;
                }
            )");
            buttonLayout->addWidget(ui->pushButton);

            connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
                QString user = ui->userName ? ui->userName->text() : "未知用户";
                qDebug() << "✅ 处理通过 - 举报人:" << user;
            });
        }

        // 拒绝按钮
        if (ui->pushButton_2) {
            ui->pushButton_2->setFixedSize(100, 36);
            ui->pushButton_2->setStyleSheet(R"(
                QPushButton {
                    background-color: #e74c3c;
                    color: white;
                    border: none;
                    border-radius: 6px;
                    font-weight: bold;
                    padding: 8px;
                    font-size: 13px;
                }
                QPushButton:hover {
                    background-color: #c0392b;
                }
                QPushButton:pressed {
                    background-color: #a93226;
                }
            )");
            buttonLayout->addWidget(ui->pushButton_2);

            connect(ui->pushButton_2, &QPushButton::clicked, this, [this]() {
                QString user = ui->userName ? ui->userName->text() : "未知用户";
                qDebug() << "❌ 处理拒绝 - 举报人:" << user;
            });
        }

        mainLayout->addLayout(buttonLayout);

        // 8. 设置ReportItem整体样式
        setMinimumHeight(180);

        setStyleSheet(R"(
            ReportItem {
                background-color: white;
                border: 1px solid #dfe6e9;
                border-radius: 10px;
                margin: 8px 0;
            }
            ReportItem:hover {
                border-color: #e74c3c;
                box-shadow: 0 2px 8px rgba(231, 76, 60, 0.15);
            }
        )");

        setAutoFillBackground(true);

        qDebug() << "=== ReportItem创建完成 ===";
}

ReportItem::~ReportItem()
{
    delete ui;
}

void ReportItem::setReportData(const QString &userName,
                               const QString &reason,
                               const QString &content)
{
    qDebug() << "设置举报数据 - 举报人:" << userName;

    // 设置用户名（举报人）
    ui->userName->setText(userName);

    // 设置举报原因（label_2）
    ui->label_2->setText(reason);

    // 设置被举报内容（label）
    ui->label->setText(content);

    // 更新显示
    update();
}
