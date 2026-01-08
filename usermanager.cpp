#include "usermanager.h"

UserManager* UserManager::m_instance = nullptr;

UserManager::UserManager() : m_userId(-1), m_userName("未登录") {}

UserManager* UserManager::instance()
{
    if (!m_instance) {
        m_instance = new UserManager();
    }
    return m_instance;
}

void UserManager::setUserInfo(int id, const QString& name)
{
    m_userId = id;
    m_userName = name;
}

int UserManager::userId() const
{
    return m_userId;
}

QString UserManager::userName() const
{
    return m_userName;
}

void UserManager::logout()
{
    m_userId = -1;
    m_userName = "未登录";
}
