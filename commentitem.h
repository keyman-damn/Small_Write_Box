#ifndef COMMENTITEM_H
#define COMMENTITEM_H

#include <QWidget>

namespace Ui {
class CommentItem;
}

class CommentItem : public QWidget
{
    Q_OBJECT

signals:
    void approved();  // 通过审核信号
    void rejected();  // 拒绝审核信号

public:
    explicit CommentItem(QWidget *parent = nullptr);
    ~CommentItem();

    void setData(const QString &user, const QString &content);
private:
    Ui::CommentItem *ui;
};

#endif // COMMENTITEM_H
