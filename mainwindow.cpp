#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QScrollBar>
#include <QEvent>
#include <QDebug>
#include "logindialog.h"
#include "message_recieve.h"
#include "focus.h"
#include "newaccount.h"
#include "page_profile.h"
#include "message_recieve.h"
#include <QMessageBox>
#include "usermanager.h"
#include "articlemodel.h"
#include "searchresultwidget.h"
#include "gamedetailwidget.h"
#include <QTimer>
#include <QMouseEvent>
#include <QIcon>
#include <QPushButton>
namespace {
    // ================== 统一主色 ==================
    const QString ACCENT_COLOR = "#4f8cff";

    // ================== 文字分层颜色 ==================
    const QString TEXT_PRIMARY   = "#e8eaed";  // 主文本
    const QString TEXT_SECONDARY = "#b0b6bf";  // 次级文本
    const QString TEXT_WEAK      = "#8b949e";  // 弱文本
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isShowingProfile(false)
    , profilePage(nullptr)
    , searchResultWidget(nullptr)
    , searchTimer(nullptr)
    , hotPageWidget(nullptr)

{
    ui->setupUi(this);
    this->setWindowTitle("小白盒 - 你的游戏助手");
    this->setWindowIcon(QIcon(":/icons/gamer.png"));
    this->installEventFilter(this);
    // 初始化游戏列表
    allGames << "我的世界" << "三角洲行动" << "无畏契约" << "CS2"
             << "绝地求生" << "英雄联盟" << "黑神话悟空" << "Wallpaper"
             << "Apex" << "单机" << "moba" << "Fps";

        // 初始化示例评论列表
    allComments << "《黑神话悟空》的打击感真的绝了！"
                << "最近在玩无畏契约，有没有一起组队的？"
                << "CS2的枪械手感比CSGO好太多了"
                << "英雄联盟新赛季改动真大，还在适应中"
                << "我的世界新版本更新了什么内容？"
                << "绝地求生的新地图值得体验吗？"
                << "三角洲行动公测体验分享"
                << "Apex新英雄技能太强了"
                << "Wallpaper Engine的创意工坊真有意思"
                << "单机游戏的剧情太感人了";
    // 连接个人中心按钮
    QPushButton *personalBtn = ui->pushButton_2;  // 使用ui指针访问
    connect(personalBtn, &QPushButton::clicked, this, &MainWindow::onPersonalCenterClicked);

       QList<QPushButton*> gameButtons = {
           ui->pushButton,      // 绝地求生
           ui->pushButton_3,    // 无畏契约
           ui->pushButton_4,    // CS2
           ui->pushButton_5     // 英雄联盟
       };

       // 为每个游戏按钮设置对应的游戏名称并连接点击事件
       QStringList buttonGames = {"绝地求生", "无畏契约", "CS2", "英雄联盟"};

       for (int i = 0; i < gameButtons.size(); i++) {
           QPushButton* button = gameButtons[i];
           if (i < buttonGames.size()) {
               // 存储游戏名称到按钮的属性中
               button->setProperty("gameName", buttonGames[i]);

               // 连接点击事件
               connect(button, &QPushButton::clicked, this, &MainWindow::onGameButtonClicked);
           }
       }

    // 搜索相关连接
       QLineEdit *searchEdit = ui->lineEdit;
       QPushButton *searchBtn = ui->pushButton_7;


       // 搜索按钮点击
       connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearchButtonClicked);
       connect(ui->pushButton_9, &QPushButton::clicked, this, &MainWindow::onHotButtonClicked);
        connect(ui->pushButton_10, &QPushButton::clicked, this, &MainWindow::onHomeButtonClicked);
       // 搜索框按下回车键搜索
       connect(searchEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearchButtonClicked);

       // 搜索框焦点变化 - 移除编辑完成的隐藏，改用点击外部关闭
       // connect(searchEdit, &QLineEdit::editingFinished, this, &MainWindow::hideSearchResults);

       // 设置搜索框属性，使其获得焦点时不失去输入能力
       searchEdit->setFocusPolicy(Qt::StrongFocus);

       // 创建搜索定时器
       searchTimer = new QTimer(this);
       searchTimer->setSingleShot(true);
       searchTimer->setInterval(300); // 300ms延迟搜索
       // 连接定时器，用于清空搜索结果（当输入框清空时）
           connect(searchTimer, &QTimer::timeout,this, [this](){
               QString keyword = ui->lineEdit->text().trimmed();
               if (keyword.isEmpty()) {
                   hideSearchResults();
               }
           });

           // 连接搜索框文本变化，只用于清空结果
           connect(searchEdit, &QLineEdit::textChanged,this, [this](const QString& text){
               if (text.trimmed().isEmpty()) {
                   searchTimer->start();  // 延迟清空搜索结果
               }
           });
    if(ArticleModel::instance()->initDatabase()){
            loadArticlesToCards();
        }


    /* =================================================
     * 1. 全局样式（方案一：文字分层）
     * ================================================= */
    this->setStyleSheet(QString(R"(
        * {
            font-family: "Microsoft YaHei";
            font-size: 14px;
            color: %1;   /* 主文本 */
        }

        QMainWindow {
            background-color: #1e2228;
        }

        /* ================= 顶部 ================= */
        QWidget#widget {
            background-color: #252a31;
            border-bottom: 1px solid #2f3540;
        }

        QLabel {
            color: %1;
        }

        QLabel[secondary="true"] {
            color: %2;
        }

        QLabel[weak="true"] {
            color: %3;
        }

        /* ================= 搜索框 ================= */
        QLineEdit#lineEdit {
            background-color: #1e2228;
            border-radius: 18px;
            padding: 6px 14px;
            border: 1px solid #2f3540;
            color: %1;
        }

        QLineEdit#lineEdit::placeholder {
            color: %3;
        }

        QLineEdit#lineEdit:focus {
            border: 1px solid %4;
            background-color: #1b1f24;
        }

        /* ================= 顶部分类按钮 ================= */
        QPushButton {
            background-color: transparent;
            border: none;
            padding: 6px 14px;
            border-radius: 16px;
            color: %2;
        }

        QPushButton:hover {
            background-color: #2f3540;
            color: %1;
        }

        /* ================= 左侧栏 ================= */
        QWidget#widget_2 {
            background-color: #1b1f24;
            border-right: 1px solid #2f3540;
        }

        QWidget#widget_2 QPushButton {
            text-align: left;
            padding: 10px 16px;
            border-radius: 8px;
            color: %2;
        }

        QWidget#widget_2 QPushButton:hover {
            background-color: #2a2f38;
            color: %1;
        }

        /* ================= 滚动区 ================= */
        QScrollArea {
            background-color: #2a2f38;   /* 灰色背景 */
            border-radius: 12px;        /* 倒角 */
            border: 1px solid #2f3540;  /* 边框 */
        }
        /* 非常重要：确保滚动区内部的视口部件背景透明 */
        QScrollArea > QWidget > QWidget {
         background-color: transparent;
        }

        /* 确保滚动区内部的内容部件不会继承错误样式 */
        QScrollArea QWidget {
        background-color: transparent;
        }
        QScrollBar:vertical {
            width: 6px;
            background: transparent;
        }

        QScrollBar::handle:vertical {
            background: #3b4250;
            border-radius: 3px;
        }

        QScrollBar::handle:vertical:hover {
            background: %4;
        }

        /* ================= 内容卡片 ================= */
        QWidget#widget_3,
        QWidget#widget_4,
        QWidget#widget_5,
        QWidget#widget_6,
        QWidget#widget_7,
        QWidget#widget_8,
        QWidget#widget_9,
        QWidget#widget_10,
        QWidget#widget_11,
        QWidget#widget_12,
        QWidget#widget_13,
        QWidget#widget_14,
        QWidget#widget_15 {
            background-color: #2a3038;   /* 比背景亮一档 */
            border-radius: 14px;
            border: none;
        }
    )")
    .arg(TEXT_PRIMARY)
    .arg(TEXT_SECONDARY)
    .arg(TEXT_WEAK)
    .arg(ACCENT_COLOR)
    );

    /* =================================================
     * 2. 滚动区设置
     * ================================================= */
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /* =================================================
     * 3. 卡片 Hover 阴影 + 高亮
     * ================================================= */
    QList<QWidget*> cards = {
        ui->widget_3, ui->widget_4, ui->widget_5,
        ui->widget_6, ui->widget_7, ui->widget_8,
        ui->widget_9, ui->widget_10, ui->widget_11,
        ui->widget_12, ui->widget_13, ui->widget_14,
        ui->widget_15
    };

    for (QWidget *card : cards) {
        if (!card) continue;

        card->setMinimumHeight(140);
        card->setContentsMargins(16, 16, 16, 16);

        auto *shadow = new QGraphicsDropShadowEffect(card);
        shadow->setBlurRadius(20);
        shadow->setOffset(0, 6);
        shadow->setColor(QColor(79, 140, 255, 110));
        shadow->setEnabled(false);

        card->setGraphicsEffect(shadow);
        card->setProperty("shadow", QVariant::fromValue<void*>(shadow));
        card->installEventFilter(this);
    }
}

/* =================================================
 * 4. 卡片 Hover 行为
 * ================================================= */
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // 1. 尝试将 obj 转换为 QWidget，并检查是否有 articleId 属性
    QWidget *card = qobject_cast<QWidget*>(obj);

    // 只要有 articleId 属性，就说明这是我们要处理的文章卡片
    if (card && card->property("articleId").isValid()) {

        // --- 处理悬停样式 ---
        auto *shadow = static_cast<QGraphicsDropShadowEffect*>(
            card->property("shadow").value<void*>()
        );

        if (event->type() == QEvent::Enter) {
            if (shadow) shadow->setEnabled(true);
            // 这里去掉了之前你代码里的 .arg(ACCENT_COLOR)，因为现在 border 是 none
            card->setStyleSheet("border: none; background-color: #303642;");
        }
        else if (event->type() == QEvent::Leave) {
            if (shadow) shadow->setEnabled(false);
            card->setStyleSheet(""); // 恢复 QSS 定义的默认样式
        }

        // --- 处理点击跳转 (重点修改处) ---
        // 使用 MouseButtonRelease 确保点击动作完成
        else if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {

                // 检查点击位置是否在卡片范围内
                if (card->rect().contains(mouseEvent->pos())) {
                    int articleId = card->property("articleId").toInt();
                    qDebug() << "成功捕获点击事件，跳转文章 ID:" << articleId;

                    // 调用跳转函数
                    this->openArticleDetail(articleId);
                    return true; // 事件消费，不再向下传递
                }
            }
        }
    }

    // 处理搜索结果框点击外部关闭逻辑（保持你原有的逻辑）
    if (obj == this && event->type() == QEvent::MouseButtonPress && searchResultWidget && searchResultWidget->isVisible()) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (!ui->lineEdit->geometry().contains(mouseEvent->pos()) &&
            !searchResultWidget->geometry().contains(mouseEvent->pos())) {
            hideSearchResults();
        }
    }

    return QMainWindow::eventFilter(obj, event);
}
QStringList MainWindow::searchComments(const QString& keyword)
{
    QStringList results;

    for (const QString& comment : qAsConst(allComments)) {
        if (comment.contains(keyword, Qt::CaseInsensitive)) {
            results.append(comment);
        }
    }

    // 限制最大数量
    if (results.size() > 10) {
        results = results.mid(0, 10);
    }

    return results;
}
void MainWindow::openArticleDetail(int articleId)
{
    // 1. 从数据库中获取文章详情
    Article article = ArticleModel::instance()->getArticleById(articleId);

    // 安全检查：如果没查到数据则返回
    if (article.id == -1) {
        qDebug() << "错误：找不到 ID 为" << articleId << "的文章";
        return;
    }

    // 2. 创建或使用详情显示窗口
    // 这里我们先创建一个带滚动条的对话框来显示内容
    QDialog *detailDlg = new QDialog(this);
    detailDlg->setWindowTitle(article.title);
    detailDlg->resize(800, 600);
    detailDlg->setStyleSheet("background-color: #ffffff;"); // 详情页通常用白底黑字，方便阅读

    QVBoxLayout *layout = new QVBoxLayout(detailDlg);

    // 标题
    QLabel *titleLabel = new QLabel(article.title);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333333; padding: 10px;");
    titleLabel->setWordWrap(true);
    layout->addWidget(titleLabel);

    // 副标题（作者和时间）
    QLabel *infoLabel = new QLabel(QString("作者: %1  |  发布于: %2")
                                   .arg(article.author)
                                   .arg(article.publishTime.toString("yyyy-MM-dd")));
    infoLabel->setStyleSheet("color: #666666; font-size: 12px; padding-left: 10px;");
    layout->addWidget(infoLabel);

    // 分割线
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #eeeeee;");
    layout->addWidget(line);

    // 正文内容（使用 QTextEdit 或 QLabel 支持富文本）
    QTextEdit *contentText = new QTextEdit();
    contentText->setReadOnly(true);
    contentText->setFrameStyle(QFrame::NoFrame);
    // 支持 HTML（如果你的内容里有 HTML 标签）
    contentText->setHtml(article.content);
    contentText->setStyleSheet("font-size: 16px; color: #444444; line-height: 1.6; padding: 10px;");
    layout->addWidget(contentText);

    // 底部关闭按钮
    QPushButton *closeBtn = new QPushButton("返回");
    closeBtn->setFixedWidth(100);
    closeBtn->setStyleSheet("background-color: #4f8cff; color: white; height: 30px; border-radius: 4px;");
    connect(closeBtn, &QPushButton::clicked, detailDlg, &QDialog::accept);
    layout->addWidget(closeBtn, 0, Qt::AlignRight);

    detailDlg->exec(); // 以模态方式打开
    delete detailDlg;  // 关闭后销毁
}
void MainWindow::showSearchResults(const QStringList& commentResults)
{
    if (!searchResultWidget) {
        searchResultWidget = new SearchResultWidget(this);

        // 设置位置（在搜索框下方）
        QLineEdit *searchEdit = ui->lineEdit;
        QPoint pos = searchEdit->mapTo(this, QPoint(0, searchEdit->height() + 5));
        searchResultWidget->move(pos);
        searchResultWidget->resize(searchEdit->width() + 50, 400);

        // 连接信号
        connect(searchResultWidget, &SearchResultWidget::commentSelected, this,[this](const QString& comment) {
            qDebug() << "选择了评论:" << comment;
            ui->lineEdit->clear();
            hideSearchResults();
        });

        connect(searchResultWidget, &SearchResultWidget::clearSearchRequested, this,[this]() {
            ui->lineEdit->clear();
            hideSearchResults();
        });
    }

    // 更新位置
    QLineEdit *searchEdit = ui->lineEdit;
    QPoint pos = searchEdit->mapTo(this, QPoint(0, searchEdit->height() + 5));
    searchResultWidget->move(pos);
    searchResultWidget->resize(searchEdit->width() + 50, 400);

    // 更新结果
    searchResultWidget->updateResults( commentResults);

    // 只有当有结果时才显示
    if (!commentResults.isEmpty()) {
        searchResultWidget->show();
        searchResultWidget->raise();
        searchResultWidget->activateWindow();
    } else {
        // 没有结果时不显示窗口，或者显示"无结果"的提示
        searchResultWidget->hide();
        // 可以在这里添加一个提示，比如状态栏消息
        ui->statusbar->showMessage("未找到相关评论", 2000);
    }
}

void MainWindow::hideSearchResults()
{
    if (searchResultWidget) {
        searchResultWidget->hide();
    }
}
void MainWindow::onSearchButtonClicked()
{
    QString keyword = ui->lineEdit->text().trimmed();
    if (!keyword.isEmpty()) {
        QStringList commentResults = searchComments(keyword);
        showSearchResults(commentResults);
    } else {
        hideSearchResults();
    }
}

void MainWindow::onPersonalCenterClicked()
{
    qDebug() << "点击个人中心按钮，执行逻辑切换...";

    // --- 核心清场：必须隐藏热点页 ---
    if (hotPageWidget) {
        hotPageWidget->hide();
    }

    if (!isShowingProfile) {
        // --- 切换到个人中心 ---
        ui->scrollArea->hide(); // 隐藏首页

        if (!profilePage) {
            // 1. 实例化外设页面
            profilePage = new PageProfile(this);
            profilePage->setGeometry(ui->scrollArea->geometry());

            // 2. 【核心修复】：在这里打通信号总线
            // 将个人中心转发出的“打开收藏文章”信号，连接到 MainWindow 现有的详情打开函数
            connect(profilePage, &PageProfile::openFavoriteDetail,
                    this, &MainWindow::openArticleDetail);

            qDebug() << "已建立个人中心信号连接 (openFavoriteDetail -> openArticleDetail)";
        }

        profilePage->show();
        profilePage->raise();

        ui->pushButton_2->setText("返回主页");
        isShowingProfile = true;
    } else {
        // --- 从个人中心返回主页 ---
        if (profilePage) profilePage->hide();
        ui->scrollArea->show();
        ui->pushButton_2->setText("个人中心");
        isShowingProfile = false;
    }
}
void MainWindow::updateGameButtons(const QStringList& gameNames)
{
    qDebug() << "更新游戏按钮，游戏列表：" << gameNames;

    // 默认的游戏按钮列表（按UI顺序）
    QList<QPushButton*> gameButtons = {
        ui->pushButton,      // 第一个按钮
        ui->pushButton_3,    // 第二个按钮
        ui->pushButton_4,    // 第三个按钮
        ui->pushButton_5     // 第四个按钮
    };

    // 默认的图标映射（根据你的资源文件）
    QMap<QString, QString> iconMap = {
        {"我的世界", ":/icons/Mc.png"},
        {"三角洲行动", ":/icons/Three.png"},
        {"无畏契约", ":/icons/Wa.png"},
        {"CS2", ":/icons/cs"},
        {"绝地求生", ":/icons/PUBG.png"},
        {"英雄联盟", ":/icons/lol.png"},
        {"黑神话悟空", ":/icons/wukong.png"},
        {"Wallpaper", ":/icons/wallpaper_engine.png"},
        {"Apex", ":/icons/apex.1.png"},
        {"单机", ":/icons/computer.png"},
        {"moba", ":/icons/pvp.png"},
        {"Fps", ":/icons/-zshejilei.png"}
    };

    // 默认的文本映射（用于显示）
    QMap<QString, QString> displayTextMap = {
        {"我的世界", "我的世界"},
        {"三角洲行动", "三角洲行动"},
        {"无畏契约", "无畏契约"},
        {"CS2", "CS2"},
        {"绝地求生", "绝地求生"},
        {"英雄联盟", "英雄联盟"},
        {"黑神话悟空", "黑神话悟空"},
        {"Wallpaper", "Wallpaper"},
        {"Apex", "Apex"},
        {"单机", "单机"},
        {"moba", "moba"},
        {"Fps", "Fps"}
    };

    // 更新按钮
    for (int i = 0; i < gameButtons.size(); i++) {
        QPushButton* button = gameButtons[i];

        if (i < gameNames.size()) {
            QString gameName = gameNames[i];
            QString displayText = displayTextMap.value(gameName, gameName);
            QString iconPath = iconMap.value(gameName, "");

            // 设置按钮文本
            button->setText(displayText);

            // 设置按钮图标
            if (!iconPath.isEmpty()) {
                button->setIcon(QIcon(iconPath));
                button->setIconSize(QSize(25, 25));
            }

            // 2. 【核心修复】更新逻辑属性！！！
                        // 这样点击时获取到的才是新的游戏名
                        button->setProperty("gameName", gameName);

                        button->show();
        } else {
            // 如果没有足够的游戏，隐藏多余的按钮
            button->hide();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if (searchResultWidget) {
            delete searchResultWidget;
            searchResultWidget = nullptr;
        }
        if (searchTimer) {
            delete searchTimer;
            searchTimer = nullptr;
        }
        // 清理所有游戏详情页
            for (GameDetailWidget* widget : gameDetailWidgets) {
                if (widget) {
                    widget->close();
                    delete widget;
                }
            }
            gameDetailWidgets.clear();
}

void MainWindow::turnToMessage_recieve(){
    message_recieve *message=new message_recieve();
    message->show();

}
void MainWindow::loadArticlesToCards()
{
    // 1. 获取所有文章
    QList<Article> articles = ArticleModel::instance()->getAllArticles();

    // 2. 获取UI上的卡片列表 (与构造函数中保持一致)
    QList<QWidget*> cards = {
        ui->widget_3, ui->widget_4, ui->widget_5,
        ui->widget_6, ui->widget_7, ui->widget_8,
        ui->widget_9, ui->widget_10, ui->widget_11,
        ui->widget_12, ui->widget_13, ui->widget_14,
        ui->widget_15
    };

    // 3. 遍历卡片进行填充
    for (int i = 0; i < cards.size(); i++) {
        QWidget* card = cards[i];

        if (i < articles.size()) {
            // 如果有对应的文章数据，显示卡片并渲染内容
            card->show();
            setupCardUi(card, articles[i]);
        } else {
            // 如果文章数量不足以填满卡片，隐藏多余的卡片
            // 或者你可以选择显示“虚位以待”的空状态
            card->hide();
        }
    }
}

/* =================================================
 * 新增功能：渲染单个卡片的内容 (布局 + 数据)
 * ================================================= */
void MainWindow::setupCardUi(QWidget* card, const Article& article)
{
    // 1. 清理旧布局 (防止刷新时重叠)
    if (card->layout()) {
        QLayoutItem* item;
        while ((item = card->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete card->layout();
    }

    // 2. 创建水平布局：左边是图片，右边是信息
    QHBoxLayout *mainLayout = new QHBoxLayout(card);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);

    // --- 左侧：封面图 ---
    QLabel *coverLabel = new QLabel(card);
    coverLabel->setFixedSize(160, 90); // 设置封面固定大小
    coverLabel->setStyleSheet("background-color: #3b4250; border-radius: 6px;");
    coverLabel->setScaledContents(true);

    if (!article.coverPath.isEmpty() && QFile::exists(article.coverPath)) {
        QPixmap pixmap(article.coverPath);
        coverLabel->setPixmap(pixmap);
    } else {
        // 如果没有封面，显示默认占位符或文字
        coverLabel->setText("No Cover");
        coverLabel->setAlignment(Qt::AlignCenter);
    }
    mainLayout->addWidget(coverLabel);

    // --- 右侧：文本信息 (标题 + 摘要 + 数据) ---
    QVBoxLayout *infoLayout = new QVBoxLayout();

    // 标题
    QLabel *titleLabel = new QLabel(article.title, card);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #e8eaed;");
    titleLabel->setWordWrap(true);
    infoLayout->addWidget(titleLabel);

    // 摘要 (截取前50个字符)
    QString summary = article.getSummary(50);
    QLabel *summaryLabel = new QLabel(summary, card);
    summaryLabel->setStyleSheet("font-size: 12px; color: #b0b6bf;"); // 使用你的 TEXT_SECONDARY 颜色
    summaryLabel->setWordWrap(true);
    summaryLabel->setAlignment(Qt::AlignTop);
    infoLayout->addWidget(summaryLabel);

    // 底部元数据 (作者 | 浏览量 | 时间)
    QHBoxLayout *metaLayout = new QHBoxLayout();
    QString metaText = QString("%1  •  👀 %2  •  %3")
                           .arg(article.author)
                           .arg(article.viewCount)
                           .arg(article.publishTime.toString("MM-dd"));

    QLabel *metaLabel = new QLabel(metaText, card);
    metaLabel->setStyleSheet("font-size: 11px; color: #8b949e;"); // 使用你的 TEXT_WEAK 颜色
    metaLayout->addWidget(metaLabel);
    metaLayout->addStretch(); // 弹簧，将文字推到左边
    QPushButton *btnFav = new QPushButton(card);
        btnFav->setText("❤ 收藏");
        btnFav->setFixedSize(65, 24);
        btnFav->setCursor(Qt::PointingHandCursor);

        // 按钮样式：符合你主色的半透明感
        btnFav->setStyleSheet(R"(
            QPushButton {
                background-color: #3b4250;
                border: 1px solid #4f8cff;
                border-radius: 4px;
                color: #4f8cff;
                font-size: 11px;
            }
            QPushButton:hover {
                background-color: #4f8cff;
                color: white;
            }
        )");

        // 【关键】：连接点击信号
        // 使用 [=] 捕获当前 article.id
        connect(btnFav, &QPushButton::clicked, this, [this, article]() {
            this->onFavoriteClicked(article.id);
        });

        metaLayout->addWidget(btnFav);

        // 将布局组合起来
        infoLayout->addLayout(metaLayout);
        mainLayout->addLayout(infoLayout);
    mainLayout->setStretch(0, 0); // 图片不拉伸
    mainLayout->setStretch(1, 1); // 文字区拉伸
    // 在 setupCardUi 函数末尾添加：
    card->setCursor(Qt::PointingHandCursor); // 变成小手形状，提醒用户可以点击
    card->setProperty("articleId", article.id); // 绑定ID以便点击时识别

}
void MainWindow::onFavoriteClicked(int articleId)
{
    // 1. 获取当前用户
    int currentUserId = UserManager::instance()->userId();
    if (currentUserId == -1) {
        // 未登录的弹窗也应用黑色样式
        QMessageBox msg(QMessageBox::Warning, "未登录", "请先登录后再操作！", QMessageBox::Ok, this);
        msg.setStyleSheet("QMessageBox{background-color:#2a3038;} QLabel{color:white;} QPushButton{background-color:#4f8cff; color:white; border-radius:4px; padding:5px 15px;}");
        msg.exec();
        return;
    }

    // 2. 状态判断
    bool alreadyFaved = ArticleModel::instance()->isFavorite(currentUserId, articleId);

    // 准备通用的黑色样式表
    // 增加 QLabel#qt_msgbox_label 提高优先级，强制背景透明，防止出现白块
    QString darkStyle = R"(
        QMessageBox {
            background-color: #2a3038;
        }
        QLabel {
            color: #ffffff;
            background-color: transparent;
            font-size: 14px;
        }
        QPushButton {
            background-color: #4f8cff;
            color: white;
            border-radius: 4px;
            padding: 6px 20px;
            min-width: 70px;
        }
        QPushButton:hover {
            background-color: #6da1ff;
        }
    )";

    bool success = false;
    QString title, text;

    if (alreadyFaved) {
        // 执行取消收藏
        if (ArticleModel::instance()->removeFavorite(currentUserId, articleId)) {
            success = true;
            title = "已取消";
            text = "文章已从收藏夹移除。";
        }
    } else {
        // 执行添加收藏
        if (ArticleModel::instance()->addFavorite(currentUserId, articleId)) {
            success = true;
            title = "收藏成功";
            text = "文章已成功添加到收藏夹！";
        }
    }

    // 3. 统一显示黑色弹窗
    if (success) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(title);
        msgBox.setText(text);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);

        // 关键点：强制应用样式
        msgBox.setStyleSheet(darkStyle);

        // 针对某些复杂主题，强制让内部文本容器变色
        QLabel *label = msgBox.findChild<QLabel*>("qt_msgbox_label");
        if (label) {
            label->setAlignment(Qt::AlignCenter);
        }

        msgBox.exec();
    }
}
void MainWindow::onGameButtonClicked()
{
    // 1. 获取点击的按钮
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    // 2. 获取该按钮当前绑定的游戏名称（对应 updateGameButtons 中设置的 property）
    QString gameName = clickedButton->property("gameName").toString();

    // 如果属性为空，尝试拿按钮的 text (去掉换行符)
    if (gameName.isEmpty()) {
        gameName = clickedButton->text().replace("\n", " ");
    }

    qDebug() << "检测到点击，准备跳转游戏详情：" << gameName;

    // 3. 执行跳转逻辑
    this->openGameDetail(gameName);
}
void MainWindow::openGameDetail(const QString& gameName)
{
    // 1. 定义数据结构
    struct GameData {
        QString title;
        QString desc;
        QString author;
        QString imagePath;
    };

    // 2. 建立映射表 (根据你的 picture.qrc 结构)
    // 注意：文件名必须与工程资源列表中的大小写、空格完全一致
    QMap<QString, GameData> gameContentMap;
    gameContentMap["黑神话悟空"] = {"黑神话：悟空", "天命人，重走西游路。国产3A大作，开启中式美学新篇章。", "游戏科学", ":/icons/Black Myth.jpg"};
    gameContentMap["无畏契约"] = {"无畏契约 (VALORANT)", "5V5 英雄战术射击游戏，技能与枪法的博弈。", "Riot Games", ":/icons/VALOLANT.jpg"};
    gameContentMap["三角洲行动"] = {"三角洲行动 (Delta Force)", "特种兵战术射击游戏，经典IP回归。", "琳琅天上", ":/icons/Delta Force.jpg"};
    gameContentMap["CS2"] = {"Counter-Strike 2", "全球竞技射击标杆，引擎全面升级。", "Valve", ":/icons/CS2.jpg"};
    gameContentMap["英雄联盟"] = {"英雄联盟 (LOL)", "经典MOBA竞技，新赛季开启全新对线体验。", "Riot Games", ":/icons/lol.jpg"};
    gameContentMap["我的世界"] = {"我的世界 (Minecraft)", "自由探索与创造的方块世界。", "Mojang", ":/icons/minecraft.jpg"};
    gameContentMap["Apex"] = {"Apex 英雄", "快节奏战术竞技，英雄协同作战。", "Respawn", ":/icons/Apex.jpg"};
    gameContentMap["绝地求生"] = {"PUBG", "战术竞技鼻祖，大吉大利今晚吃鸡。", "Krafton", ":/icons/pubg.jpg"};
    gameContentMap["Wallpaper"] = {"Wallpaper Engine", "让你的桌面充满生机。", "Kristjan Skutta", ":/icons/wallpaper.jpg"};

    // 默认数据处理
    GameData data = gameContentMap.value(gameName, {gameName, "暂无详细介绍。", "未知", ""});

    // 3. UI 界面构建
    QDialog *detailDlg = new QDialog(this);
    detailDlg->setWindowTitle(data.title);
    detailDlg->setMinimumSize(850, 650);
    // 采用深色电竞风格
    detailDlg->setStyleSheet("background-color: #121217; color: #e8eaed; font-family: 'Microsoft YaHei UI';");

    QVBoxLayout *mainLayout = new QVBoxLayout(detailDlg);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // --- 顶部大图区 ---
    QLabel *imageLabel = new QLabel();
    imageLabel->setFixedHeight(380); // 固定高度，保持比例
    if (!data.imagePath.isEmpty()) {
        QPixmap pix(data.imagePath);
        if(!pix.isNull()){
            // 使用 KeepAspectRatioByExpanding 填充顶部
            imageLabel->setPixmap(pix.scaled(850, 380, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        } else {
            imageLabel->setText("图片资源加载失败: " + data.imagePath);
        }
    }
    imageLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(imageLabel);

    // --- 下部信息区 ---
    QWidget *infoWidget = new QWidget();
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(35, 25, 35, 25);

    // 标题
    QLabel *titleLabel = new QLabel(data.title);
    titleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #4f8cff;");
    infoLayout->addWidget(titleLabel);

    // 开发商
    QLabel *authorLabel = new QLabel("DEVELOPER: " + data.author);
    authorLabel->setStyleSheet("color: #636e7b; font-size: 14px; font-weight: 600; text-transform: uppercase;");
    infoLayout->addWidget(authorLabel);

    // 装饰线
    QFrame *line = new QFrame();
    line->setFixedHeight(2);
    line->setStyleSheet("background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4f8cff, stop:1 transparent);");
    infoLayout->addWidget(line);
    infoLayout->addSpacing(15);

    // 详情介绍
    QTextEdit *content = new QTextEdit();
    content->setReadOnly(true);
    content->setHtml("<div style='line-height:1.6; font-size: 16px;'>" + data.desc + "</div>");
    content->setStyleSheet("border: none; background: transparent;");
    infoLayout->addWidget(content);

    // 底部返回按钮
    QPushButton *closeBtn = new QPushButton("返回主控台");
    closeBtn->setFixedSize(140, 45);
    closeBtn->setStyleSheet(
        "QPushButton { background-color: #4f8cff; border-radius: 8px; font-weight: bold; font-size: 15px; }"
        "QPushButton:hover { background-color: #6da1ff; transform: translateY(-2px); }"
        "QPushButton:pressed { background-color: #3b6bcc; }"
    );
    connect(closeBtn, &QPushButton::clicked, detailDlg, &QDialog::accept);

    infoLayout->addWidget(closeBtn, 0, Qt::AlignRight);

    mainLayout->addWidget(infoWidget);
    detailDlg->exec();
    delete detailDlg;
}
// ================== 热点页面相关函数 ==================

void MainWindow::setupHotPage()
{
    // 创建热点页面，与主页面使用相同的结构和位置
    hotPageWidget = new QWidget(this);
    hotPageWidget->setGeometry(ui->scrollArea->geometry());

    // 设置背景色与主页面一致
    hotPageWidget->setStyleSheet("background-color: #1e2228;");

    // 创建滚动区域（与首页相同的结构）
    QScrollArea *hotScrollArea = new QScrollArea(hotPageWidget);
    hotScrollArea->setGeometry(0, 0, ui->scrollArea->width(), ui->scrollArea->height());
    hotScrollArea->setWidgetResizable(true);
    hotScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    hotScrollArea->setStyleSheet("QScrollArea { border: none; background-color: transparent; }");

    QWidget *scrollContent = new QWidget();
    scrollContent->setStyleSheet("background-color: #1e2228;");

    QVBoxLayout *mainLayout = new QVBoxLayout(scrollContent);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // 热点页面标题
    QLabel *titleLabel = new QLabel("热门话题", scrollContent);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #e8eaed; margin-bottom: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // 热点话题列表
    QStringList hotTopics = {
        "🔥 《黑神话悟空》销量突破1000万套",
        "🏆 无畏契约世界赛中国战队夺冠",
        "🎮 CS2新地图上线引发玩家热议",
        "⚔️ 英雄联盟S14全球总决赛开幕",
        "🧱 我的世界1.21更新内容预览",
        "🏝️ 绝地求生即将推出全新生存模式",
        "🎯 三角洲行动国服首周玩家破百万",
        "👤 Apex Legends第20赛季新英雄公布",
        "💰 Steam冬季特卖即将开始",
        "🏅 2024年度最佳游戏提名公布",
        "💻 RTX 5090显卡游戏性能实测",
        "✨ 独立游戏《星核》成为年度黑马",
        "🔄 电竞选手转会期开启"
    };

    // 创建热点卡片（使用与首页相似的样式）
    for (const QString &topic : hotTopics) {
        QWidget *card = new QWidget(scrollContent);
        card->setMinimumHeight(140);
        card->setStyleSheet(
            "background-color: #2a3038;"
            "border-radius: 14px;"
            "border: 1px solid #2f3540;"
        );

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(16, 16, 16, 16);
        cardLayout->setSpacing(8);

        // 热点标题
        QLabel *topicLabel = new QLabel(topic, card);
        topicLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #e8eaed;");
        topicLabel->setWordWrap(true);
        cardLayout->addWidget(topicLabel);

        // 热点描述（示例）
        QStringList descriptions = {
            "24小时内讨论量超过50万，玩家热议不断",
            "中国战队首次夺得世界冠军，创造历史",
            "全新地图带来全新战术体验",
            "全球顶尖战队争夺最高荣誉",
            "全新生物、建筑和红石机械",
            "全新的生存挑战，考验玩家极限",
            "首周注册玩家数量突破记录",
            "全新英雄带来全新战术可能",
            "海量游戏折扣，不容错过",
            "年度最佳游戏即将揭晓",
            "新一代显卡带来极致游戏体验",
            "独立开发团队打造的年度惊喜之作",
            "各大俱乐部选手变动频繁"
        };

        int index = &topic - &hotTopics[0];
        if (index >= 0 && index < descriptions.size()) {
            QLabel *descLabel = new QLabel(descriptions[index], card);
            descLabel->setStyleSheet("font-size: 14px; color: #b0b6bf;");
            descLabel->setWordWrap(true);
            cardLayout->addWidget(descLabel);
        }

        // 热度信息
        QHBoxLayout *infoLayout = new QHBoxLayout();
        infoLayout->addStretch();

        QLabel *heatLabel = new QLabel(QString("热度: %1万").arg(50 + index * 8), card);
        heatLabel->setStyleSheet("font-size: 12px; color: #4f8cff;");
        infoLayout->addWidget(heatLabel);

        QLabel *timeLabel = new QLabel(QString("%1小时前").arg(1 + index), card);
        timeLabel->setStyleSheet("font-size: 12px; color: #8b949e; margin-left: 15px;");
        infoLayout->addWidget(timeLabel);

        cardLayout->addLayout(infoLayout);

        mainLayout->addWidget(card);
    }

    // 添加弹簧
    mainLayout->addStretch();

    // 设置滚动区域的内容
    hotScrollArea->setWidget(scrollContent);
}

// ================== 页面切换函数 ==================

void MainWindow::onHomeButtonClicked()
{
    qDebug() << "尝试点回首页...";

    // 1. 必须隐藏热点页面
    if (hotPageWidget) {
        hotPageWidget->hide();
    }

    // 2. 必须隐藏个人中心（如果存在）
    if (profilePage) {
        profilePage->hide();
    }

    // 3. 【最关键的一步】：重新显示首页的滚动区域
    ui->scrollArea->show();

    // 4. 重置相关状态位
    isShowingProfile = false;
    ui->pushButton_2->setText("个人中心");

    qDebug() << "首页已重新显示";
}
void MainWindow::onHotButtonClicked()
{
    qDebug() << "正在进入热点页，执行清场...";

    // --- 清场逻辑 ---
    ui->scrollArea->hide();               // 隐藏首页
    if (profilePage) profilePage->hide(); // 隐藏个人中心
    isShowingProfile = false;             // 重置个人中心状态
    ui->pushButton_2->setText("个人中心"); // 恢复按钮文字

    // --- 执行显示 ---
    if (!hotPageWidget) {
        setupHotPage();
    }
    hotPageWidget->show();
    hotPageWidget->raise(); // 确保它在最前面，防止被其他透明控件遮挡
}
