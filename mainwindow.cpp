#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsDropShadowEffect>
#include <QScrollBar>
#include <QEvent>
#include <QDebug>
#include "logindialog.h"
#include "message_recieve.h"
#include "focus.h"
#include "newaccount.h"

namespace {
    // ================== 统一主色 ==================
    const QString ACCENT_COLOR = "#4f8cff";

    // ================== 文字分层颜色 ==================
    const QString TEXT_PRIMARY   = "#e8eaed";  // 主文本
    const QString TEXT_SECONDARY = "#b0b6bf";  // 次级文本
    const QString TEXT_WEAK      = "#8b949e";  // 弱文本
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("小白盒 - 你的游戏助手");
    this->setWindowIcon(QIcon(":/icons/gamer.png"));



    /* =================================================
     * 1. 全局样式（方案一：文字分层）
     * ================================================= */
    this->setStyleSheet(QString(R"(
        * {
            font-family: "Microsoft YaHei";
            font-size: 14px;
            color: %1;   /* 主文本 */
        }

        QMainWindow {
            background-color: #1e2228;
        }

        /* ================= 顶部 ================= */
        QWidget#widget {
            background-color: #252a31;
            border-bottom: 1px solid #2f3540;
        }

        QLabel {
            color: %1;
        }

        QLabel[secondary="true"] {
            color: %2;
        }

        QLabel[weak="true"] {
            color: %3;
        }

        /* ================= 搜索框 ================= */
        QLineEdit#lineEdit {
            background-color: #1e2228;
            border-radius: 18px;
            padding: 6px 14px;
            border: 1px solid #2f3540;
            color: %1;
        }

        QLineEdit#lineEdit::placeholder {
            color: %3;
        }

        QLineEdit#lineEdit:focus {
            border: 1px solid %4;
            background-color: #1b1f24;
        }

        /* ================= 顶部分类按钮 ================= */
        QPushButton {
            background-color: transparent;
            border: none;
            padding: 6px 14px;
            border-radius: 16px;
            color: %2;
        }

        QPushButton:hover {
            background-color: #2f3540;
            color: %1;
        }

        /* ================= 左侧栏 ================= */
        QWidget#widget_2 {
            background-color: #1b1f24;
            border-right: 1px solid #2f3540;
        }

        QWidget#widget_2 QPushButton {
            text-align: left;
            padding: 10px 16px;
            border-radius: 8px;
            color: %2;
        }

        QWidget#widget_2 QPushButton:hover {
            background-color: #2a2f38;
            color: %1;
        }

        /* ================= 滚动区 ================= */
        QScrollArea {
            background-color: transparent;
            border: none;
        }

        QScrollBar:vertical {
            width: 6px;
            background: transparent;
        }

        QScrollBar::handle:vertical {
            background: #3b4250;
            border-radius: 3px;
        }

        QScrollBar::handle:vertical:hover {
            background: %4;
        }

        /* ================= 内容卡片 ================= */
        QWidget#widget_3,
        QWidget#widget_4,
        QWidget#widget_5,
        QWidget#widget_6,
        QWidget#widget_7,
        QWidget#widget_8,
        QWidget#widget_9,
        QWidget#widget_10,
        QWidget#widget_11,
        QWidget#widget_12,
        QWidget#widget_13,
        QWidget#widget_14,
        QWidget#widget_15 {
            background-color: #2a3038;   /* 比背景亮一档 */
            border-radius: 14px;
            border: 1px solid #2f3540;
        }
    )")
    .arg(TEXT_PRIMARY)
    .arg(TEXT_SECONDARY)
    .arg(TEXT_WEAK)
    .arg(ACCENT_COLOR)
    );

    /* =================================================
     * 2. 滚动区设置
     * ================================================= */
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /* =================================================
     * 3. 卡片 Hover 阴影 + 高亮
     * ================================================= */
    QList<QWidget*> cards = {
        ui->widget_3, ui->widget_4, ui->widget_5,
        ui->widget_6, ui->widget_7, ui->widget_8,
        ui->widget_9, ui->widget_10, ui->widget_11,
        ui->widget_12, ui->widget_13, ui->widget_14,
        ui->widget_15
    };

    for (QWidget *card : cards) {
        if (!card) continue;

        card->setMinimumHeight(140);
        card->setContentsMargins(16, 16, 16, 16);

        auto *shadow = new QGraphicsDropShadowEffect(card);
        shadow->setBlurRadius(20);
        shadow->setOffset(0, 6);
        shadow->setColor(QColor(79, 140, 255, 110));
        shadow->setEnabled(false);

        card->setGraphicsEffect(shadow);
        card->setProperty("shadow", QVariant::fromValue<void*>(shadow));
        card->installEventFilter(this);
    }
}

/* =================================================
 * 4. 卡片 Hover 行为
 * ================================================= */
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *card = qobject_cast<QWidget*>(obj);
    if (!card) return false;

    auto *shadow = static_cast<QGraphicsDropShadowEffect*>(
        card->property("shadow").value<void*>()
    );

    if (event->type() == QEvent::Enter) {
        if (shadow) shadow->setEnabled(true);
        card->setStyleSheet(QString(
            "border: 1px solid %1; background-color: #303642;"
        ).arg(ACCENT_COLOR));
    }

    if (event->type() == QEvent::Leave) {
        if (shadow) shadow->setEnabled(false);
        card->setStyleSheet("");
    }

    return false;
}

void MainWindow::updateGameButtons(const QStringList& gameNames)
{
    qDebug() << "更新游戏按钮，游戏列表：" << gameNames;

    // 默认的游戏按钮列表（按UI顺序）
    QList<QPushButton*> gameButtons = {
        ui->pushButton,      // 第一个按钮
        ui->pushButton_3,    // 第二个按钮
        ui->pushButton_4,    // 第三个按钮
        ui->pushButton_5     // 第四个按钮
    };

    // 默认的图标映射（根据你的资源文件）
    QMap<QString, QString> iconMap = {
        {"我的世界", ":/icons/Mc.png"},
        {"三角洲行动", ":/icons/Three.png"},
        {"无畏契约", ":/icons/Wa.png"},
        {"CS2", ":/icons/cs"},
        {"绝地求生", ":/icons/PUBG.png"},
        {"英雄联盟", ":/icons/lol.png"},
        {"黑神话悟空", ":/icons/wukong.png"},
        {"Wallpaper", ":/icons/wallpaper_engine.png"},
        {"Apex", ":/icons/apex.1.png"},
        {"单机", ":/icons/computer.png"},
        {"moba", ":/icons/pvp.png"},
        {"Fps", ":/icons/-zshejilei.png"}
    };

    // 默认的文本映射（用于显示）
    QMap<QString, QString> displayTextMap = {
        {"我的世界", "我的世界"},
        {"三角洲行动", "三角洲行动"},
        {"无畏契约", "无畏契约"},
        {"CS2", "CS2"},
        {"绝地求生", "绝地求生"},
        {"英雄联盟", "英雄联盟"},
        {"黑神话悟空", "黑神话悟空"},
        {"Wallpaper", "Wallpaper"},
        {"Apex", "Apex"},
        {"单机", "单机"},
        {"moba", "moba"},
        {"Fps", "Fps"}
    };

    // 更新按钮
    for (int i = 0; i < gameButtons.size(); i++) {
        QPushButton* button = gameButtons[i];

        if (i < gameNames.size()) {
            QString gameName = gameNames[i];
            QString displayText = displayTextMap.value(gameName, gameName);
            QString iconPath = iconMap.value(gameName, "");

            // 设置按钮文本
            button->setText(displayText);

            // 设置按钮图标
            if (!iconPath.isEmpty()) {
                button->setIcon(QIcon(iconPath));
                button->setIconSize(QSize(25, 25));
            }

            // 显示按钮
            button->show();
        } else {
            // 如果没有足够的游戏，隐藏多余的按钮
            button->hide();
        }
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
