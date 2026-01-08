#include "commentitem.h"
#include "ui_commentitem.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>

CommentItem::CommentItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommentItem)
{
    ui->setupUi(this);
    qDebug() << "=== 开始创建CommentItem ===";

       // ============================================
       // 关键步骤1：删除UI文件中的固定geometry
       // ============================================
       // 删除所有控件的固定geometry，让布局管理器控制位置
       ui->label->setGeometry(QRect());        // 清除评论内容的固定位置
       ui->userName->setGeometry(QRect());     // 清除用户名的固定位置
       ui->pushButton->setGeometry(QRect());   // 清除通过按钮的固定位置
       ui->pushButton_2->setGeometry(QRect()); // 清除拒绝按钮的固定位置
       ui->label_3->setGeometry(QRect());      // 清除头像的固定位置

       // ============================================
       // 关键步骤2：清除可能存在的旧布局
       // ============================================
       if (this->layout()) {
           qDebug() << "删除旧布局";
           delete this->layout();  // 删除UI文件中可能设置的布局
       }

       // ============================================
       // 关键步骤3：设置大小策略
       // ============================================
       // 设置整个CommentItem的大小策略
       // Expanding: 水平可以扩展，Fixed: 垂直高度由内容决定
       setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

       // ============================================
       // 关键步骤4：检查控件状态
       // ============================================
       qDebug() << "检查控件状态:";
       qDebug() << "  userName exists:" << (ui->userName != nullptr);
       qDebug() << "  label exists:" << (ui->label != nullptr);
       qDebug() << "  pushButton exists:" << (ui->pushButton != nullptr);
       qDebug() << "  pushButton_2 exists:" << (ui->pushButton_2 != nullptr);
       qDebug() << "  label_3 (avatar) exists:" << (ui->label_3 != nullptr);

       // ============================================
       // 关键步骤5：创建新的布局
       // ============================================
       // 1. 主垂直布局
       QVBoxLayout *mainLayout = new QVBoxLayout(this);
       mainLayout->setSpacing(12);
       mainLayout->setContentsMargins(15, 15, 15, 15);

       // 2. 用户信息行（水平布局）
       QHBoxLayout *userInfoLayout = new QHBoxLayout();

       // 头像
       if (ui->label_3) {
           ui->label_3->setFixedSize(40, 40);  // 固定头像大小
           ui->label_3->setScaledContents(true); // 缩放图片以适应
           userInfoLayout->addWidget(ui->label_3);
       }

       // 用户名
       if (ui->userName) {
           ui->userName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
           ui->userName->setStyleSheet("font-weight: bold; font-size: 14px; color: #2c3e50;");
           userInfoLayout->addWidget(ui->userName);
       }

       userInfoLayout->addStretch();  // 将用户信息推到左边
       mainLayout->addLayout(userInfoLayout);

       // 3. 评论内容
       if (ui->label) {
           // 重要：设置内容标签的属性
           ui->label->setWordWrap(true);  // 允许换行
           ui->label->setAlignment(Qt::AlignLeft | Qt::AlignTop);  // 左上对齐
           ui->label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);

           // 设置样式
           ui->label->setStyleSheet(R"(
               QLabel {
                   font-size: 13px;
                   color: #34495e;
                   background-color: #f8f9fa;
                   border-left: 4px solid #3498db;
                   padding: 10px;
                   border-radius: 4px;
                   margin-top: 5px;
                   margin-bottom: 10px;
               }
           )");

           mainLayout->addWidget(ui->label);
       }

       // 4. 按钮行（水平布局）
       QHBoxLayout *buttonLayout = new QHBoxLayout();
       buttonLayout->addStretch();  // 将按钮推到右边

       // 通过按钮
       if (ui->pushButton) {
           ui->pushButton->setFixedSize(100, 36);
           ui->pushButton->setStyleSheet(R"(
               QPushButton {
                   background-color: #27ae60;
                   color: white;
                   border: none;
                   border-radius: 6px;
                   font-weight: bold;
                   padding: 8px;
                   font-size: 13px;
               }
               QPushButton:hover {
                   background-color: #229954;
               }
               QPushButton:pressed {
                   background-color: #1e8449;
               }
           )");
           buttonLayout->addWidget(ui->pushButton);

           // 连接信号
           connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
               QString user = ui->userName ? ui->userName->text() : "未知用户";
               qDebug() << "✅ 通过评论 - 用户:" << user;
               emit approved();  // 如果需要，可以发射信号
           });
       }

       // 拒绝按钮
       if (ui->pushButton_2) {
           ui->pushButton_2->setFixedSize(100, 36);
           ui->pushButton_2->setStyleSheet(R"(
               QPushButton {
                   background-color: #e74c3c;
                   color: white;
                   border: none;
                   border-radius: 6px;
                   font-weight: bold;
                   padding: 8px;
                   font-size: 13px;
               }
               QPushButton:hover {
                   background-color: #c0392b;
               }
               QPushButton:pressed {
                   background-color: #a93226;
               }
           )");
           buttonLayout->addWidget(ui->pushButton_2);

           // 连接信号
           connect(ui->pushButton_2, &QPushButton::clicked, this, [this]() {
               QString user = ui->userName ? ui->userName->text() : "未知用户";
               qDebug() << "❌ 拒绝评论 - 用户:" << user;
               emit rejected();  // 如果需要，可以发射信号
           });
       }

       mainLayout->addLayout(buttonLayout);

       // ============================================
       // 关键步骤6：设置CommentItem的样式和属性
       // ============================================
       // 设置最小高度
       setMinimumHeight(160);

       // 设置背景和边框
       setStyleSheet(R"(
           CommentItem {
               background-color: white;
               border: 1px solid #dfe6e9;
               border-radius: 10px;
               margin: 8px 0;
           }
           CommentItem:hover {
               border-color: #3498db;
               box-shadow: 0 2px 8px rgba(52, 152, 219, 0.15);
           }
       )");

       // 启用自动填充背景
       setAutoFillBackground(true);

       qDebug() << "=== CommentItem创建完成 ===";
       qDebug() << "初始尺寸:" << size();
       qDebug() << "最小尺寸:" << minimumSize();
}

CommentItem::~CommentItem()
{
    delete ui;
}

void CommentItem::setData(const QString &user, const QString &content)
{
    qDebug() << "设置评论数据 - 用户:" << user;

    // 设置用户名
    if (ui->userName) {
        // 添加用户图标
        ui->userName->setText("👤 " + user);
        qDebug() << "用户名已设置:" << ui->userName->text();
    }

    // 设置评论内容
    if (ui->label) {
        // 确保内容不为空
        QString displayContent = content;
        if (content.isEmpty()) {
            displayContent = "（这条评论没有内容）";
        }

        ui->label->setText(displayContent);
        qDebug() << "评论内容已设置，长度:" << displayContent.length();
        qDebug() << "内容预览:" << displayContent.left(50) << "...";
    }

    // 强制更新布局和尺寸
    updateGeometry();
    adjustSize();

    qDebug() << "setData完成，当前尺寸:" << size();
    qDebug() << "推荐尺寸:" << sizeHint();
}



