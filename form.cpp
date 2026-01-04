#include "form.h"
#include "ui_form.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    this->setWindowTitle("小白盒 - 你的游戏助手");
    this->setWindowIcon(QIcon(":/icons/gamer.png"));
    this->setStyleSheet(R"(
    /* ===== 主窗口 ===== */
    QWidget#Form {
        background: qradialgradient(cx:0.5, cy:0.5, radius: 1,
            fx:0.5, fy:0.5,
            stop:0 rgba(240, 244, 255, 1),
            stop:1 rgba(230, 236, 255, 1));
        border: 3px solid #6C63FF;
        border-radius: 25px;
        box-shadow: 0 15px 35px rgba(108, 99, 255, 0.25);
    }

    /* ===== 标题标签 ===== */
    QLabel#label{
        color: #6C63FF;
        font-size: 24px;
        font-weight: bold;
        font-family: "Microsoft YaHei UI";
        text-shadow: 0 2px 4px rgba(108, 99, 255, 0.2);
        background: transparent;
        border: none;
    }

    /* ===== 游戏标题标签 (我的世界、三角洲行动等) ===== */
    QLabel#label_2,
    QLabel#label_3,
    QLabel#label_4,
    QLabel#label_5,
    QLabel#label_6,
    QLabel#label_7,
    QLabel#label_8,
    QLabel#label_9,
    QLabel#label_10,
    QLabel#label_11,
    QLabel#label_12,
    QLabel#label_13,
    QLabel#label_14 {
        color: #333333;
        font-size: 15px;
        font-weight: bold;
        background: rgba(255, 255, 255, 0.9);
        border: 2px solid transparent;
        border-radius: 12px;
        padding: 8px 15px;
        margin: 3px;
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.05);
    }

    /* ===== 游戏按钮 - 基础样式 ===== */
    QPushButton {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
            stop:0 rgba(255, 255, 255, 0.95),
            stop:1 rgba(245, 247, 255, 0.95));
        border: 3px solid #E0E3FF;
        border-radius: 18px;
        color: #555555;
        font-weight: bold;
        font-size: 14px;
        padding: 15px;
        min-width: 100px;
        min-height: 60px;
        margin: 5px;
        text-align: center;
        transition: all 0.3s cubic-bezier(0.175, 0.885, 0.32, 1.275);
    }

    /* ===== 游戏按钮悬停效果 ===== */
    QPushButton:hover {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
            stop:0 rgba(255, 248, 240, 0.95),
            stop:1 rgba(255, 241, 230, 0.95));
        border: 3px solid #FF6B35;
        color: #FF6B35;
        transform: translateY(-5px);
        box-shadow: 0 12px 20px rgba(255, 107, 53, 0.25);
    }

    /* ===== 游戏按钮按下效果 ===== */
    QPushButton:pressed {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
            stop:0 rgba(255, 241, 230, 0.95),
            stop:1 rgba(255, 233, 220, 0.95));
        border: 3px solid #FF4081;
        color: #FF4081;
        transform: translateY(-2px);
        box-shadow: 0 6px 12px rgba(255, 64, 129, 0.2);
    }

    /* ===== 游戏按钮选中状态 ===== */
    QPushButton:checked {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
            stop:0 rgba(108, 99, 255, 0.15),
            stop:1 rgba(108, 99, 255, 0.1));
        border: 3px solid #6C63FF;
        color: #6C63FF;
        box-shadow: 0 0 20px rgba(108, 99, 255, 0.3);
        font-weight: bold;
    }

    /* ===== 游戏按钮选中状态悬停 ===== */
    QPushButton:checked:hover {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
            stop:0 rgba(108, 99, 255, 0.25),
            stop:1 rgba(108, 99, 255, 0.2));
        border: 3px solid #FF6B35;
        color: #FF6B35;
        transform: translateY(-5px);
    }

    /* ===== 第一排游戏按钮 (我的世界、三角洲行动、无畏契约、CS2) ===== */
    QPushButton#pushButton,   /* 我的世界 */
    QPushButton#pushButton_2, /* 三角洲行动 */
    QPushButton#pushButton_3, /* 无畏契约 */
    QPushButton#pushButton_4  /* CS2 */
    {
        border-top: 5px solid #06D6A0;
    }

    /* ===== 第二排游戏按钮 (TUBO、绝地求生、英雄联盟、黑神话悟空) ===== */
    QPushButton#pushButton_5, /* TUBO */
    QPushButton#pushButton_6, /* 绝地求生 */
    QPushButton#pushButton_7, /* 英雄联盟 */
    QPushButton#pushButton_8  /* 黑神话悟空 */
    {
        border-top: 5px solid #FFD166;
    }

    /* ===== 第三排游戏按钮 (WallPaper、APEX、单机、moba) ===== */
    QPushButton#pushButton_9,  /* WallPaper */
    QPushButton#pushButton_10, /* APEX */
    QPushButton#pushButton_11, /* 单机 */
    QPushButton#pushButton_12  /* moba */
    {
        border-top: 5px solid #FF6B35;
    }

    /* ===== 第四排按钮 (Fps、提交按钮) ===== */
    QPushButton#pushButton_13, /* Fps */
    QPushButton#pushButton_14  /* 假设是提交按钮，如果没有就用这个样式 */
    {
        border-top: 5px solid #6C63FF;
    }

    /* ===== 特殊：Fps按钮额外效果 ===== */
    QPushButton#pushButton_13 {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
            stop:0 rgba(255, 255, 255, 0.95),
            stop:1 rgba(255, 240, 240, 0.95));
    }

    QPushButton#pushButton_13:hover {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
            stop:0 rgba(255, 230, 230, 0.95),
            stop:1 rgba(255, 210, 210, 0.95));
        border: 3px solid #FF3333;
        color: #FF3333;
    }

    /* ===== 底部提示文字 ===== */
    QLabel[objectName^="label_"][text*="最少选择一个"] {
        color: #666666;
        font-size: 13px;
        font-weight: normal;
        background: rgba(108, 99, 255, 0.05);
        border: 1px dashed #6C63FF;
        border-radius: 10px;
        padding: 10px;
        margin-top: 10px;
        font-family: "Microsoft YaHei";
    }

    /* ===== 按钮聚焦效果 ===== */
    QPushButton:focus {
        outline: none;
        border: 3px solid #FF4081;
        box-shadow: 0 0 0 3px rgba(255, 64, 129, 0.2);
    }

    /* ===== 游戏图标区域背景 ===== */
    QWidget[objectName^="widget_"] {
        background: rgba(255, 255, 255, 0.7);
        border-radius: 20px;
        border: 2px solid rgba(108, 99, 255, 0.1);
        margin: 10px;
        padding: 15px;
    }

    /* ===== 动画关键帧（需要通过代码实现） ===== */
    /* 选中时的脉冲动画 */
    @keyframes pulse {
        0% { box-shadow: 0 0 0 0 rgba(108, 99, 255, 0.4); }
        70% { box-shadow: 0 0 0 10px rgba(108, 99, 255, 0); }
        100% { box-shadow: 0 0 0 0 rgba(108, 99, 255, 0); }
    }

    /* ===== 响应式设计 ===== */
    @media (max-width: 800px) {
        QPushButton {
            min-width: 80px;
            min-height: 50px;
            padding: 10px;
            font-size: 12px;
        }
    }

    /* ===== 鼠标手型 ===== */
    QPushButton:hover {
        cursor: pointer;
    }

    /* ===== 禁用的按钮 ===== */
    QPushButton:disabled {
        background: rgba(200, 200, 200, 0.5);
        border: 3px solid #CCCCCC;
        color: #999999;
        transform: none;
        box-shadow: none;
    }
    )");

    // 收集所有游戏按钮
     gameButtons = {
            ui->pushButton,      // 三角洲行动
            ui->pushButton_2,    // 英雄联盟
            ui->pushButton_3,    // Fps
            ui->pushButton_4,    // 单机
            ui->pushButton_5,    // Wallpaper
            ui->pushButton_6,    // CS2
            ui->pushButton_7,    // 无畏契约
            ui->pushButton_8,    // 我的世界
            ui->pushButton_9,    // 绝地求生
            ui->pushButton_10,   // 黑神话悟空
            ui->pushButton_11,   // Apex
            ui->pushButton_12    // moba
        };

        // 设置所有游戏按钮为可选状态
       for (QPushButton* button : gameButtons) {
                button->setCheckable(true);
                button->setChecked(false);
                connect(button, &QPushButton::clicked, this, &Form::onGameButtonClicked);
            }
    // 连接跳过按钮
       connect(ui->pushButton_13, &QPushButton::clicked,
                this, &Form::on_pushButton_13_clicked);
       // 连接确认选择按钮
       connect(ui->pushButton_14, &QPushButton::clicked,
                   this, &Form::on_pushButton_14_clicked);

           // 初始化选中列表
       selectedGames.clear();
}

Form::~Form()
{
    delete ui;
}


void Form::on_pushButton_13_clicked()
{
    // 发出跳过信号
    emit skipClicked();
}

void Form::on_pushButton_14_clicked()
{
    // 检查是否选择了至少一个游戏
    if (selectedGames.isEmpty()) {
        QMessageBox::warning(this, "提示", "请至少选择一个游戏！");
        return;
    }

    // 检查是否选择了超过四个游戏
    if (selectedGames.size() > 4) {
        QMessageBox::warning(this, "提示", "最多只能选择四个游戏！");
        return;
    }

    qDebug() << "已选择的游戏：" << selectedGames;

    // 发出确认信号
    emit confirmed();
    this->close();  // 关闭当前窗口
}

void Form::onGameButtonClicked()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    updateSelection(clickedButton);
}

void Form::updateSelection(QPushButton* button)
{
    QString gameName = button->text();

    if (button->isChecked()) {
        // 如果已经选中了4个，取消选中并提示
        if (selectedGames.size() >= 4) {
            button->setChecked(false);
            QMessageBox::information(this, "提示", "最多只能选择四个游戏！");
            return;
        }

        // 添加到选中列表
        if (!selectedGames.contains(gameName)) {
            selectedGames.append(gameName);
            qDebug() << "添加游戏：" << gameName << "，当前选择数：" << selectedGames.size();
        }
    } else {
        // 从选中列表中移除
        selectedGames.removeAll(gameName);
        qDebug() << "移除游戏：" << gameName << "，当前选择数：" << selectedGames.size();
    }

    // 更新按钮样式
    button->setStyleSheet(button->isChecked() ?
        "background-color: #6C63FF; color: white; font-weight: bold;" :
        "");
}
