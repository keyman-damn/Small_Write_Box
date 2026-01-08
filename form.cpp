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
    // 在 Form 构造函数中，替换现有的样式表为以下内容：

    this->setStyleSheet(R"(
        /* ===== 主窗口 ===== */
        QWidget#Form {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(22, 22, 28, 1),
                stop:1 rgba(32, 32, 42, 1));
            border-radius: 20px;
            box-shadow: 0 20px 50px rgba(0, 0, 0, 0.4);
        }

        /* ===== 标题标签 ===== */
        QLabel#label {
            color: #FFFFFF;
            font-size: 20px;
            font-weight: 500;
            font-family: "Microsoft YaHei UI", "Segoe UI";
            background: transparent;
            border: none;
            padding: 10px;
            text-align: center;
        }

        QLabel#label p {
            margin: 8px 0;
        }

        /* ===== 游戏图标区域 ===== */
        QLabel[labelWithIcon] {
            border-radius: 16px;
            background: rgba(40, 40, 50, 0.8);
            border: 1px solid rgba(255, 255, 255, 0.1);
            backdrop-filter: blur(10px);
        }

        /* ===== 游戏按钮容器 ===== */
        QPushButton {
            min-width: 90px;
            min-height: 40px;
            background: rgba(35, 35, 45, 0.9);
            border: 2px solid rgba(255, 255, 255, 0.1);
            border-radius: 16px;
            color: #CCCCCC;
            font-weight: 500;
            font-size: 14px;
            font-family: "Microsoft YaHei UI";
            padding: 15px 10px;
            margin: 8px;
            text-align: center;
            transition: all 0.25s cubic-bezier(0.4, 0, 0.2, 1);
        }

        /* ===== 按钮悬停效果 ===== */
        QPushButton:hover {
            background: rgba(45, 45, 60, 0.95);
            border: 2px solid rgba(100, 150, 255, 0.3);
            color: #FFFFFF;
            transform: translateY(-3px);
            box-shadow: 0 10px 20px rgba(0, 0, 0, 0.3);
        }

        /* ===== 按钮按下效果 ===== */
        QPushButton:pressed {
            background: rgba(55, 55, 70, 0.95);
            border: 2px solid rgba(100, 150, 255, 0.5);
            color: #FFFFFF;
            transform: translateY(-1px);
            box-shadow: 0 5px 15px rgba(0, 0, 0, 0.2);
        }

        /* ===== 按钮选中状态 ===== */
        QPushButton:checked {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(80, 120, 255, 0.9),
                stop:1 rgba(100, 150, 255, 0.9));
            border: 2px solid rgba(150, 180, 255, 0.6);
            color: #FFFFFF;
            font-weight: 600;
            box-shadow: 0 0 0 4px rgba(80, 120, 255, 0.2),
                        0 10px 25px rgba(80, 120, 255, 0.3);
        }

        /* ===== 按钮选中悬停 ===== */
        QPushButton:checked:hover {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(90, 130, 255, 0.95),
                stop:1 rgba(110, 160, 255, 0.95));
            border: 2px solid rgba(170, 200, 255, 0.8);
            transform: translateY(-3px);
        }

        /* ===== 跳过按钮 ===== */
        QPushButton#pushButton_13 {
            background: rgba(45, 45, 55, 0.9);
            border: 2px solid rgba(255, 255, 255, 0.15);
            border-radius: 12px;
            color: #AAAAAA;
            font-weight: 300;padding: 12px 20px;
            min-height: 28px;
            min-width: 50px;
        }

        QPushButton#pushButton_13:hover {
            background: rgba(55, 55, 65, 0.95);
            border: 2px solid rgba(255, 255, 255, 0.25);
            color: #FFFFFF;
        }

        /* ===== 确认选择按钮 ===== */
        QPushButton#pushButton_14 {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(80, 120, 255, 1),
                stop:1 rgba(100, 150, 255, 1));
            border: none;
            border-radius: 14px;
            color: #FFFFFF;
            font-weight: 400;
            font-size: 20px;
            padding: 20px 40px;
            min-height: 40px;
            min-width:100px;
            box-shadow: 0 8px 20px rgba(80, 120, 255, 0.3);
        }

        QPushButton#pushButton_14:hover {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(90, 130, 255, 1),
                stop:1 rgba(110, 160, 255, 1));
            box-shadow: 0 12px 25px rgba(80, 120, 255, 0.4);
            transform: translateY(-2px);
        }

        QPushButton#pushButton_14:pressed {
            transform: translateY(0px);
            box-shadow: 0 5px 15px rgba(80, 120, 255, 0.3);
        }

        /* ===== 按钮禁用状态 ===== */
        QPushButton:disabled {
            background: rgba(30, 30, 35, 0.6);
            border: 2px solid rgba(255, 255, 255, 0.05);
            color: #666666;
            opacity: 0.6;
            transform: none;
            box-shadow: none;
        }

        /* ===== 按钮聚焦效果 ===== */
        QPushButton:focus {
            outline: none;
            border: 2px solid rgba(100, 150, 255, 0.5);
            box-shadow: 0 0 0 3px rgba(100, 150, 255, 0.2);
        }

        /* ===== 游戏按钮分组边框色 ===== */
        /* 第一行 */
        QPushButton#pushButton,
        QPushButton#pushButton_8,
        QPushButton#pushButton_7,
        QPushButton#pushButton_6 {
            border-top: 4px solid rgba(0, 200, 255, 0.3);
        }

        /* 第二行 */
        QPushButton#pushButton_9,
        QPushButton#pushButton_2,
        QPushButton#pushButton_10,
        QPushButton#pushButton_5 {
            border-top: 4px solid rgba(255, 150, 50, 0.3);
        }

        /* 第三行 */
        QPushButton#pushButton_11,
        QPushButton#pushButton_4,
        QPushButton#pushButton_12,
        QPushButton#pushButton_3 {
            border-top: 4px solid rgba(150, 100, 255, 0.3);
        }

        /* ===== 滚动条样式（如果未来需要） ===== */
        QScrollBar:vertical {
            background: rgba(40, 40, 50, 0.5);
            width: 10px;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical {
            background: rgba(100, 100, 120, 0.8);
            border-radius: 5px;
            min-height: 30px;
        }

        QScrollBar::handle:vertical:hover {
            background: rgba(120, 120, 140, 0.9);
        }

        /* ===== 鼠标指针 ===== */
        QPushButton:hover {
            cursor: pointer;
        }

        /* ===== 玻璃态效果 ===== */
        QWidget {
            backdrop-filter: blur(5px);
        }
    )");


    // 为图标标签添加圆角和阴影效果
    QList<QLabel*> iconLabels = {
        ui->label_2, ui->label_3, ui->label_4, ui->label_5,
        ui->label_6, ui->label_7, ui->label_8, ui->label_9,
        ui->label_10, ui->label_11, ui->label_12, ui->label_13
    };

    for (QLabel* label : iconLabels) {
        label->setStyleSheet(label->styleSheet() +
            "border-radius: 12px;"
            "background: rgba(40, 40, 50, 0.8);"
            "border: 1px solid rgba(255, 255, 255, 0.1);"
            "padding: 5px;");
    }

    // 设置标题标签样式
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setContentsMargins(10, 10, 10, 10);

    // 调整按钮文字换行
    for (QPushButton* button : gameButtons) {
        button->setText(button->text().replace(" ", "\n"));
    }

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
