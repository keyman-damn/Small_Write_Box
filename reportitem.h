#ifndef REPORTITEM_H
#define REPORTITEM_H

#include <QWidget>

namespace Ui {
class ReportItem;
}

class ReportItem : public QWidget
{
    Q_OBJECT

public:
    explicit ReportItem(QWidget *parent = nullptr);
    ~ReportItem();

    // 设置举报数据（根据你的UI设计）
     void setReportData(const QString &userName,      // 举报人用户名
                           const QString &reason,        // 举报原因
                           const QString &content);      // 被举报内容

private:
    Ui::ReportItem *ui;
};

#endif // REPORTITEM_H
