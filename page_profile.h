#ifndef PAGE_PROFILE_H
#define PAGE_PROFILE_H

#include <QWidget>

namespace Ui {
class page_profile;
}

class PageProfile : public QWidget
{
    Q_OBJECT

public:
    explicit PageProfile(QWidget *parent = nullptr);
    ~PageProfile();
    void OnPublishClicked();
    void OnLogoutClicked();
signals:
    void needRefreshMain(); // 告诉 MainWindow 需要刷新
   void articleRequested(int id); // 信号：请求主窗口打开文章详情
   void openFavoriteDetail(int id);
private:
    Ui::page_profile *ui;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void showArticles();
    void loadFavorites(); // 负责去数据库读取并渲染卡片
   void on_btnFavorites_clicked();
};
#endif // PAGE_PROFILE_H
