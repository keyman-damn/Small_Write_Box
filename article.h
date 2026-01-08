// articlemodel.h
#ifndef ARTICLEMODEL_H
#define ARTICLEMODEL_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class Article
{
public:
    Article();
    Article(const QString &title, const QString &content,
            const QString &author, const QString &cover = "",
            const QStringList &tags = QStringList());

    // 数据库映射字段
    int id;                 // 主键ID
    QString title;          // 标题
    QString content;        // 内容
    QString coverPath;      // 封面路径
    QString tags;           // 标签（用逗号分隔）
    QString category;       // 分类
    QString author;         // 作者（用于个人主页筛选）
    bool isOriginal;        // 是否原创
    bool allowComments;     // 是否允许评论
    QDateTime publishTime;  // 发布时间
    QDateTime updateTime;   // 更新时间
    int viewCount;          // 浏览次数
    int likeCount;          // 点赞数
    int commentCount;       // 评论数
    int userId;             // 用户ID（如果有用户系统）

    // 辅助方法
    QString getSummary(int maxLength = 100) const;
    QStringList getTagsList() const;
    void setTagsFromList(const QStringList &tagsList);

    // 序列化和反序列化
    QJsonObject toJson() const;
    static Article fromJson(const QJsonObject &json);
};

class ArticleModel : public QObject
{
    Q_OBJECT

public:
    static ArticleModel* instance();

    // 数据库操作
    bool initDatabase();
    bool createTables();

    // 文章操作
    int addArticle(const Article &article);
    bool updateArticle(const Article &article);
    bool deleteArticle(int id);
    Article getArticleById(int id) const;
    QList<Article> getAllArticles() const;
    QList<Article> getArticlesByAuthor(const QString &author) const;
    QList<Article> getArticlesByCategory(const QString &category) const;
    QList<Article> getArticlesByUserId(int userId) const;
    QList<Article> searchArticles(const QString &keyword) const;

    // 统计
    int getTotalCount() const;
    int getAuthorCount(const QString &author) const;
    int getCategoryCount(const QString &category) const;

    // 更新计数
    bool incrementViewCount(int id);
    bool incrementLikeCount(int id);
    bool incrementCommentCount(int id);

signals:
    void articleAdded(const Article &article);
    void articleUpdated(const Article &article);
    void articleDeleted(int id);
    void dataChanged();

private:
    explicit ArticleModel(QObject *parent = nullptr);
    ~ArticleModel();

    QSqlDatabase db;
    static ArticleModel* m_instance;

    // 从查询结果创建Article对象
    Article createArticleFromQuery(const QSqlQuery &query) const;
};

#endif // ARTICLEMODEL_H
