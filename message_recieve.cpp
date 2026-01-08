#include "message_recieve.h"
#include "ui_message_recieve.h"
#include "QStyle"
#include "page_profile.h"
message_recieve::message_recieve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::message_recieve)
{
    ui->setupUi(this);
    ui->all_comment->setProperty("selected1", true);
    ui->prize->setProperty("selected2", false);
    ui->focus->setProperty("selected3", false);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&message_recieve::turnToHome);
    connect(ui->all_comment, &QPushButton::clicked, this, [=]()
    {
    // 切换显示第一个子窗口
       ui->stackedWidget->setCurrentWidget(ui->page);
       ui->all_comment->setProperty("selected1", true);
       ui->prize->setProperty("selected2", false);
       ui->focus->setProperty("selected3", false);
       ui->all_comment->style()->unpolish(ui->all_comment);
       ui->all_comment->style()->polish(ui->all_comment);
       ui->prize->style()->unpolish(ui->prize);
       ui->prize->style()->polish(ui->prize);
       ui->focus->style()->unpolish(ui->focus);
       ui->focus->style()->polish(ui->focus);
       ui->all_comment->update();
       ui->prize->update();
       ui->focus->update();
    });
    connect(ui->prize, &QPushButton::clicked, this, [=]()
    {
    // 切换显示第二个子窗口
       ui->stackedWidget->setCurrentWidget(ui->page_2);
       ui->all_comment->setProperty("selected1", false);
       ui->prize->setProperty("selected2", true);
       ui->focus->setProperty("selected3", false);
       ui->all_comment->style()->unpolish(ui->all_comment);
       ui->all_comment->style()->polish(ui->all_comment);
       ui->prize->style()->unpolish(ui->prize);
       ui->prize->style()->polish(ui->prize);
       ui->focus->style()->unpolish(ui->focus);
       ui->focus->style()->polish(ui->focus);
       ui->all_comment->update();
       ui->prize->update();
       ui->focus->update();
    });
    connect(ui->focus, &QPushButton::clicked, this, [=]()
    {
    // 切换显示第三个子窗口
       ui->stackedWidget->setCurrentWidget(ui->page_3);
       ui->all_comment->setProperty("selected1", false);
       ui->prize->setProperty("selected2", false);
       ui->focus->setProperty("selected3", true);
       ui->all_comment->style()->unpolish(ui->all_comment);
       ui->all_comment->style()->polish(ui->all_comment);
       ui->prize->style()->unpolish(ui->prize);
       ui->prize->style()->polish(ui->prize);
       ui->focus->style()->unpolish(ui->focus);
       ui->focus->style()->polish(ui->focus);
       ui->all_comment->update();
       ui->prize->update();
       ui->focus->update();
    });
}

message_recieve::~message_recieve()
{
    delete ui;
}
void message_recieve::turnToHome(){
    PageProfile *P=new PageProfile ();
    P->show();

}
