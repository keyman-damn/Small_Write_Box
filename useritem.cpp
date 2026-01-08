#include "useritem.h"
#include "ui_useritem.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>

UserItem::UserItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserItem),
    currentStatus("正常")
{
    ui->setupUi(this);

    qDebug() << "=== 开始创建UserItem ===";

    // ============================================
    // 关键步骤：清除固定geometry，使用布局管理器
    // ============================================

    // 清除所有控件的固定geometry
    ui->pushButton->setGeometry(QRect());
    ui->pushButton_2->setGeometry(QRect());
    ui->pushButton_3->setGeometry(QRect());
    ui->label_3->setGeometry(QRect());
    ui->userName->setGeometry(QRect());
    ui->label->setGeometry(QRect());

    // 清除可能存在的旧布局
    if (this->layout()) {
        qDebug() << "删除UserItem旧布局";
        delete this->layout();
    }

    // 设置大小策略
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(150);
    setMaximumHeight(180);

    // ============================================
    // 创建新的布局
    // ============================================

    // 主水平布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 20, 25, 20);

    // 左侧：头像区域
    QVBoxLayout *avatarLayout = new QVBoxLayout();
    avatarLayout->setAlignment(Qt::AlignCenter);

    if (ui->label_3) {
        ui->label_3->setFixedSize(60, 60);
        ui->label_3->setStyleSheet(R"(
            QLabel {
                border-radius: 30px;
                background-color: #ecf0f1;
                border: 2px solid #bdc3c7;
                qproperty-alignment: AlignCenter;
            }
        )");
        avatarLayout->addWidget(ui->label_3);
    }

    mainLayout->addLayout(avatarLayout);

    // 中间：用户信息区域（垂直布局）
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(10);
    infoLayout->setContentsMargins(0, 5, 0, 5);

    // 用户信息行（水平布局）
    QHBoxLayout *userInfoRow = new QHBoxLayout();
    userInfoRow->setSpacing(30);

    // 用户名
    if (ui->userName) {
        ui->userName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->userName->setStyleSheet(R"(
            QLabel {
                font-size: 16px;
                font-weight: bold;
                color: #2c3e50;
                min-width: 120px;
            }
        )");
        userInfoRow->addWidget(ui->userName);
    }

    // 注册日期
    if (ui->label) {
        ui->label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->label->setStyleSheet(R"(
            QLabel {
                font-size: 14px;
                color: #7f8c8d;
                min-width: 150px;
            }
        )");
        userInfoRow->addWidget(ui->label);
    }

    userInfoRow->addStretch();
    infoLayout->addLayout(userInfoRow);

    // 状态显示区域（水平布局）
    QHBoxLayout *statusLayout = new QHBoxLayout();
    statusLayout->setSpacing(15);

    // 状态标签（动态添加）
    QLabel *statusLabel = new QLabel();
    statusLabel->setFixedSize(80, 28);
    statusLabel->setStyleSheet(R"(
        QLabel {
            font-size: 12px;
            font-weight: bold;
            border-radius: 14px;
            padding: 2px 10px;
            qproperty-alignment: AlignCenter;
            background-color: #27ae60;
            color: white;
        }
    )");
    statusLabel->setText("正常");
    statusLayout->addWidget(statusLabel);

    // 举报次数标签（动态添加）
    QLabel *reportLabel = new QLabel("被举报: 0次");
    reportLabel->setFixedSize(100, 28);
    reportLabel->setStyleSheet(R"(
        QLabel {
            font-size: 12px;
            color: #e74c3c;
            font-weight: bold;
            background-color: rgba(231, 76, 60, 0.1);
            border-radius: 14px;
            padding: 2px 10px;
            qproperty-alignment: AlignCenter;
        }
    )");
    statusLayout->addWidget(reportLabel);

    statusLayout->addStretch();
    infoLayout->addLayout(statusLayout);

    mainLayout->addLayout(infoLayout);

    // 右侧：操作按钮区域（垂直布局）
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setSpacing(10);
    buttonLayout->setAlignment(Qt::AlignCenter);

    // 第一行按钮（解封和警告）
    QHBoxLayout *buttonRow1 = new QHBoxLayout();
    buttonRow1->setSpacing(15);

    if (ui->pushButton) {
        ui->pushButton->setFixedSize(100, 36);
        ui->pushButton->setStyleSheet(R"(
            QPushButton {
                background-color: #27ae60;
                color: white;
                border: none;
                border-radius: 8px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #229954;
            }
            QPushButton:disabled {
                background-color: #bdc3c7;
            }
        )");
        buttonRow1->addWidget(ui->pushButton);

        connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
            qDebug() << "🔓 解封用户:" << (ui->userName ? ui->userName->text() : "未知用户");
            currentStatus = "正常";
            updateButtonsByStatus();
            emit userUnbanned();
        });
    }

    if (ui->pushButton_3) {
        ui->pushButton_3->setFixedSize(100, 36);
        ui->pushButton_3->setStyleSheet(R"(
            QPushButton {
                background-color: #f39c12;
                color: white;
                border: none;
                border-radius: 8px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #d68910;
            }
            QPushButton:disabled {
                background-color: #bdc3c7;
            }
        )");
        buttonRow1->addWidget(ui->pushButton_3);

        connect(ui->pushButton_3, &QPushButton::clicked, this, [this]() {
            qDebug() << "⚠️ 警告用户:" << (ui->userName ? ui->userName->text() : "未知用户");
            currentStatus = "警告中";
            updateButtonsByStatus();
            emit userWarned();
        });
    }

    buttonLayout->addLayout(buttonRow1);

    // 第二行按钮（封禁）
    QHBoxLayout *buttonRow2 = new QHBoxLayout();
    buttonRow2->setSpacing(15);

    if (ui->pushButton_2) {
        ui->pushButton_2->setFixedSize(215, 36); // 宽度等于两个按钮宽度加间距
        ui->pushButton_2->setStyleSheet(R"(
            QPushButton {
                background-color: #e74c3c;
                color: white;
                border: none;
                border-radius: 8px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #c0392b;
            }
            QPushButton:disabled {
                background-color: #bdc3c7;
            }
        )");
        buttonRow2->addWidget(ui->pushButton_2);

        connect(ui->pushButton_2, &QPushButton::clicked, this, [this]() {
            qDebug() << "🚫 封禁用户:" << (ui->userName ? ui->userName->text() : "未知用户");
            currentStatus = "已封禁";
            updateButtonsByStatus();
            emit userBanned();
        });
    }

    buttonLayout->addLayout(buttonRow2);
    mainLayout->addLayout(buttonLayout);

    // 设置UserItem整体样式
    setStyleSheet(R"(
        UserItem {
            background-color: white;
            border: 1px solid #dfe6e9;
            border-radius: 12px;
            margin: 8px 0;
        }
        UserItem:hover {
            border-color: #3498db;
            box-shadow: 0 2px 12px rgba(52, 152, 219, 0.15);
        }
    )");

    setAutoFillBackground(true);

    // 存储动态创建的控件指针，以便后续更新
    // 这里可以使用QObject的setProperty来存储额外信息
    setProperty("statusLabel", QVariant::fromValue<QWidget*>(statusLabel));
    setProperty("reportLabel", QVariant::fromValue<QWidget*>(reportLabel));

    qDebug() << "=== UserItem创建完成 ===";
}

UserItem::~UserItem()
{
    delete ui;
}

void UserItem::setUserData(const QString &userId,
                          const QString &username,
                          const QString &registerDate,
                          const QString &status,
                          int reportCount,
                          const QString &email)
{
    this->userId = userId;
    this->currentStatus = status;

    qDebug() << "设置用户数据 - ID:" << userId << "用户名:" << username;

    // 设置用户名
    if (ui->userName) {
        ui->userName->setText(username);
    }

    // 设置注册日期
    if (ui->label) {
        ui->label->setText("注册: " + registerDate);
    }

    // 更新状态显示
    updateButtonsByStatus();

    // 更新状态标签
    QWidget* statusLabelWidget = property("statusLabel").value<QWidget*>();
    if (statusLabelWidget) {
        QLabel* statusLabel = qobject_cast<QLabel*>(statusLabelWidget);
        if (statusLabel) {
            QString statusText = status;
            QString statusStyle;

            if (status == "正常") {
                statusStyle = "background-color: #27ae60; color: white;";
            } else if (status == "警告中") {
                statusStyle = "background-color: #f39c12; color: white;";
            } else if (status == "已封禁") {
                statusStyle = "background-color: #e74c3c; color: white;";
            } else {
                statusStyle = "background-color: #95a5a6; color: white;";
            }

            statusLabel->setText(statusText);
            statusLabel->setStyleSheet(QString(R"(
                QLabel {
                    font-size: 12px;
                    font-weight: bold;
                    border-radius: 14px;
                    padding: 2px 10px;
                    qproperty-alignment: AlignCenter;
                    %1
                }
            )").arg(statusStyle));
        }
    }

    // 更新举报次数标签
    QWidget* reportLabelWidget = property("reportLabel").value<QWidget*>();
    if (reportLabelWidget) {
        QLabel* reportLabel = qobject_cast<QLabel*>(reportLabelWidget);
        if (reportLabel) {
            reportLabel->setText(QString("被举报: %1次").arg(reportCount));

            // 根据举报次数设置颜色
            QString reportStyle;
            if (reportCount == 0) {
                reportStyle = "color: #27ae60; background-color: rgba(39, 174, 96, 0.1);";
            } else if (reportCount < 3) {
                reportStyle = "color: #f39c12; background-color: rgba(243, 156, 18, 0.1);";
            } else {
                reportStyle = "color: #e74c3c; background-color: rgba(231, 76, 60, 0.1);";
            }

            reportLabel->setStyleSheet(QString(R"(
                QLabel {
                    font-size: 12px;
                    font-weight: bold;
                    border-radius: 14px;
                    padding: 2px 10px;
                    qproperty-alignment: AlignCenter;
                    %1
                }
            )").arg(reportStyle));
        }
    }

    // 设置头像（根据用户名首字母）
    if (ui->label_3) {
        if (!username.isEmpty()) {
            QString firstLetter = username.left(1).toUpper();

            // 根据首字母设置不同的背景色
            QChar firstChar = firstLetter.at(0);
            QString bgColor;

            if (firstChar >= 'A' && firstChar <= 'F') {
                bgColor = "#3498db";  // 蓝色
            } else if (firstChar >= 'G' && firstChar <= 'L') {
                bgColor = "#9b59b6";  // 紫色
            } else if (firstChar >= 'M' && firstChar <= 'R') {
                bgColor = "#2ecc71";  // 绿色
            } else {
                bgColor = "#e67e22";  // 橙色
            }

            ui->label_3->setStyleSheet(QString(R"(
                QLabel {
                    border-radius: 30px;
                    background-color: %1;
                    border: 2px solid %2;
                    color: white;
                    font-size: 24px;
                    font-weight: bold;
                    qproperty-alignment: AlignCenter;
                }
            )").arg(bgColor).arg(QColor(bgColor).darker(120).name()));

            ui->label_3->setText(firstLetter);
        }
    }

    qDebug() << "用户数据设置完成 - 状态:" << status << "举报次数:" << reportCount;
}

void UserItem::updateButtonsByStatus()
{
    qDebug() << "更新按钮状态 - 当前状态:" << currentStatus;

    // 根据当前状态启用/禁用按钮
    if (ui->pushButton) {    // 解封按钮
        ui->pushButton->setEnabled(currentStatus == "已封禁");
        if (currentStatus == "已封禁") {
            ui->pushButton->setStyleSheet(R"(
                QPushButton {
                    background-color: #27ae60;
                    color: white;
                    border: none;
                    border-radius: 8px;
                    font-size: 14px;
                    font-weight: bold;
                }
                QPushButton:hover {
                    background-color: #229954;
                }
            )");
        } else {
            ui->pushButton->setStyleSheet(R"(
                QPushButton {
                    background-color: #bdc3c7;
                    color: #7f8c8d;
                    border: none;
                    border-radius: 8px;
                    font-size: 14px;
                    font-weight: bold;
                }
            )");
        }
    }

    if (ui->pushButton_2) {  // 封禁按钮
        ui->pushButton_2->setEnabled(currentStatus != "已封禁");
        if (currentStatus != "已封禁") {
            ui->pushButton_2->setStyleSheet(R"(
                QPushButton {
                    background-color: #e74c3c;
                    color: white;
                    border: none;
                    border-radius: 8px;
                    font-size: 14px;
                    font-weight: bold;
                }
                QPushButton:hover {
                    background-color: #c0392b;
                }
            )");
        } else {
            ui->pushButton_2->setStyleSheet(R"(
                QPushButton {
                    background-color: #bdc3c7;
                    color: #7f8c8d;
                    border: none;
                    border-radius: 8px;
                    font-size: 14px;
                    font-weight: bold;
                }
            )");
        }
    }

    if (ui->pushButton_3) {  // 警告按钮
        ui->pushButton_3->setEnabled(currentStatus != "已封禁");
        if (currentStatus != "已封禁") {
            ui->pushButton_3->setStyleSheet(R"(
                QPushButton {
                    background-color: #f39c12;
                    color: white;
                    border: none;
                    border-radius: 8px;
                    font-size: 14px;
                    font-weight: bold;
                }
                QPushButton:hover {
                    background-color: #d68910;
                }
            )");
        } else {
            ui->pushButton_3->setStyleSheet(R"(
                QPushButton {
                    background-color: #bdc3c7;
                    color: #7f8c8d;
                    border: none;
                    border-radius: 8px;
                    font-size: 14px;
                    font-weight: bold;
                }
            )");
        }
    }

    // 更新按钮文本
    if (currentStatus == "警告中") {
        if (ui->pushButton_3) {
            ui->pushButton_3->setText("撤销警告");
        }
    } else {
        if (ui->pushButton_3) {
            ui->pushButton_3->setText("警告");
        }
    }
}
