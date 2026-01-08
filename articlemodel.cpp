// articlemodel.cpp
#include "articlemodel.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QDebug>
#include <QDir>

Article::Article()
    : id(-1)
    , isOriginal(true)
    , allowComments(true)
    , viewCount(0)
    , likeCount(0)
    , commentCount(0)
    , userId(0)
{
    publishTime = QDateTime::currentDateTime();
    updateTime = publishTime;
    author = "当前用户";
    category = "未分类";
    tags = "";
}

Article::Article(const QString &title, const QString &content,
                 const QString &author, const QString &cover,
                 const QStringList &tagsList)
    : id(-1)
    , title(title)
    , content(content)
    , coverPath(cover)
    , category("未分类")
    , author(author)
    , isOriginal(true)
    , allowComments(true)
    , viewCount(0)
    , likeCount(0)
    , commentCount(0)
    , userId(0)
{
    publishTime = QDateTime::currentDateTime();
    updateTime = publishTime;
    setTagsFromList(tagsList);
}

QString Article::getSummary(int maxLength) const
{
    QString plainText = content;
    // 移除HTML标签
    plainText = plainText.replace(QRegExp("<[^>]*>"), "");
    plainText = plainText.replace("\n", " ").simplified();

    if (plainText.length() > maxLength) {
        return plainText.left(maxLength) + "...";
    }
    return plainText;
}

QStringList Article::getTagsList() const
{
    if (tags.isEmpty()) {
        return QStringList();
    }
    return tags.split(',', QString::SkipEmptyParts);
}

void Article::setTagsFromList(const QStringList &tagsList)
{
    tags = tagsList.join(',');
}

QJsonObject Article::toJson() const
{
    QJsonObject json;
    json["id"] = id;
    json["title"] = title;
    json["content"] = content;
    json["coverPath"] = coverPath;
    json["tags"] = tags;
    json["category"] = category;
    json["author"] = author;
    json["isOriginal"] = isOriginal;
    json["allowComments"] = allowComments;
    json["publishTime"] = publishTime.toString(Qt::ISODate);
    json["updateTime"] = updateTime.toString(Qt::ISODate);
    json["viewCount"] = viewCount;
    json["likeCount"] = likeCount;
    json["commentCount"] = commentCount;
    json["userId"] = userId;

    return json;
}

Article Article::fromJson(const QJsonObject &json)
{
    Article article;
    article.id = json["id"].toInt();
    article.title = json["title"].toString();
    article.content = json["content"].toString();
    article.coverPath = json["coverPath"].toString();
    article.tags = json["tags"].toString();
    article.category = json["category"].toString();
    article.author = json["author"].toString();
    article.isOriginal = json["isOriginal"].toBool();
    article.allowComments = json["allowComments"].toBool();
    article.publishTime = QDateTime::fromString(json["publishTime"].toString(), Qt::ISODate);
    article.updateTime = QDateTime::fromString(json["updateTime"].toString(), Qt::ISODate);
    article.viewCount = json["viewCount"].toInt();
    article.likeCount = json["likeCount"].toInt();
    article.commentCount = json["commentCount"].toInt();
    article.userId = json["userId"].toInt();

    return article;
}

// ArticleModel 实现
ArticleModel* ArticleModel::m_instance = nullptr;

ArticleModel* ArticleModel::instance()
{
    if (!m_instance) {
        m_instance = new ArticleModel();
        m_instance->initDatabase();
    }
    return m_instance;
}

ArticleModel::ArticleModel(QObject *parent) : QObject(parent)
{
}

ArticleModel::~ArticleModel()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool ArticleModel::initDatabase()
{
    // 使用相对路径，与userdata.db在同一个目录
    QString dbPath = "articles.db";  // 相对路径

    qDebug() << "数据库文件路径:" << QDir::currentPath() + "/" + dbPath;

    // 连接数据库
    db = QSqlDatabase::addDatabase("QSQLITE", "articles_connection");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qWarning() << "无法打开数据库:" << db.lastError().text();
        return false;
    }

    // 创建表
    return createTables();
}

bool ArticleModel::createTables()
{
    QSqlQuery query(db);

    // 创建文章表
    QString createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS articles (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            content TEXT NOT NULL,
            cover_path TEXT,
            tags TEXT,
            category TEXT DEFAULT '未分类',
            author TEXT NOT NULL,
            is_original BOOLEAN DEFAULT 1,
            allow_comments BOOLEAN DEFAULT 1,
            publish_time DATETIME NOT NULL,
            update_time DATETIME NOT NULL,
            view_count INTEGER DEFAULT 0,
            like_count INTEGER DEFAULT 0,
            comment_count INTEGER DEFAULT 0,
            user_id INTEGER DEFAULT 0,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(createTableSQL)) {
        qWarning() << "创建表失败:" << query.lastError().text();
        return false;
    }
    if (!initFavoriteTable()) {
            qWarning() << "创建收藏表失败";
        }
    // 创建索引
    query.exec("CREATE INDEX IF NOT EXISTS idx_author ON articles(author)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_category ON articles(category)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_user_id ON articles(user_id)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_publish_time ON articles(publish_time)");

    return true;
}

int ArticleModel::addArticle(const Article &article)
{
    QSqlQuery query(db);

    query.prepare(R"(
        INSERT INTO articles (
            title, content, cover_path, tags, category, author,
            is_original, allow_comments, publish_time, update_time,
            view_count, like_count, comment_count, user_id
        ) VALUES (
            :title, :content, :cover_path, :tags, :category, :author,
            :is_original, :allow_comments, :publish_time, :update_time,
            :view_count, :like_count, :comment_count, :user_id
        )
    )");

    query.bindValue(":title", article.title);
    query.bindValue(":content", article.content);
    query.bindValue(":cover_path", article.coverPath);
    query.bindValue(":tags", article.tags);
    query.bindValue(":category", article.category);
    query.bindValue(":author", article.author);
    query.bindValue(":is_original", article.isOriginal);
    query.bindValue(":allow_comments", article.allowComments);
    query.bindValue(":publish_time", article.publishTime.toString(Qt::ISODate));
    query.bindValue(":update_time", article.updateTime.toString(Qt::ISODate));
    query.bindValue(":view_count", article.viewCount);
    query.bindValue(":like_count", article.likeCount);
    query.bindValue(":comment_count", article.commentCount);
    query.bindValue(":user_id", article.userId);

    if (!query.exec()) {
        qWarning() << "添加文章失败:" << query.lastError().text();
        return -1;
    }

    int newId = query.lastInsertId().toInt();

    // 发送信号
    Article newArticle = article;
    newArticle.id = newId;
    emit articleAdded(newArticle);
    emit dataChanged();

    return newId;
}

bool ArticleModel::updateArticle(const Article &article)
{
    QSqlQuery query(db);

    query.prepare(R"(
        UPDATE articles SET
            title = :title,
            content = :content,
            cover_path = :cover_path,
            tags = :tags,
            category = :category,
            author = :author,
            is_original = :is_original,
            allow_comments = :allow_comments,
            update_time = :update_time,
            view_count = :view_count,
            like_count = :like_count,
            comment_count = :comment_count,
            user_id = :user_id
        WHERE id = :id
    )");

    query.bindValue(":id", article.id);
    query.bindValue(":title", article.title);
    query.bindValue(":content", article.content);
    query.bindValue(":cover_path", article.coverPath);
    query.bindValue(":tags", article.tags);
    query.bindValue(":category", article.category);
    query.bindValue(":author", article.author);
    query.bindValue(":is_original", article.isOriginal);
    query.bindValue(":allow_comments", article.allowComments);
    query.bindValue(":update_time", QDateTime::currentDateTime().toString(Qt::ISODate));
    query.bindValue(":view_count", article.viewCount);
    query.bindValue(":like_count", article.likeCount);
    query.bindValue(":comment_count", article.commentCount);
    query.bindValue(":user_id", article.userId);

    if (!query.exec()) {
        qWarning() << "更新文章失败:" << query.lastError().text();
        return false;
    }

    emit articleUpdated(article);
    emit dataChanged();

    return query.numRowsAffected() > 0;
}

bool ArticleModel::deleteArticle(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM articles WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "删除文章失败:" << query.lastError().text();
        return false;
    }

    bool success = query.numRowsAffected() > 0;
    if (success) {
        emit articleDeleted(id);
        emit dataChanged();
    }

    return success;
}

Article ArticleModel::getArticleById(int id) const
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM articles WHERE id = :id");
    query.bindValue(":id",id);

    if (!query.exec() || !query.next()) {
        return Article();
    }

    return createArticleFromQuery(query);
}

QList<Article> ArticleModel::getAllArticles() const
{
    QList<Article> articles;
    QSqlQuery query(db);

    if (!query.exec("SELECT * FROM articles ORDER BY publish_time DESC")) {
        qWarning() << "查询文章失败:" << query.lastError().text();
        return articles;
    }

    while (query.next()) {
        articles.append(createArticleFromQuery(query));
    }

    return articles;
}

QList<Article> ArticleModel::getArticlesByAuthor(const QString &author) const
{
    QList<Article> articles;
    QSqlQuery query(db);

    query.prepare("SELECT * FROM articles WHERE author = :author ORDER BY publish_time DESC");
    query.bindValue(":author", author);

    if (!query.exec()) {
        qWarning() << "按作者查询失败:" << query.lastError().text();
        return articles;
    }

    while (query.next()) {
        articles.append(createArticleFromQuery(query));
    }

    return articles;
}

QList<Article> ArticleModel::getArticlesByCategory(const QString &category) const
{
    QList<Article> articles;
    QSqlQuery query(db);

    query.prepare("SELECT * FROM articles WHERE category = :category ORDER BY publish_time DESC");
    query.bindValue(":category", category);

    if (!query.exec()) {
        qWarning() << "按分类查询失败:" << query.lastError().text();
        return articles;
    }

    while (query.next()) {
        articles.append(createArticleFromQuery(query));
    }

    return articles;
}

QList<Article> ArticleModel::getArticlesByUserId(int userId) const
{
    QList<Article> articles;
    QSqlQuery query(db);

    query.prepare("SELECT * FROM articles WHERE user_id = :user_id ORDER BY publish_time DESC");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qWarning() << "按用户ID查询失败:" << query.lastError().text();
        return articles;
    }

    while (query.next()) {
        articles.append(createArticleFromQuery(query));
    }

    return articles;
}

QList<Article> ArticleModel::searchArticles(const QString &keyword) const
{
    QList<Article> articles;
    QSqlQuery query(db);

    query.prepare(R"(
        SELECT * FROM articles
        WHERE title LIKE :keyword
           OR content LIKE :keyword
           OR author LIKE :keyword
        ORDER BY publish_time DESC
    )");

    query.bindValue(":keyword", QString("%%1%").arg(keyword));

    if (!query.exec()) {
        qWarning() << "搜索文章失败:" << query.lastError().text();
        return articles;
    }

    while (query.next()) {
        articles.append(createArticleFromQuery(query));
    }

    return articles;
}

int ArticleModel::getTotalCount() const
{
    QSqlQuery query(db);
    query.exec("SELECT COUNT(*) FROM articles");

    if (query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

int ArticleModel::getAuthorCount(const QString &author) const
{
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM articles WHERE author = :author");
    query.bindValue(":author", author);

    if (!query.exec() || !query.next()) {
        return 0;
    }

    return query.value(0).toInt();
}

int ArticleModel::getCategoryCount(const QString &category) const
{
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM articles WHERE category = :category");
    query.bindValue(":category", category);

    if (!query.exec() || !query.next()) {
        return 0;
    }

    return query.value(0).toInt();
}

bool ArticleModel::incrementViewCount(int id)
{
    QSqlQuery query(db);
    query.prepare("UPDATE articles SET view_count = view_count + 1 WHERE id = :id");
    query.bindValue(":id", id);

    return query.exec();
}

bool ArticleModel::incrementLikeCount(int id)
{
    QSqlQuery query(db);
    query.prepare("UPDATE articles SET like_count = like_count + 1 WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        emit dataChanged();
        return true;
    }

    return false;
}

bool ArticleModel::incrementCommentCount(int id)
{
    QSqlQuery query(db);
    query.prepare("UPDATE articles SET comment_count = comment_count + 1 WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        emit dataChanged();
        return true;
    }

    return false;
}

Article ArticleModel::createArticleFromQuery(const QSqlQuery &query) const
{
    Article article;

    article.id = query.value("id").toInt();
    article.title = query.value("title").toString();
    article.content = query.value("content").toString();
    article.coverPath = query.value("cover_path").toString();
    article.tags = query.value("tags").toString();
    article.category = query.value("category").toString();
    article.author = query.value("author").toString();
    article.isOriginal = query.value("is_original").toBool();
    article.allowComments = query.value("allow_comments").toBool();
    article.publishTime = QDateTime::fromString(query.value("publish_time").toString(), Qt::ISODate);
    article.updateTime = QDateTime::fromString(query.value("update_time").toString(), Qt::ISODate);
    article.viewCount = query.value("view_count").toInt();
    article.likeCount = query.value("like_count").toInt();
    article.commentCount = query.value("comment_count").toInt();
    article.userId = query.value("user_id").toInt();

    return article;
}
bool ArticleModel::initFavoriteTable() {
    // 必须传入 db 成员，确保操作的是 articles.db
    QSqlQuery query(db);
    return query.exec("CREATE TABLE IF NOT EXISTS favorites ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "user_id INTEGER, "
                      "article_id INTEGER, "
                      "fav_time DATETIME, "
                      "UNIQUE(user_id, article_id))");
}
bool ArticleModel::addFavorite(int userId, int articleId) {
    QSqlQuery query(db); // 指定 db
    query.prepare("INSERT OR IGNORE INTO favorites (user_id, article_id, fav_time) "
                  "VALUES (:uid, :aid, datetime('now'))");
    query.bindValue(":uid", userId);
    query.bindValue(":aid", articleId);
    bool ok = query.exec();
    if(!ok) qDebug() << "添加收藏失败:" << query.lastError().text();
    return ok;
}
QList<Article> ArticleModel::getFavoriteArticles(int userId) {
    QList<Article> list;
    QSqlQuery query(db); // 指定 db
    query.prepare("SELECT a.* FROM articles a "
                  "INNER JOIN favorites f ON a.id = f.article_id "
                  "WHERE f.user_id = :uid ORDER BY f.fav_time DESC");
    query.bindValue(":uid", userId);

    if (query.exec()) {
        while (query.next()) {
            list.append(createArticleFromQuery(query)); // 直接复用你现成的解析函数
        }
    } else {
        qDebug() << "获取收藏失败:" << query.lastError().text();
    }
    return list;
}
// 检查状态 (读)
bool ArticleModel::isFavorite(int userId, int articleId) {
    QSqlQuery query(db); // 注意使用你的 db 成员
    query.prepare("SELECT 1 FROM favorites WHERE user_id = :uid AND article_id = :aid");
    query.bindValue(":uid", userId);
    query.bindValue(":aid", articleId);
    return query.exec() && query.next();
}



// 取消收藏 (删)
bool ArticleModel::removeFavorite(int userId, int articleId) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM favorites WHERE user_id = :uid AND article_id = :aid");
    query.bindValue(":uid", userId);
    query.bindValue(":aid", articleId);
    return query.exec();
}
