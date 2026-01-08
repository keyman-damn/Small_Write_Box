#include "gamedetailwidget.h"
#include "ui_gamedetailwidget.h"
#include <QDebug>
#include <QMap>
#include <QPixmap>
#include <QTimer>
#include <QIcon>

GameDetailWidget::GameDetailWidget(const QString& gameName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameDetailWidget),
    currentGameName(gameName)
{
    ui->setupUi(this);
    // 设置窗口图标（在构造函数中根据游戏名称设置）
       setWindowIconByGameName(gameName);
    // 调试：检查UI中label的初始尺寸
    qDebug() << "Label初始尺寸:" << ui->label->size();
    // 设置窗口属性
       this->setWindowFlags(Qt::Window);
       this->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除

       // 设置固定大小或最小大小
       this->setMinimumSize(1000, 800);
       this->resize(1100, 900);
    // 确保label在布局中有足够空间
        ui->label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        // 设置标签的最小尺寸
            ui->label->setMinimumSize(300, 250);
            // 设置边距，给图片更多空间
                ui->label->setContentsMargins(10, 10, 10, 10);

                // 禁用自动缩放
                ui->label->setScaledContents(false);

            // 调试信息
            qDebug() << "GameDetailWidget 构造函数";
            qDebug() << "窗口尺寸:" << this->size();
            qDebug() << "Label初始尺寸:" << ui->label->size();
    loadGameData(gameName);
    // 加载完成后再次检查尺寸
        QTimer::singleShot(100, this, [this]() {
            qDebug() << "窗口加载完成后尺寸:";
            qDebug() << "窗口:" << this->size();
            qDebug() << "Label:" << ui->label->size();
            qDebug() << "Label pixmap:" << (ui->label->pixmap() ? ui->label->pixmap()->size() : QSize(0, 0));
        });
}

// 新增一个私有方法来设置窗口图标
void GameDetailWidget::setWindowIconByGameName(const QString& gameName)
{
    // 窗口图标映射
    static QMap<QString, QString> windowIconMap = {
        {"绝地求生", ":/icons/PUBG.png"},
        {"无畏契约", ":/icons/Wa.png"},
        {"CS2", ":/icons/CSGO.png"},
        {"英雄联盟", ":/icons/lol.png"},
        {"我的世界", ":/icons/Mc.png"},
        {"三角洲行动", ":/icons/Three.png"},
        {"黑神话悟空", ":/icons/wukong.png"},
        {"Apex", ":/icons/apex.1.png"},
        {"Wallpaper", ":/icons/wallpaper_engine.png"},
        {"单机", ":/icons/computer.png"},
        {"moba", ":/icons/pvp.png"},
        {"Fps", ":/icons/-zshejilei.png"}
    };

    QString iconPath = windowIconMap.value(gameName, ":/icons/gamer.png");
    this->setWindowIcon(QIcon(iconPath));
}

// 在 GameDetailWidget::~GameDetailWidget() 函数之前添加：

void GameDetailWidget::displayOptimizedImage(const QString& imagePath)
{
    QPixmap pixmap;
    if (!pixmap.load(imagePath)) {
        // 加载失败，使用默认图片
        pixmap.load(":/icons/gamer.png");
    }

    if (pixmap.isNull()) return;

    // 直接使用Label的完整尺寸（减去少量边距）
    int labelWidth = ui->label->width() - 20;  // 10px边距*2
    int labelHeight = ui->label->height() - 20;

    qDebug() << "Label完整尺寸:" << ui->label->width() << "x" << ui->label->height();
    qDebug() << "Label可用尺寸:" << labelWidth << "x" << labelHeight;

    // 根据图片尺寸决定显示策略
    int originalWidth = pixmap.width();
    int originalHeight = pixmap.height();

    int targetWidth, targetHeight;

    // 分类处理 - 更激进的策略
    if (originalWidth < 800) {
        // Category A: 小图片 (我的世界620×309, 英雄联盟575×380)
        // 放大到至少Label宽度的80%
        targetWidth = labelWidth * 0.85;
        if (targetWidth < 600) targetWidth = 600; // 最小600px

        // 如果原始图片很小，可以放大更多
        if (originalWidth < 400) {
            targetWidth = labelWidth * 0.9; // 更小图片用90%宽度
        }

        targetHeight = targetWidth * originalHeight / originalWidth;

        qDebug() << "小图片策略: 原始" << originalWidth << "x" << originalHeight;
        qDebug() << "放大到" << targetWidth << "宽度";

    } else if (originalWidth < 2000) {
        // Category B: 中等图片 (三角洲行动1440×795, Wallpaper1649×927)
        // 使用Label的90%宽度
        targetWidth = labelWidth * 0.9;
        targetHeight = targetWidth * originalHeight / originalWidth;

        qDebug() << "中等图片策略: 使用90%宽度";

    } else {
        // Category C: 大图片 (无畏契约2881×1620, CS2 3840×2160等)
        // 直接使用Label的完整宽度（保持比例）
        targetWidth = labelWidth;
        targetHeight = targetWidth * originalHeight / originalWidth;

        // 如果按比例计算的高度超过Label高度，改用高度为准
        if (targetHeight > labelHeight) {
            targetHeight = labelHeight;
            targetWidth = targetHeight * originalWidth / originalHeight;
        }

        qDebug() << "大图片策略: 填满宽度";
    }

    // 最终调整 - 确保充分利用空间
    // 检查是否还可以更大
    if (targetHeight < labelHeight * 0.9) {
        // 高度还有空间，可以适当增加
        int potentialHeight = labelHeight * 0.95;
        int potentialWidth = potentialHeight * originalWidth / originalHeight;

        if (potentialWidth <= labelWidth) {
            targetWidth = potentialWidth;
            targetHeight = potentialHeight;
            qDebug() << "高度优化: 增加到" << targetWidth << "x" << targetHeight;
        }
    }

    // 确保在Label范围内
    targetWidth = qMin(targetWidth, labelWidth);
    targetHeight = qMin(targetHeight, labelHeight);

    // 确保最小显示尺寸
    targetWidth = qMax(targetWidth, 500);  // 最小500px
    targetHeight = qMax(targetHeight, 300); // 最小300px

    qDebug() << "计算的目标尺寸:" << targetWidth << "x" << targetHeight;

    // 应用缩放
    QPixmap scaledPixmap = pixmap.scaled(targetWidth, targetHeight,
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);

    ui->label->setPixmap(scaledPixmap);
    ui->label->setAlignment(Qt::AlignCenter);

    // 记录最终尺寸
    qDebug() << "最终显示尺寸: " << scaledPixmap.width() << "x" << scaledPixmap.height();
}

GameDetailWidget::~GameDetailWidget()
{
    delete ui;
}

// 只修改 showEvent 和 resizeEvent，不使用 Qt::ReturnByValue
void GameDetailWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    // 简单延迟调整
    QTimer::singleShot(100, this, [this]() {
        if (ui->label->pixmap()) {
            // 获取当前图片（如果有的话）
            QPixmap current = *ui->label->pixmap();
            if (!current.isNull()) {
                updatePixmapSize(current);
            }
        }
    });
}

void GameDetailWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // 简单的重新调整
    if (ui->label->pixmap()) {
        QPixmap current = *ui->label->pixmap();
        if (!current.isNull()) {
            updatePixmapSize(current);
        }
    }
}

// 修改 updatePixmapSize 函数
void GameDetailWidget::updatePixmapSize(const QPixmap &pixmap)
{
    if (pixmap.isNull()) return;

    // 当窗口大小变化时，重新计算最佳显示尺寸
    QRect contentRect = ui->label->contentsRect();
    int maxWidth = contentRect.width();
    int maxHeight = contentRect.height();

    int originalWidth = pixmap.width();
    int originalHeight = pixmap.height();

    int targetWidth, targetHeight;

    // 根据图片尺寸智能计算
    if (originalWidth < 600) {
        targetWidth = maxWidth * 0.7;
        if (targetWidth < 500) targetWidth = 500;
        targetHeight = targetWidth * originalHeight / originalWidth;
    } else if (originalWidth < 2000) {
        targetWidth = maxWidth * 0.85;
        targetHeight = targetWidth * originalHeight / originalWidth;
    } else {
        targetWidth = maxWidth;
        targetHeight = maxHeight;

        double imageRatio = (double)originalWidth / originalHeight;
        double containerRatio = (double)maxWidth / maxHeight;

        if (imageRatio > containerRatio) {
            targetHeight = targetWidth / imageRatio;
        } else {
            targetWidth = targetHeight * imageRatio;
        }
    }

    // 最终调整
    targetWidth = qMin(targetWidth, maxWidth);
    targetHeight = qMin(targetHeight, maxHeight);

    if (targetWidth < 400) targetWidth = 400;
    if (targetHeight < 250) targetHeight = 250;

    QPixmap scaledPixmap = pixmap.scaled(
        targetWidth, targetHeight,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );

    ui->label->setPixmap(scaledPixmap);
}
void GameDetailWidget::loadGameData(const QString& gameName)
{
    qDebug() << "=== 加载游戏数据 ===";
        qDebug() << "传入的游戏名称:" << gameName;
        qDebug() << "当前游戏名称:" << currentGameName;

        // 更新当前游戏名称
           currentGameName = gameName;

    // 设置窗口标题
    this->setWindowTitle(gameName + " - 详情");
           // 根据游戏名称设置不同的数据
               static QMap<QString, QString> gameDescriptionMap = {
                   {"绝地求生", "《绝地求生》(PUBG) 是一款战术竞技型射击类沙盒游戏..."},
                   {"无畏契约", "《无畏契约》是一款由拳头游戏开发并且免费发行的多人第一人称射击游戏..."},
                   {"CS2", "《反恐精英2》是经典第一人称射击游戏《反恐精英》的正统续作..."},
                   {"英雄联盟", "《英雄联盟》是由美国拳头游戏开发、中国大陆地区腾讯游戏代理运营的英雄对战MOBA竞技网游..."},
                   {"我的世界", "《我的世界》是一款沙盒建造游戏，玩家可以在一个由方块组成的3D世界中探索、采集资源、制作工具和建造建筑..."},
                   {"三角洲行动", "《三角洲行动》是一款战术射击游戏..."},
                   {"黑神话悟空", "《黑神话：悟空》是一款以中国神话为背景的动作角色扮演游戏..."},
                   {"Apex", "《Apex英雄》是一款免费的战术竞技射击游戏，玩家在游戏中将扮演传奇英雄，在战场上展开激烈对决..."},
                   {"Wallpaper", "Wallpaper Engine是一款动态壁纸软件，允许用户在桌面上使用动画、视频和交互式背景..."},
                   {"三角洲行动", "《三角洲行动》是一款现代军事题材的第一人称射击游戏..."},
                   {"单机", "单机游戏指不需要网络连接即可游玩的游戏..."},
                   {"moba", "MOBA（多人在线战术竞技游戏）是一种流行的游戏类型，玩家控制一个角色与队友合作对抗敌方队伍..."},
                   {"Fps", "FPS（第一人称射击游戏）是一种以第一人称视角进行射击的游戏类型..."}
               };

               static QMap<QString, QString> gameImageMap = {
                   {"绝地求生", ":/icons/pubg.jpg"},
                   {"无畏契约", ":/icons/VALOLANT.jpg"},  // 使用已有资源
                   {"CS2", ":/icons/CS2.jpg"},
                   {"英雄联盟", ":/icons/lol.jpg"},
                   {"我的世界", ":/icons/minecraft.jpg"},
                   {"三角洲行动", ":/icons/Delta Force.jpg"},
                   {"黑神话悟空", ":/icons/Black Myth.jpg"},
                   {"Apex", ":/icons/Apex.jpg"},
                   {"Wallpaper", ":/icons/wallpaper.jpg"},
                   {"单机", ":/icons/computer.png"},
                   {"moba", ":/icons/pvp.png"},
                   {"Fps", ":/icons/-zshejilei.png"}
               };

               static QMap<QString, QString> gameInfoMap = {
                   {"绝地求生", "PUBG：BATTLEGROUNDS<br/>发行时间：2017年12月21日<br/>开发商：PUBG Corporation<br/>发行商：KRAFTON, Inc"},
                   {"无畏契约", "VALORANT<br/>发行时间：2020年6月2日<br/>开发商：Riot Games<br/>发行商：Riot Games"},
                   {"CS2", "Counter-Strike 2<br/>发行时间：2023年9月27日<br/>开发商：Valve<br/>发行商：Valve"},
                   {"英雄联盟", "League of Legends<br/>发行时间：2009年10月27日<br/>开发商：Riot Games<br/>发行商：Riot Games"},
                   {"我的世界", "Minecraft<br/>发行时间：2011年11月18日<br/>开发商：Mojang Studios<br/>发行商：Mojang Studios"},
                   {"三角洲行动", "Operation Delta<br/>发行时间：待定<br/>开发商：未知<br/>发行商：未知"},
                   {"黑神话悟空", "Black Myth: Wukong<br/>发行时间：2024年8月20日<br/>开发商：游戏科学<br/>发行商：游戏科学"},
                   {"Apex", "Apex Legends<br/>发行时间：2019年2月4日<br/>开发商：Respawn Entertainment<br/>发行商：Electronic Arts"},
                   {"Wallpaper", "Wallpaper Engine<br/>发行时间：2016年11月11日<br/>开发商：Kristjan Skutta<br/>发行商：Kristjan Skutta"}
               };

               static QMap<QString, QString> gameAwardsMap = {
                   {"绝地求生", "DICE年度大奖 2018 - 年度动作<br/>Steam大奖 2018 - 年度最佳<br/>Steam大奖 2017 - \"老妈的意大利面\"奖<br/>金摇杆奖 2017 - 最佳多人游戏"},
                   {"英雄联盟", "金摇杆奖 2011 - 年度最佳免费游戏<br/>The Game Awards 2019 - 最佳电竞游戏<br/>Esports Awards 2020 - 年度最佳电竞游戏"},
                   {"我的世界", "BAFTA Games Award 2011 - 最佳游戏<br/>Game Developers Choice Awards 2011 - 最佳下载游戏<br/>Golden Joystick Awards 2011 - 最佳下载游戏"},
                   {"CS2", "The Game Awards 2023 - 最佳电竞游戏提名<br/>Esports Awards 2023 - 最佳电竞游戏提名"}
               };

    // 设置游戏标题
    ui->titlelabel->setText(gameName);

    // 设置游戏描述
        QString description = gameDescriptionMap.value(gameName, "暂无游戏描述");
        ui->textBrowser->setHtml(QString("<p style='font-size:11pt; line-height:140%;'>%1</p>").arg(description));

        // 设置游戏图片 - 使用新的优化函数
            QString imagePath = gameImageMap.value(gameName, ":/icons/gamer.png");
            qDebug() << "\n=== 图片处理开始 ===";
            qDebug() << "游戏:" << gameName;
            qDebug() << "图片路径:" << imagePath;

            displayOptimizedImage(imagePath);

            qDebug() << "=== 图片处理结束 ===\n";
        // 设置游戏信息
        QString infoText = gameInfoMap.value(gameName,
            QString("游戏：%1<br/>发行时间：未知<br/>开发商：未知<br/>发行商：未知").arg(gameName));
        ui->label_2->setText(infoText);

        // 设置游戏奖项
        QString awardsText = gameAwardsMap.value(gameName, "暂无奖项信息");
        ui->label_3->setText(awardsText);

        qDebug() << "游戏数据加载完成：" << gameName;
}
