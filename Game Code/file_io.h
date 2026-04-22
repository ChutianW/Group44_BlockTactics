#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <vector>
#include <ctime>

// 用户数据结构体 —— 记录每个玩家的进度信息
struct UserData {
    std::string username;     // 用户名
    int highest_level;        // 已解锁的最高难度 (1=Easy, 2=Medium, 3=Hard)
    int best_steps_easy;      // 简单难度最佳步数
    int best_steps_medium;    // 中等难度最佳步数
    int best_steps_hard;     // 困难难度最佳步数
    int total_undos_easy;    // 简单难度总撤销次数
    int total_undos_medium;  // 中等难度总撤销次数
    int total_undos_hard;    // 困难难度总撤销次数
    time_t created_at;       // 账户创建时间

    UserData();
    UserData(const std::string &name);
};

// —— 用户数据读写 ——
// 将用户数据保存到文件（默认 data/user_data.txt）
bool saveUserData(const UserData &data, const std::string &filename = "data/user_data.txt");
// 从文件加载用户数据
bool loadUserData(const std::string &username, UserData &data, const std::string &filename = "data/user_data.txt");

// —— 用户管理 ——
// 检查用户名是否已存在于数据文件中
bool userExists(const std::string &username, const std::string &filename = "data/user_data.txt");
// 加载所有用户数据
std::vector<UserData> loadAllUsers(const std::string &filename = "data/user_data.txt");
// 创建新用户（初始最高关卡为 1，各难度最佳步数为 0）
UserData createNewUser(const std::string &username);

// —— 游戏中途保存/加载 ——
// 保存当前游戏状态（关卡号和步数）到文件
bool saveGame(const std::string &filename, int level, int steps);
// 从文件加载已保存的游戏状态
bool loadGame(const std::string &filename, int &level, int &steps);

// —— 排行榜 ——
struct LeaderboardEntry {
    std::string username;
    int difficulty;       // 1=Easy, 2=Medium, 3=Hard
    int best_steps;
    int total_undos;
    time_t created_at;
};
std::vector<LeaderboardEntry> getLeaderboard(const std::string &filename = "data/user_data.txt");
void displayLeaderboard(const std::vector<LeaderboardEntry> &leaderboard);

#endif // FILE_IO_H
