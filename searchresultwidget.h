#ifndef SEARCHRESULTWIDGET_H
#define SEARCHRESULTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

namespace Ui {
class SearchResultWidget;
}

class SearchResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchResultWidget(QWidget *parent = nullptr);
    ~SearchResultWidget();
    void updateResults( const QStringList& commentResults);
    void clearResults();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void gameSelected(const QString& gameName);
    void commentSelected(const QString& commentId);
    void clearSearchRequested();  // 添加这个信号

private:
    Ui::SearchResultWidget *ui;
    QPoint dragPosition;
};

#endif // SEARCHRESULTWIDGET_H
