// publish.h
#ifndef PUBLISH_H
#define PUBLISH_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QFrame>
#include <QFileDialog>
#include <QMessageBox>

class Publish : public QWidget
{
    Q_OBJECT

public:
    explicit Publish(QWidget *parent = nullptr);
    ~Publish();
signals:
    void articlePublished();
private slots:
    void onCoverButtonClicked();
    void onPublishButtonClicked();
    void onSaveDraftButtonClicked();
    void onAddTagButtonClicked();

private:
    void setupUI();
    void setupStyles();
    void validateInput();
    void clearForm();
    void clearTags();
    // 主布局
    QVBoxLayout *mainLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;

    // 标题部分
    QLineEdit *titleEdit;

    // 封面部分
    QGroupBox *coverGroup;
    QLabel *coverLabel;
    QPushButton *coverButton;

    // 内容编辑器
    QTextEdit *contentEdit;

    // 标签部分
    QGroupBox *tagGroup;
    QLineEdit *tagEdit;
    QPushButton *addTagButton;
    QWidget *tagContainer;
    QHBoxLayout *tagLayout;

    // 设置部分
    QGroupBox *settingsGroup;
    QComboBox *categoryCombo;
    QCheckBox *originalCheck;
    QCheckBox *allowCommentCheck;
    QCheckBox *immediatePublishCheck;

    // 按钮区域
    QFrame *buttonFrame;
    QPushButton *publishButton;
    QPushButton *saveDraftButton;
    QPushButton *previewButton;

    // 状态标签
    QLabel *statusLabel;

    // 数据
    QString coverPath;
    QStringList tags;
};

#endif // PUBLISH_H
