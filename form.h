#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVector>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

    // 添加获取选中游戏的方法
     QStringList getSelectedGames() const { return selectedGames; }
signals:
    // 定义跳过信号
    void skipClicked();
    // 添加确认选择信号
       void confirmed();

private slots:
    void on_pushButton_13_clicked();  // 假设 pushButton_13是跳过按钮
    void on_pushButton_14_clicked();  // 确认选择按钮
    void onGameButtonClicked();       // 游戏按钮点击处理

private:
    Ui::Form *ui;
    QStringList selectedGames;        // 存储选中的游戏
    QList<QPushButton*> gameButtons;  // 存储所有游戏按钮
    void updateSelection(QPushButton* button); // 更新选中状态

};

#endif // FORM_H
