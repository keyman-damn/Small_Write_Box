#include "manager.h"
#include "ui_manager.h"
#include "commentitem.h"
#include "reportitem.h"
#include "useritem.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QStackedWidget>

Manager::Manager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Manager),
    stackedWidget(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("小白盒 - 管理员后台");
    this->setWindowIcon(QIcon(":/icons/gamer.png"));

    qDebug() << "=== Manager 初始化开始 ===";

    // 创建 stackedWidget 用于切换内容
    stackedWidget = new QStackedWidget(this);

    // 设置 stackedWidget 的位置和大小，覆盖原来的 scrollArea 区域
    stackedWidget->setGeometry(ui->scrollArea->geometry());

    // 隐藏原来的 scrollArea
    ui->scrollArea->hide();

    // 初始化各个界面
    initContentAudit();
    initReportHandling();
    initUserManagement();

    // 连接按钮信号
    connect(ui->pushButton, &QPushButton::clicked,
            this, &Manager::onContentAuditClicked);
    connect(ui->pushButton_4, &QPushButton::clicked,
            this, &Manager::onReportHandlingClicked);
    connect(ui->pushButton_5, &QPushButton::clicked,
            this, &Manager::onUserManagementClicked);

    // 默认显示内容审核界面
    stackedWidget->setCurrentIndex(0);

    qDebug() << "=== Manager 初始化完成 ===";
}

Manager::~Manager()
{
    delete ui;
}
// 初始化内容审核界面
void Manager::initContentAudit()
{
    // 1. 创建一个容器Widget
    QWidget *container = new QWidget();

    // 2. 创建一个垂直布局用于容器
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);

    // 3. 创建一个ScrollArea
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // 关键：允许widget调整大小
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 4. 创建真正的内容Widget
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(contentWidget);

    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setAlignment(Qt::AlignTop);

    // 添加测试评论
    QStringList testComments = {
        "这个游戏很好玩，画面精美，操作流畅，推荐给大家！",
        "有点卡顿，希望优化一下性能，特别是团战的时候。",
        "新手引导太长了，希望能跳过或者缩短一些。",
        "角色设计很酷，技能特效也很炫，但平衡性需要调整。",
        "服务器经常掉线，希望官方能修复网络问题。"
    };

    for(int i = 0; i < 5; i++) {
        CommentItem *item = new CommentItem(contentWidget);
        item->setData(
            QString("玩家%1").arg(i+1),
            QString("【测试评论%1】").arg(i+1) + testComments[i]
        );
        mainLayout->addWidget(item);
    }

    // 添加伸缩因子
    mainLayout->addStretch();

    // 5. 设置ScrollArea的内容
    scrollArea->setWidget(contentWidget);

    // 6. 将ScrollArea添加到容器布局
    containerLayout->addWidget(scrollArea);

    // 7. 将容器添加到StackedWidget
    stackedWidget->addWidget(container);
}

// 初始化举报处理界面
void Manager::initReportHandling()
{
    // 1. 创建一个容器Widget
    QWidget *container = new QWidget();

    // 2. 创建一个垂直布局用于容器
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);

    // 3. 创建一个ScrollArea
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // 关键：允许widget调整大小
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 4. 创建真正的内容Widget
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(contentWidget);

    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setAlignment(Qt::AlignTop);

    // 5. 添加测试举报数据
    QStringList reasons = {
        "语言侮辱：发表不当言论",
        "垃圾广告：频繁发布广告信息",
        "色情内容：包含不适宜内容",
        "人身攻击：对其他用户进行攻击",
        "违规行为：违反社区规定"
    };

    QStringList contents = {
        "你这个人是真的垃圾，不会玩就别玩！",
        "加VX：12345678，最新游戏外挂，稳定不封号",
        "这里有一些不适合公开的内容链接...",
        "你就是个废物，操作这么差还玩游戏",
        "使用外挂程序进行游戏，影响公平性"
    };

    for (int i = 0; i < 5; i++) {
        ReportItem* item = new ReportItem(contentWidget);
        item->setReportData(
            QString("举报人%1").arg(i+1),
            reasons[i % reasons.size()],
            contents[i % contents.size()]
        );
        mainLayout->addWidget(item);
    }

    // 6. 添加拉伸空间
    mainLayout->addStretch();

    // 7. 设置ScrollArea的内容
    scrollArea->setWidget(contentWidget);

    // 8. 将ScrollArea添加到容器布局
    containerLayout->addWidget(scrollArea);

    // 9. 将容器添加到StackedWidget
    stackedWidget->addWidget(container);

    qDebug() << "举报处理界面初始化完成";
}

// manager.cpp 中修改 initUserManagement() 函数
void Manager::initUserManagement()
{
    // 1. 创建一个容器Widget（与其他界面保持一致）
    QWidget *container = new QWidget();
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);

    // 2. 创建ScrollArea
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 3. 创建内容Widget
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(contentWidget);

    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setAlignment(Qt::AlignTop);

    // 4. 添加用户管理标题和搜索
    QHBoxLayout *headerLayout = new QHBoxLayout();

    QLabel *titleLabel = new QLabel("用户管理");
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 20px;
            font-weight: bold;
            color: #2c3e50;
        }
    )");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    // 添加搜索框
    QLineEdit *searchInput = new QLineEdit();
    searchInput->setPlaceholderText("搜索用户名或ID...");
    searchInput->setFixedWidth(250);
    searchInput->setStyleSheet(R"(
        QLineEdit {
            padding: 8px 12px;
            border: 1px solid #bdc3c7;
            border-radius: 6px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border-color: #3498db;
        }
    )");
    headerLayout->addWidget(searchInput);

    mainLayout->addLayout(headerLayout);

    // 5. 添加分隔线
    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("color: #ecf0f1;");
    mainLayout->addWidget(separator);

    // 6. 添加示例用户数据（实际中应该从数据库获取）
    // 用户数据：ID, 用户名, 注册日期, 状态, 举报次数, 邮箱
    QList<QStringList> userData = {
        {"U001", "张三", "2023-01-15", "正常", "2", "zhangsan@example.com"},
        {"U002", "李四", "2023-02-20", "警告中", "5", "lisi@example.com"},
        {"U003", "王五", "2023-03-10", "已封禁", "12", "wangwu@example.com"},
        {"U004", "赵六", "2023-04-05", "正常", "0", "zhaoliu@example.com"},
        {"U005", "钱七", "2023-05-18", "警告中", "3", "qianqi@example.com"},
        {"U006", "孙八", "2023-06-22", "正常", "1", "sunba@example.com"},
        {"U007", "周九", "2023-07-30", "已封禁", "8", "zhoujiu@example.com"},
        {"U008", "吴十", "2023-08-14", "正常", "0", "wushi@example.com"}
    };

    // 7. 添加用户项
    for (const QStringList &user : userData) {
        UserItem *userItem = new UserItem(contentWidget);
        userItem->setUserData(
            user[0],  // ID
            user[1],  // 用户名
            user[2],  // 注册日期
            user[3],  // 状态
            user[4].toInt(),  // 举报次数
            user[5]   // 邮箱
        );
        mainLayout->addWidget(userItem);

        // 连接信号
        connect(userItem, &UserItem::userBanned, this, [userItem]() {
            qDebug() << "用户管理：用户" << userItem->getUserId() << "被管理员封禁";
        });

        connect(userItem, &UserItem::userWarned, this, [userItem]() {
            qDebug() << "用户管理：用户" << userItem->getUserId() << "被管理员警告";
        });

        connect(userItem, &UserItem::userUnbanned, this, [userItem]() {
            qDebug() << "用户管理：用户" << userItem->getUserId() << "被管理员解封";
        });
    }

    // 8. 添加拉伸空间
    mainLayout->addStretch();

    // 9. 设置ScrollArea
    scrollArea->setWidget(contentWidget);
    containerLayout->addWidget(scrollArea);

    // 10. 添加到StackedWidget
    stackedWidget->addWidget(container);

    qDebug() << "用户管理界面初始化完成";
}
// 槽函数：内容审核按钮点击
void Manager::onContentAuditClicked()
{
    stackedWidget->setCurrentIndex(0);
    qDebug() << "切换到内容审核界面";

    // 可以在这里更新按钮样式，表示当前选中的界面
    ui->pushButton->setStyleSheet("background-color: #e0e0e0;");
    ui->pushButton_4->setStyleSheet("");
    ui->pushButton_5->setStyleSheet("");
}

// 槽函数：举报处理按钮点击
void Manager::onReportHandlingClicked()
{
    stackedWidget->setCurrentIndex(1);
    qDebug() << "切换到举报处理界面";

    ui->pushButton->setStyleSheet("");
    ui->pushButton_4->setStyleSheet("background-color: #e0e0e0;");
    ui->pushButton_5->setStyleSheet("");
}

// 槽函数：用户管理按钮点击
void Manager::onUserManagementClicked()
{
    stackedWidget->setCurrentIndex(2);
    qDebug() << "切换到用户管理界面";

    ui->pushButton->setStyleSheet("");
    ui->pushButton_4->setStyleSheet("");
    ui->pushButton_5->setStyleSheet("background-color: #e0e0e0;");
}
