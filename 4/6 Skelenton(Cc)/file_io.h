#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>

// 用户数据结构体 —— 记录每个玩家的进度信息
struct UserData {
    std::string username;     // 用户名
    int highest_level;        // 已解锁的最高关卡
    int best_steps;           // 最佳步数记录
};

// —— 用户数据读写 ——
// 将用户数据保存到文件（默认 user_data.txt）
bool saveUserData(const UserData &data, const std::string &filename = "user_data.txt");
// 从文件加载用户数据
bool loadUserData(UserData &data, const std::string &filename = "user_data.txt");

// —— 用户管理 ——
// 检查用户名是否已存在于数据文件中
bool userExists(const std::string &username, const std::string &filename = "user_data.txt");
// 创建新用户（初始最高关卡为 1，最佳步数为 0）
UserData createNewUser(const std::string &username);
// 尝试加载已有用户；若不存在则创建新用户
UserData loadOrCreateUser(const std::string &username, const std::string &filename = "user_data.txt");

// —— 游戏中途保存/加载 ——
// 保存当前游戏状态（关卡号和步数）到文件
bool saveGame(const std::string &filename, int level, int steps);
// 从文件加载已保存的游戏状态
bool loadGame(const std::string &filename, int &level, int &steps);

#endif // FILE_IO_H
