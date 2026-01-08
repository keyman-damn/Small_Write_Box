#include "page_profile.h"
#include "ui_page_profile.h"
#include "publish.h"
#include "articlemodel.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QScrollArea>
#include "usermanager.h"
#include "logindialog.h"
#include "colletion.h"
PageProfile::PageProfile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::page_profile)
{
    ui->setupUi(this);
    showArticles();
    connect(ui->btnPublish,&QPushButton::clicked,this,&PageProfile::OnPublishClicked);
    connect(ui->btnLogout, &QPushButton::clicked, this, &PageProfile::OnLogoutClicked);
    connect(ui->btnFavorites, &QPushButton::clicked, this, &PageProfile::on_btnFavorites_clicked);

}

PageProfile::~PageProfile()
{
    delete ui;
}

void PageProfile::showArticles()
{
    // 1. 确保 listArticles 存在
    if (!ui->listArticles) {
        qDebug() << "错误: listArticles 控件不存在";
        return;
    }

    if (ui->listArticles->layout()) {
            QLayoutItem* item;
            while ((item = ui->listArticles->layout()->takeAt(0)) != nullptr) {
                if (item->widget()) delete item->widget();
                delete item;
            }
        } else {
            new QVBoxLayout(ui->listArticles);
        }

        // 3. 【权限判定】检查当前是否登录
        int currentUserId = UserManager::instance()->userId();
        QString currentUserName = UserManager::instance()->userName();
        if (currentUserId == -1) {
                // --- 情况 A：未登录 ---
                ui->labelName->setText("访客");
                ui->btnLogout->setText("登录账号");
                // 建议给登录按钮一个醒目的颜色（比如蓝色）
                ui->btnLogout->setStyleSheet("background-color: #4f8cff; color: white; border-radius: 4px; padding: 5px;");

                // 显示占位提示
                QLabel* guestLabel = new QLabel("登录后即可查看和管理您的文章");
                guestLabel->setAlignment(Qt::AlignCenter);
                guestLabel->setStyleSheet("color: #8b949e; font-size: 14px; padding: 50px;");
                ui->listArticles->layout()->addWidget(guestLabel);

                return; // 未登录状态下，处理完按钮后在此返回
            } else {
                // --- 情况 B：已登录 ---
                ui->labelName->setText(currentUserName);
                ui->btnLogout->setText("退出登录");
                // 退出按钮通常使用警告色（比如红色或灰色）
                ui->btnLogout->setStyleSheet("background-color: #ff4d4f; color: white; border-radius: 4px; padding: 5px;");
            }
        // 4. 只获取当前登录用户的文章（不要调用 getAllArticles）
        ArticleModel* model = ArticleModel::instance();
        QList<Article> articles = model->getArticlesByUserId(currentUserId);

        qDebug() << "用户" << currentUserId << "加载文章数量：" << articles.size();
    // 5. 显示文章
    if (articles.isEmpty()) {
        // 没有文章时显示提示
        QLabel* emptyLabel = new QLabel("还没有发布过文章\n点击上方按钮发布第一篇文章吧！");
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet(R"(
            font-size: 14px;
            color: #999;
            padding: 40px;
            line-height: 1.5;
        )");
        ui->listArticles->layout()->addWidget(emptyLabel);
    } else {
        // 有文章，显示文章列表

        // 创建滚动区域，以便显示多篇文章
        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        // 滚动区域的内容窗口
        QWidget* scrollContent = new QWidget();
        QVBoxLayout* contentLayout = new QVBoxLayout(scrollContent);
        contentLayout->setContentsMargins(0, 0, 0, 0);
        contentLayout->setSpacing(12);

        // 添加每篇文章
        for (const Article& article : articles) {
            // 创建文章项框架
            QFrame* articleFrame = new QFrame();
            articleFrame->setFrameShape(QFrame::StyledPanel);
            articleFrame->setProperty("articleId", article.id);

                // 【新增修改 2】：设置手型光标，告诉用户这里可以点
                articleFrame->setCursor(Qt::PointingHandCursor);

                // 【新增修改 3】：安装过滤器，让本类（PageProfile）监控这个 Frame 的动作
                articleFrame->installEventFilter(this);
            articleFrame->setStyleSheet(R"(
                QFrame {
                    background-color: white;
                    border: 1px solid #e0e0e0;
                    border-radius: 8px;
                    padding: 15px;
                }
                QFrame:hover {
                    background-color: #f9f9f9;
                    border-color: #ccc;
                }
            )");

            // 文章内容的垂直布局
            QVBoxLayout* articleLayout = new QVBoxLayout(articleFrame);
            articleLayout->setSpacing(8);

            // 文章标题
            QLabel* titleLabel = new QLabel(article.title);
            titleLabel->setStyleSheet(R"(
                font-size: 16px;
                font-weight: bold;
                color: #333;
            )");
            titleLabel->setWordWrap(true);

            // 文章摘要（移除HTML标签）
            QString summary = article.content;
            // 简单移除HTML标签
            summary = summary.remove(QRegExp("<[^>]*>")).simplified();
            if (summary.length() > 100) {
                summary = summary.left(100) + "...";
            }

            QLabel* summaryLabel = new QLabel(summary);
            summaryLabel->setStyleSheet(R"(
                font-size: 13px;
                color: #666;
                line-height: 1.4;
            )");
            summaryLabel->setWordWrap(true);

            // 文章信息行
            QHBoxLayout* infoLayout = new QHBoxLayout();

            // 分类
            QLabel* categoryLabel = new QLabel(article.category);
            categoryLabel->setStyleSheet(R"(
                font-size: 12px;
                color: #fff;
                background-color: #409EFF;
                padding: 2px 8px;
                border-radius: 10px;
            )");

            // 发布时间
            QLabel* timeLabel = new QLabel(article.publishTime.toString("yyyy-MM-dd HH:mm"));
            timeLabel->setStyleSheet(R"(
                font-size: 12px;
                color: #999;
            )");

            // 阅读量
            QLabel* viewsLabel = new QLabel(QString("阅读: %1").arg(article.viewCount));
            viewsLabel->setStyleSheet(R"(
                font-size: 12px;
                color: #999;
            )");

            infoLayout->addWidget(categoryLabel);
            infoLayout->addStretch();
            infoLayout->addWidget(timeLabel);
            infoLayout->addWidget(viewsLabel);

            // 添加到文章布局
            articleLayout->addWidget(titleLabel);
            articleLayout->addWidget(summaryLabel);
            articleLayout->addLayout(infoLayout);

            // 添加到内容布局
            contentLayout->addWidget(articleFrame);
        }

        // 添加弹性空间
        contentLayout->addStretch();

        // 设置滚动区域的内容
        scrollArea->setWidget(scrollContent);

        // 将滚动区域添加到listArticles
        ui->listArticles->layout()->addWidget(scrollArea);

        // 更新文章数量显示（如果UI中有显示文章数量的标签）
        // 这里需要根据你的实际UI调整
        // 例如：ui->labelArticleCount->setText(QString::number(articles.size()));
    }

    // 6. 确保布局生效
    ui->listArticles->updateGeometry();
}

void PageProfile::OnPublishClicked()
{
    // 1. 权限拦截：判断是否未登录
    if (UserManager::instance()->userId() == -1) {
        // 1. 实例化 QMessageBox 对象，而不是直接调用静态函数
        QMessageBox msgBox(QMessageBox::Question, "提示", "发布文章需要先登录，是否现在去登录？",
                           QMessageBox::Yes | QMessageBox::No, this);

        // 2. 【核心修复】设置样式表，覆盖掉 MainWindow 的全局白色文字
        // 将文字设为黑色 (#000000) 或深灰色 (#333333)
        msgBox.setStyleSheet("QLabel{ color: #333333; font-size: 19px; } "
                             "QPushButton{ color: #333333; background-color: #f0f0f0; padding: 5px 10px; }");

        // 3. 运行并获取结果
        int reply = msgBox.exec();

        if (reply == QMessageBox::Yes) {
            LoginDialog loginDlg(this);
            if (loginDlg.exec() == QDialog::Accepted) {
                QMessageBox::information(this, "成功", "登录成功！现在可以发布文章了。");
                showArticles();
            }
        }
        return; // 拦截结束
    }
    // --- 以下是已登录状态下的正常流程 ---
    Publish *publish = new Publish();

    // 建立刷新信号链
    connect(publish, &Publish::articlePublished, this, &PageProfile::showArticles);
    connect(publish, &Publish::articlePublished, this, &PageProfile::needRefreshMain);

    publish->show();
}
void PageProfile::OnLogoutClicked() {
    int currentUserId = UserManager::instance()->userId();

    if (currentUserId == -1) {
        // --- 未登录：去登录 ---
        LoginDialog loginDlg(this);
        if (loginDlg.exec() == QDialog::Accepted) {

            // 【关键修改】：登录成功的提示框也要改样式
            QMessageBox successBox(QMessageBox::Information, "成功", "登录成功！现在可以发布文章了。", QMessageBox::Ok, this);
            successBox.setStyleSheet("QLabel{ color: #333333; } QPushButton{ color: #333333; }");
            successBox.exec();

            this->showArticles();
            emit needRefreshMain();
        }
    } else {
        // --- 已登录：去注销 ---
        // 这里的询问框也要改样式
        QMessageBox askBox(QMessageBox::Question, "注销", "确定要退出当前账号并切换吗？",
                           QMessageBox::Yes | QMessageBox::No, this);
        askBox.setStyleSheet("QLabel{ color: #333333; } QPushButton{ color: #333333; }");

        if (askBox.exec() == QMessageBox::Yes) {
            UserManager::instance()->logout();

            LoginDialog loginDlg(this);
            if (loginDlg.exec() == QDialog::Accepted) {
                // 这里的切换成功提示框也要改样式
                QMessageBox switchBox(QMessageBox::Information, "成功", "已成功切换账号！", QMessageBox::Ok, this);
                switchBox.setStyleSheet("QLabel{ color: #333333; } QPushButton{ color: #333333; }");
                switchBox.exec();

                this->showArticles();
            } else {
                this->showArticles();
            }
            emit needRefreshMain();
        }
    }
}
void PageProfile::on_btnFavorites_clicked() {
    // 权限校验
    if (UserManager::instance()->userId() == -1) {
        // ... 提示登录 ...
        return;
    }

    Colletion *dialog = new Colletion(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->loadCollectionData();

    // 【关键】：信号中转
    // 监听弹窗发出的信号，PageProfile 接着发出自己的信号传递给 MainWindow
    // 注意：这里的 openFavoriteDetail 是专门给收藏夹起的名字，防止和首页信号冲突
    connect(dialog, &Colletion::articleRequested, this, [this](int id){
        emit openFavoriteDetail(id);
    });

    dialog->exec();
}
bool PageProfile::eventFilter(QObject *obj, QEvent *event)
{
    // 判断产生事件的对象是否是我们的文章卡片 (QFrame)
    QFrame *frame = qobject_cast<QFrame*>(obj);

    // 如果是 QFrame 且带有名为 "articleId" 的属性
    if (frame && frame->property("articleId").isValid()) {
        // 监听鼠标左键释放动作
        if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {

                // 获取之前绑定的 ID
                int articleId = frame->property("articleId").toInt();
                qDebug() << "用户点击了发布的文章卡片，准备打开详情。ID:" << articleId;

                // 【核心跳转】：发出信号给 MainWindow
                // 这里建议复用 openFavoriteDetail 信号，
                // 这样你在 MainWindow 里就不用写重复的槽函数了
                emit openFavoriteDetail(articleId);

                return true; // 表示事件已处理，不再向下传递
            }
        }
    }
    // 其他事件交给父类默认处理
    return QWidget::eventFilter(obj, event);
}
