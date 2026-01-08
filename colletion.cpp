#include "colletion.h"
#include "ui_colletion.h"
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include "usermanager.h"
#include "page_profile.h"
Colletion::Colletion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Colletion)
{
    ui->setupUi(this);

    // 返回按钮
    connect(ui->btn_back, &QPushButton::clicked,
            this, &Colletion::backClicked);

}

Colletion::~Colletion()
{
    delete ui;
}
void Colletion::loadCollectionData()
{
    // 1. 获取当前用户 ID
    int userId = UserManager::instance()->userId();
    if (userId == -1) return;

    // 2. 清理旧布局（防止重复加载）
    // 假设你的 colletionLayout 最后一个元素是 Spacer
    while (ui->colletionLayout->count() > 1) {
        QLayoutItem *item = ui->colletionLayout->takeAt(0);
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    // 3. 从数据库读取收藏列表
    QList<Article> favs = ArticleModel::instance()->getFavoriteArticles(userId);

    if (favs.isEmpty()) {
        ui->label_empty->show();
    } else {
        ui->label_empty->hide();
        for (const Article &art : favs) {
            // 调用下面的新版添加函数
            addColletionItemEx(art);
        }
    }
}

// 建议写一个扩展版的添加函数，传入完整的 Article 对象
void Colletion::addColletionItemEx(const Article &art)
{
    // 创建卡片容器
    QFrame *item = new QFrame;
    item->setFrameShape(QFrame::StyledPanel);
    item->setCursor(Qt::PointingHandCursor);
    item->setStyleSheet(R"(
        QFrame {
            background: #2a3038;
            border: 1px solid #3b4250;
            border-radius: 8px;
            padding: 5px;
        }
        QFrame:hover { background: #323942; border-color: #4f8cff; }
    )");

    QHBoxLayout *layout = new QHBoxLayout(item); // 改用水平布局更美观

    // 标题
    QLabel *label = new QLabel(art.title);
    label->setStyleSheet("font-size:14px; color: #e8eaed; border: none; font-weight: bold;");
    label->setWordWrap(true);
    layout->addWidget(label, 1);

    // 查看按钮（可选，或者直接让整个卡片可点击）
    QPushButton *btnView = new QPushButton("查看详情");
    btnView->setFixedSize(80, 28);
    btnView->setStyleSheet("background: #4f8cff; color: white; border-radius: 4px; border: none;");
    layout->addWidget(btnView);

    // 【核心逻辑】：点击卡片发出信号，告诉 MainWindow 要看哪篇文章
    // 既然这是个 QWidget 且没写复杂的 EventFilter，最简单的办法是用按钮或子控件的信号
    connect(btnView, &QPushButton::clicked, this, [this, art]() {
        emit articleRequested(art.id);
    });

    // 插入到布局中（Spacer 之前）
    ui->colletionLayout->insertWidget(ui->colletionLayout->count() - 1, item);
}
