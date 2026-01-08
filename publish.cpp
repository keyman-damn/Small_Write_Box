// publish.cpp
#include "publish.h"
#include <QDateTime>
#include <QTimer>
#include "articlemodel.h"
#include "usermanager.h"
Publish::Publish(QWidget *parent) : QWidget(parent)
{
    setupUI();
    setupStyles();
}

Publish::~Publish() {}

void Publish::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // 创建滚动区域
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    scrollWidget = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
    scrollLayout->setContentsMargins(10, 10, 10, 10);
    scrollLayout->setSpacing(15);

    // 1. 标题输入
    QLabel *titleLabel = new QLabel("文章标题");
    titleLabel->setFixedHeight(30);
    titleEdit = new QLineEdit();
    titleEdit->setPlaceholderText("请输入文章标题（1-50字）");
    titleEdit->setMaxLength(50);
    scrollLayout->addWidget(titleLabel);
    scrollLayout->addWidget(titleEdit);

    // 2. 封面选择
    coverGroup = new QGroupBox("文章封面");
    QVBoxLayout *coverLayout = new QVBoxLayout(coverGroup);

    coverLabel = new QLabel();
    coverLabel->setFixedSize(300, 180);
    coverLabel->setAlignment(Qt::AlignCenter);
    coverLabel->setText("点击选择封面图片\n建议尺寸：800×450");
    coverLabel->setStyleSheet("border: 2px dashed #ccc; border-radius: 8px;");

    coverButton = new QPushButton("选择封面图片");
    coverButton->setFixedWidth(150);

    coverLayout->addWidget(coverLabel, 0, Qt::AlignHCenter);
    coverLayout->addWidget(coverButton, 0, Qt::AlignHCenter);
    scrollLayout->addWidget(coverGroup);

    // 3. 内容编辑
    QLabel *contentLabel = new QLabel("文章内容");
    contentLabel->setFixedHeight(30);
    contentEdit = new QTextEdit();
    contentEdit->setPlaceholderText("请输入文章内容...");
    contentEdit->setMinimumHeight(400);
    scrollLayout->addWidget(contentLabel);
    scrollLayout->addWidget(contentEdit);

    // 4. 标签区域
    tagGroup = new QGroupBox("文章标签");
    QVBoxLayout *tagGroupLayout = new QVBoxLayout(tagGroup);

    QWidget *tagInputWidget = new QWidget();
    QHBoxLayout *tagInputLayout = new QHBoxLayout(tagInputWidget);
    tagInputLayout->setContentsMargins(0, 0, 0, 0);

    tagEdit = new QLineEdit();
    tagEdit->setPlaceholderText("输入标签，按回车或点击添加");
    addTagButton = new QPushButton("添加");
    addTagButton->setFixedWidth(80);

    tagInputLayout->addWidget(tagEdit);
    tagInputLayout->addWidget(addTagButton);

    tagContainer = new QWidget();
    tagLayout = new QHBoxLayout(tagContainer);
    tagLayout->setContentsMargins(0, 10, 0, 0);
    tagLayout->setSpacing(8);

    tagGroupLayout->addWidget(tagInputWidget);
    tagGroupLayout->addWidget(tagContainer);
    scrollLayout->addWidget(tagGroup);

    // 5. 发布设置
    settingsGroup = new QGroupBox("发布设置");
    QGridLayout *settingsLayout = new QGridLayout(settingsGroup);

    QLabel *categoryLabel = new QLabel("文章分类:");
    categoryCombo = new QComboBox();
    categoryCombo->addItems({"游戏资讯", "硬件评测", "电竞动态", "玩家心得", "业界分析"});

    originalCheck = new QCheckBox("原创文章");
    originalCheck->setChecked(true);

    allowCommentCheck = new QCheckBox("允许评论");
    allowCommentCheck->setChecked(true);

    immediatePublishCheck = new QCheckBox("立即发布");
    immediatePublishCheck->setChecked(true);

    settingsLayout->addWidget(categoryLabel, 0, 0);
    settingsLayout->addWidget(categoryCombo, 0, 1);
    settingsLayout->addWidget(originalCheck, 1, 0, 1, 2);
    settingsLayout->addWidget(allowCommentCheck, 2, 0, 1, 2);
    settingsLayout->addWidget(immediatePublishCheck, 3, 0, 1, 2);

    scrollLayout->addWidget(settingsGroup);

    // 6. 按钮区域
    buttonFrame = new QFrame();
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);

    publishButton = new QPushButton("发布文章");
    publishButton->setFixedSize(120, 40);

    saveDraftButton = new QPushButton("保存草稿");
    saveDraftButton->setFixedSize(100, 35);

    previewButton = new QPushButton("预览");
    previewButton->setFixedSize(80, 35);

    buttonLayout->addStretch();
    buttonLayout->addWidget(saveDraftButton);
    buttonLayout->addWidget(previewButton);
    buttonLayout->addWidget(publishButton);

    scrollLayout->addWidget(buttonFrame);
    scrollLayout->addStretch();

    // 7. 状态标签
    statusLabel = new QLabel();
    statusLabel->setFixedHeight(20);
    scrollLayout->addWidget(statusLabel);

    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    // 连接信号槽
    connect(coverButton, &QPushButton::clicked, this, &Publish::onCoverButtonClicked);
    connect(publishButton, &QPushButton::clicked, this, &Publish::onPublishButtonClicked);
    connect(saveDraftButton, &QPushButton::clicked, this, &Publish::onSaveDraftButtonClicked);
    connect(addTagButton, &QPushButton::clicked, this, &Publish::onAddTagButtonClicked);
    connect(tagEdit, &QLineEdit::returnPressed, this, &Publish::onAddTagButtonClicked);
}

void Publish::setupStyles()
{
    // 先设置按钮的objectName
    publishButton->setObjectName("publishButton");
    saveDraftButton->setObjectName("saveDraftButton");
    previewButton->setObjectName("previewButton");

    // 设置全局样式
    this->setStyleSheet(R"(
        /* 主界面背景 - 统一的米白色 */
        Publish, QWidget#publishWindow, QMainWindow {
            background-color: #f8f4e9;
        }

        /* 所有QWidget继承米白色背景 */
        QWidget {
            font-family: "Microsoft YaHei", "Segoe UI";
            background-color: #f8f4e9;
        }

        /* === 文章标签区域特殊处理 === */
        /* 确保标签输入区域背景为白色 */
        #tagsContainer, #tagsWidget,
        QFrame[objectName*="tag"],
        QWidget[objectName*="tag"],
        .tags-area, .tag-input-area {
            background-color: white;
            border: 1px solid #e0e0e0;
            border-radius: 4px;
        }

        /* 标签输入框 */
        QLineEdit[objectName*="tag"],
        QLineEdit#tagInput {
            background-color: white;
            border: none;
            padding: 6px 8px;
        }

        /* 标签按钮容器 */
        QWidget#tagButtonsContainer,
        QWidget#tagButtonsWidget {
            background-color: white;
            border: none;
        }

        /* 单个标签样式 */
        QLabel[objectName*="tagLabel"],
        QPushButton[objectName*="tagButton"] {
            background-color: #eef5ff;
            color: #409EFF;
            border: 1px solid #c6e2ff;
            border-radius: 3px;
            padding: 2px 8px;
            margin: 2px;
        }

        /* 添加标签按钮 */
        QPushButton#addTagButton {
            background-color: #f5f5f5;
            color: #333;
            border: 1px solid #d9d9d9;
            border-radius: 3px;
            padding: 4px 12px;
        }

        QPushButton#addTagButton:hover {
            background-color: #e8e8e8;
            border-color: #bfbfbf;
        }

        /* === 通用样式 === */
        /* 所有分组框、框架都使用白色背景 */
        QFrame, QGroupBox, QScrollArea,
        QScrollArea > QWidget > QWidget,
        .content-area, .settings-area {
            background-color: white;
            border: 1px solid #e0e0e0;
            border-radius: 6px;
        }

        /* 无边框的容器也设为白色 */
        QFrame[frameShape="0"],
        QWidget#contentWidget {
            background-color: white;
            border: none;
            border-radius: 6px;
        }

        /* 输入框、文本编辑框等 */
        QLineEdit, QTextEdit, QComboBox, QPlainTextEdit {
            font-size: 14px;
            padding: 8px;
            border: 1px solid #e0e0e0;
            border-radius: 4px;
            background-color: white;
        }

        QLineEdit:focus, QTextEdit:focus,
        QComboBox:focus, QPlainTextEdit:focus {
            border-color: #409EFF;
            background-color: white;
        }

        QLabel {
            font-size: 14px;
            color: #333;
            background-color: transparent;
        }

        /* 分组框 */
        QGroupBox {
            font-size: 15px;
            font-weight: bold;
            background-color: white;
            border: 1px solid #e0e0e0;
            border-radius: 6px;
            margin-top: 10px;
            padding-top: 15px;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
            color: #333;
            background-color: white;
        }

        /* 按钮基础样式 */
        QPushButton {
            font-size: 14px;
            padding: 8px 16px;
            border-radius: 4px;
            border: 1px solid #dcdfe6;
            min-width: 80px;
            background-color: #f5f5f5;
        }

        /* 发布按钮 - 黑色背景白色字体 */
        QPushButton#publishButton {
            background-color: #000000;
            color: white;
            font-weight: bold;
            border: 1px solid #000000;
        }

        QPushButton#publishButton:hover {
            background-color: #333333;
            border-color: #333333;
        }

        QPushButton#publishButton:pressed {
            background-color: #000000;
            border-color: #000000;
        }

        /* 草稿按钮 */
        QPushButton#saveDraftButton {
            background-color: #f5f5f5;
            color: #000000;
            border: 1px solid #d0d0d0;
        }

        QPushButton#saveDraftButton:hover {
            background-color: #e8e8e8;
            border-color: #b0b0b0;
        }

        QPushButton#saveDraftButton:pressed {
            background-color: #d9d9d9;
            border-color: #b0b0b0;
        }

        /* 预览按钮 */
        QPushButton#previewButton {
            background-color: #f5f5f5;
            color: #000000;
            border: 1px solid #d0d0d0;
        }

        QPushButton#previewButton:hover {
            background-color: #e8e8e8;
            border-color: #b0b0b0;
        }

        QPushButton#previewButton:pressed {
            background-color: #d9d9d9;
            border-color: #b0b0b0;
        }

        /* 复选框 */
        QCheckBox {
            font-size: 14px;
            spacing: 8px;
            background-color: white;
        }

        QCheckBox::indicator {
            width: 16px;
            height: 16px;
        }

        /* 分割线 */
        QFrame[frameShape="4"], QFrame#line {
            background-color: #e0e0e0;
            border: none;
            max-height: 1px;
            min-height: 1px;
        }

        /* 滚动区域 */
        QScrollArea {
            border: none;
            background-color: white;
        }
    )");

    // 确保样式立即应用
    this->update();
}
void Publish::onCoverButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "选择封面图片",
        "",
        "图片文件 (*.jpg *.jpeg *.png *.bmp)");

    if (!fileName.isEmpty()) {
        coverPath = fileName;
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) {
            QPixmap scaledPixmap = pixmap.scaled(coverLabel->size(),
                Qt::KeepAspectRatio, Qt::SmoothTransformation);
            coverLabel->setPixmap(scaledPixmap);
            coverLabel->setText("");
            statusLabel->setText("封面已选择: " + QFileInfo(fileName).fileName());
            statusLabel->setStyleSheet("color: #67C23A;");
        }
    }
}

void Publish::onSaveDraftButtonClicked()
{
    if (titleEdit->text().isEmpty() && contentEdit->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "警告", "没有内容可保存！");
        return;
    }

    // 模拟保存草稿
    statusLabel->setText("草稿已保存 " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    statusLabel->setStyleSheet("color: #E6A23C;");

    QTimer::singleShot(2000, [this]() {
        statusLabel->clear();
    });
}

void Publish::onAddTagButtonClicked()
{
    QString tag = tagEdit->text().trimmed();
    if (!tag.isEmpty()) {
        tags.append(tag);

        // 创建标签按钮
        QPushButton *tagButton = new QPushButton(tag + " ×");
        tagButton->setStyleSheet(R"(
            QPushButton {
                background-color: #ecf5ff;
                color: #409EFF;
                border: 1px solid #d9ecff;
                padding: 4px 8px;
                border-radius: 12px;
                font-size: 12px;
            }
            QPushButton:hover {
                background-color: #d9ecff;
            }
        )");

        tagLayout->addWidget(tagButton);

        // 点击标签可删除
        connect(tagButton, &QPushButton::clicked, [this, tag, tagButton]() {
            tags.removeAll(tag);
            tagLayout->removeWidget(tagButton);
            tagButton->deleteLater();
        });

        tagEdit->clear();
    }
}

void Publish::onPublishButtonClicked()
{
    validateInput();

    if (titleEdit->text().isEmpty() || contentEdit->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写标题和内容！");
        return;
    }

    // 获取ArticleModel实例
       ArticleModel* model = ArticleModel::instance();
       if (!model) {
           QMessageBox::critical(this, "错误", "数据库未初始化！");
           return;
       }

       // 创建文章对象
       Article article;
       article.title = titleEdit->text();
       article.content = contentEdit->toHtml();  // 保存为HTML格式
       article.coverPath = coverPath;

       // 获取标签列表
       QStringList tagList;
       for (int i = 0; i < tagLayout->count(); ++i) {
           QLayoutItem* item = tagLayout->itemAt(i);
           if (item && item->widget()) {
               QPushButton* tagButton = qobject_cast<QPushButton*>(item->widget());
               if (tagButton) {
                   QString tagText = tagButton->text();
                   tagText = tagText.replace(" ×", "");  // 移除删除符号
                   tagList.append(tagText);
               }
           }
       }
       article.setTagsFromList(tagList);

       article.category = categoryCombo->currentText();
       article.author = UserManager::instance()->userName();
       article.userId = UserManager::instance()->userId();
       article.isOriginal = originalCheck->isChecked();
       article.allowComments = allowCommentCheck->isChecked();

       // 模拟发布过程
       publishButton->setEnabled(false);
       statusLabel->setText("正在发布...");
       statusLabel->setStyleSheet("color: #409EFF;");

       QTimer::singleShot(1500, [this, model, article]() mutable {
           // 保存到数据库
           int articleId = model->addArticle(article);

           if (articleId > 0) {
               QMessageBox::information(this, "成功",
                   QString("文章发布成功！\n文章ID: %1").arg(articleId));
               statusLabel->setText("发布于 " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));
               statusLabel->setStyleSheet("color: #67C23A;");

               // 清空表单
               clearForm();

               // 发送信号通知其他页面
               emit articlePublished();
           } else {
               QMessageBox::critical(this, "错误", "文章保存到数据库失败！");
               statusLabel->setText("发布失败");
               statusLabel->setStyleSheet("color: #F56C6C;");
           }
           publishButton->setEnabled(true);
       });
}
void Publish::clearForm()
{
    // 清空标题
    titleEdit->clear();

    // 清空内容
    contentEdit->clear();

    // 重置封面
    if (coverLabel) {
        coverLabel->setText("点击选择封面图片\n建议尺寸：800×450");
        coverLabel->setPixmap(QPixmap());
        coverLabel->setStyleSheet("border: 2px dashed #ccc; border-radius: 8px;");
    }
    coverPath.clear();

    // 清空标签
    clearTags();

    // 重置其他控件
    if (categoryCombo) {
        categoryCombo->setCurrentIndex(0);
    }
    if (originalCheck) {
        originalCheck->setChecked(true);
    }
    if (allowCommentCheck) {
        allowCommentCheck->setChecked(true);
    }
    if (immediatePublishCheck) {
        immediatePublishCheck->setChecked(true);
    }

    // 清空标签编辑框
    if (tagEdit) {
        tagEdit->clear();
    }
}
void Publish::clearTags()
{
    // 清空标签容器
    if (tagLayout) {
        QLayoutItem* item;
        while ((item = tagLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
    }

    // 清空标签列表
    tags.clear();
}

void Publish::validateInput()
{
    // 验证标题长度
    if (titleEdit->text().length() > 50) {
        titleEdit->setStyleSheet("border: 2px solid #F56C6C; border-radius: 4px;");
        QMessageBox::warning(this, "警告", "标题不能超过50字！");
        return;
    } else {
        titleEdit->setStyleSheet("");
    }

    // 验证内容长度
    if (contentEdit->toPlainText().length() < 10) {
        QMessageBox::warning(this, "警告", "内容太短，至少需要10个字！");
        return;
    }
}
