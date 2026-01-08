#ifndef COLLETION_H
#define COLLETION_H

#include <QDialog> // 1. 包含对话框头文件
#include "articlemodel.h"
namespace Ui {
class Colletion;
}

class Colletion : public QDialog
{
    Q_OBJECT

public:
    explicit Colletion(QWidget *parent = nullptr);
    ~Colletion();
    void loadCollectionData();     // 新增：加载数据库数据的方法
    // 添加收藏项
   void addColletionItemEx(const Article &art);
signals:
    void backClicked();   // 返回信号
    void articleRequested(int id); // 点击某篇文章时发出的信号

private:
    Ui::Colletion *ui;
};

#endif // COLLETION_H
