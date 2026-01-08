#include "searchresultwidget.h"
#include "ui_searchresultwidget.h"
#include <QListWidgetItem>
#include <QDebug>
#include <QMouseEvent>

SearchResultWidget::SearchResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchResultWidget)
{
    ui->setupUi(this);
    // 设置窗口属性为无边框工具窗口，支持拖动
        this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);

        // 连接信号槽
        connect(ui->commentListWidget, &QListWidget::itemClicked, [this](QListWidgetItem *item) {
            emit commentSelected(item->text());
        });

        // 连接退出按钮
        connect(ui->closeButton, &QPushButton::clicked, [this]() {
            this->hide();
            emit clearSearchRequested();
        });
}

SearchResultWidget::~SearchResultWidget()
{
    delete ui;
}

void SearchResultWidget::updateResults( const QStringList& commentResults)
{
    // 清空现有结果
    ui->commentListWidget->clear();

    // 只显示评论结果
    for (const QString& comment : commentResults) {
        QListWidgetItem *item = new QListWidgetItem(comment);
        ui->commentListWidget->addItem(item);
    }

    // 使用 QString::number() 避免链式 arg() 调用
    ui->resultCountLabel->setText("共找到 " + QString::number(commentResults.size()) + " 条评论");

    // 如果没有结果，显示提示
    if (commentResults.isEmpty()) {
        ui->commentSectionLabel->setText("相关评论 (无结果)");
    } else {
        ui->commentSectionLabel->setText("相关评论 (" + QString::number(commentResults.size()) + "条)");
    }
}

void SearchResultWidget::clearResults()
{
    ui->commentListWidget->clear();
        ui->commentSectionLabel->setText("相关评论");
        ui->resultCountLabel->setText("共找到 0 条评论");
}

// 添加鼠标事件处理以实现拖动
void SearchResultWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void SearchResultWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void SearchResultWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        event->accept();
    }
}
