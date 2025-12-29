#include "UserManager.h"
#include "LibraryManager.h"

// ===== 构造：内置最高管理员 =====
UserManager::UserManager() {
    users_.emplace_back(
        "admin",          // 账号
        "admin123",       // 密码
        "系统管理员",     // 姓名
        "系统内置",       // 联系方式
        999,              // 最大借书数
        UserRole::SuperAdmin,
        false             // 未封禁
    );
}

// ===== 内部查找（可修改）=====
User* UserManager::findUserMutable(const std::string& username) {
    for (auto& u : users_) {
        if (u.username() == username)
            return &u;
    }
    return nullptr;
}

// ===== 只读查找 =====
std::optional<User> UserManager::findUser(const std::string& username) const {
    for (const auto& u : users_) {
        if (u.username() == username)
            return u;
    }
    return std::nullopt;
}

// ===== 注册读者 =====
bool UserManager::registerReader(const std::string& username,
    const std::string& password,
    const std::string& name,
    const std::string& contact,
    std::string& outMsg)
{
    if (username.empty() || password.empty() ||
        name.empty() || contact.empty()) {
        outMsg = "注册信息不完整。";
        return false;
    }

    if (findUser(username).has_value()) {
        outMsg = "该账号已存在。";
        return false;
    }

    users_.emplace_back(
        username,
        password,
        name,
        contact,
        3,                  // 默认最大借书数
        UserRole::Reader,   // 默认读者
        false               // 未封禁
    );

    outMsg = "注册成功。";
    return true;
}

// ===== 登录 =====
std::optional<User> UserManager::login(const std::string& username,
    const std::string& password,
    std::string& outMsg) const
{
    for (const auto& u : users_) {
        if (u.username() != username)
            continue;

        if (u.password() != password) {
            outMsg = "密码错误。";
            return std::nullopt;
        }

        if (u.banned()) {
            outMsg = "该账号已被封禁，无法登录。";
            return std::nullopt;
        }

        outMsg = "登录成功。";
        return u;
    }

    outMsg = "账号不存在。";
    return std::nullopt;
}

// ===== 查询 =====
std::vector<User> UserManager::allUsers() const {
    return users_;
}

std::vector<User> UserManager::bannedUsers() const {
    std::vector<User> res;
    for (const auto& u : users_) {
        if (u.banned())
            res.push_back(u);
    }
    return res;
}

// ===== 封禁 / 解封 =====
bool UserManager::setBanned(const std::string& target, bool banned) {
    User* u = findUserMutable(target);
    if (!u) return false;
    if (u->isSuperAdmin()) return false; // 不能封禁最高管理员

    u->setBanned(banned);
    return true;
}

// ===== 管理员修改读者信息 =====
bool UserManager::updateUserInfoByAdmin(const std::string& target,
    const std::string& name,
    const std::string& contact,
    int maxBorrow)
{
    User* u = findUserMutable(target);
    if (!u) return false;
    if (u->isSuperAdmin()) return false;
    if (maxBorrow < 1) return false;

    u->setName(name);
    u->setContact(contact);
    u->setMaxBorrow(maxBorrow);
    return true;
}

// ===== 超级管理员修改角色 =====
bool UserManager::setRoleBySuperAdmin(const std::string& target, UserRole role) {
    User* u = findUserMutable(target);
    if (!u) return false;
    if (u->isSuperAdmin()) return false; // 不允许操作自己

    u->setRole(role);
    return true;
}

// ===== 读者注销（本人）=====
bool UserManager::deleteSelf(const std::string& selfUsername,
    const LibraryManager& lib,
    std::string& outMsg)
{
    // 有未归还书籍，禁止注销
    if (lib.hasUnreturned(selfUsername)) {
        outMsg = "仍有未归还的书籍，无法注销账号。";
        return false;
    }

    for (auto it = users_.begin(); it != users_.end(); ++it) {
        if (it->username() == selfUsername) {
            if (it->isSuperAdmin()) {
                outMsg = "最高管理员账号不可注销。";
                return false;
            }
            users_.erase(it);
            outMsg = "账号已成功注销。";
            return true;
        }
    }

    outMsg = "账号不存在。";
    return false;
}
