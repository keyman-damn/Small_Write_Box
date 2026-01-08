#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include "articlemodel.h"
#include "searchresultwidget.h"
#include "gamedetailwidget.h"
#include "page_profile.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 添加更新游戏按钮的方法
    void updateGameButtons(const QStringList& gameNames);

    void turnToMessage_recieve();
    void onFavoriteClicked(int articleId);
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void openArticleDetail(int articleId);
private slots:
    void onPersonalCenterClicked();  // 添加槽函数
    void onSearchButtonClicked();  // 搜索按钮点击
    void hideSearchResults();  // 隐藏搜索结果
    void onGameButtonClicked();  // 添加游戏按钮点击槽函数
    void onHomeButtonClicked();  // 首页按钮点击
    void onHotButtonClicked();   // 热点按钮点击

private:
    // 搜索相关函数
    QStringList searchComments(const QString& keyword);
    void showSearchResults( const QStringList& commentResults);

    // 打开游戏详情页
    void openGameDetail(const QString& gameName);
    void setupHotPage();

private:
    Ui::MainWindow *ui;

    void loadArticlesToCards(); // 获取数据并分发给卡片
    void setupCardUi(QWidget* card, const Article& article); // 渲染单个卡片的UI
    // 搜索相关函数
    bool isShowingProfile;           // 标记当前是否显示个人中心
    PageProfile *profilePage;        // 个人中心页面指针
    SearchResultWidget *searchResultWidget;  // 搜索结果组件
    QTimer *searchTimer;             // 搜索延迟定时器
    QStringList allGames;            // 所有游戏列表
    QStringList allComments;         // 所有评论列表（示例）
    QVector<GameDetailWidget*> gameDetailWidgets;  // 存储所有游戏详情页
     QWidget *hotPageWidget;  // 热点页面
};
#endif // MAINWINDOW_H
