#include "file_io.h"
#include <fstream>
#include <sstream>
#include <iostream>

// ============================================================
// UserData 构造函数
// ============================================================

UserData::UserData()
    : username(""), highest_level(1), best_steps_easy(0),
      best_steps_medium(0), best_steps_hard(0) {}

UserData::UserData(const std::string &name)
    : username(name), highest_level(1), best_steps_easy(0),
      best_steps_medium(0), best_steps_hard(0) {}

// ============================================================
// 创建新用户
// ============================================================
UserData createNewUser(const std::string &username) {
    return UserData(username);
}

// ============================================================
// 保存用户数据到文件
//
// 文件格式（每行一个用户）：
//   用户名 最高难度 简单最佳步数 中等最佳步数 困难最佳步数
// 示例：
//   Alice 3 45 78 120
//   Bob 1 0 0 0
//
// 实现方式：
//   1. 读取所有用户数据到 vector
//   2. 更新或追加目标用户
//   3. 全部写回文件
// ============================================================
bool saveUserData(const UserData &data, const std::string &filename) {
    // 读取所有现有用户
    std::vector<UserData> all_users = loadAllUsers(filename);

    // 更新已有用户或追加新用户
    bool found = false;
    for (auto &user : all_users) {
        if (user.username == data.username) {
            user = data;
            found = true;
            break;
        }
    }

    if (!found) {
        all_users.push_back(data);
    }

    // 写回文件
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (const auto &user : all_users) {
        file << user.username << " "
             << user.highest_level << " "
             << user.best_steps_easy << " "
             << user.best_steps_medium << " "
             << user.best_steps_hard << "\n";
    }

    file.close();
    return true;
}

// ============================================================
// 从文件加载用户数据
// 逐行读取，匹配用户名后解析其进度信息
// ============================================================
bool loadUserData(const std::string &username, UserData &data, const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string stored_name;
        ss >> stored_name;

        if (stored_name == username) {
            data.username = stored_name;
            ss >> data.highest_level
               >> data.best_steps_easy
               >> data.best_steps_medium
               >> data.best_steps_hard;
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// ============================================================
// 检查用户名是否已存在
// 逐行读取文件，匹配用户名
// ============================================================
bool userExists(const std::string &username, const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string stored_name;
        ss >> stored_name;
        if (stored_name == username) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// ============================================================
// 加载所有用户数据（用于 saveUserData 中的更新逻辑）
// ============================================================
std::vector<UserData> loadAllUsers(const std::string &filename) {
    std::vector<UserData> users;
    std::ifstream file(filename);

    if (!file.is_open()) {
        return users;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        UserData user;
        ss >> user.username
           >> user.highest_level
           >> user.best_steps_easy
           >> user.best_steps_medium
           >> user.best_steps_hard;

        if (!user.username.empty()) {
            users.push_back(user);
        }
    }

    file.close();
    return users;
}

// ============================================================
// 保存游戏状态（用于游戏中途存档）
// 格式：关卡号 步数
// ============================================================
bool saveGame(const std::string &filename, int level, int steps) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << level << " " << steps << "\n";
    file.close();
    return true;
}

// ============================================================
// 加载游戏状态（用于继续上次的进度）
// ============================================================
bool loadGame(const std::string &filename, int &level, int &steps) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file >> level >> steps;
    file.close();
    return true;
}
