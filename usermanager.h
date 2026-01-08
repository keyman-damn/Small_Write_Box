#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QString>

class UserManager
{
public:
    static UserManager* instance();

    // 设置当前登录用户的信息（在登录成功后调用）
    void setUserInfo(int id, const QString& name);

    // 获取当前用户信息
    int  userId() const;
    QString userName() const;
    void logout(); // 登出

private:
    UserManager(); // 私有构造
    static UserManager* m_instance;

    int m_userId;
    QString m_userName;
};

#endif // USERMANAGER_H
