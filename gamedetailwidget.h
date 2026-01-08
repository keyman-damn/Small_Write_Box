#ifndef GAMEDETAILWIDGET_H
#define GAMEDETAILWIDGET_H

#include <QWidget>
#include <QString>


namespace Ui {
class GameDetailWidget;
}

class GameDetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameDetailWidget(const QString& gameName,QWidget *parent = nullptr);
    ~GameDetailWidget();

private:
    Ui::GameDetailWidget *ui;
    // 添加方法根据游戏名称加载数据
    void loadGameData(const QString& gameName);
    void updatePixmapSize(const QPixmap &pixmap);  // 新增辅助函数
    void displayOptimizedImage(const QString& imagePath);  // 新增：优化图片显示
    // 新增：设置窗口图标的方法
    void setWindowIconByGameName(const QString& gameName);
    // 存储当前游戏名称
    QString currentGameName;

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GAMEDETAILWIDGET_H
