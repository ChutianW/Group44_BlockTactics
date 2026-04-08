#include "file_io.h"
#include <fstream>
#include <iostream>

// ============================================================
// 【下一步工作】保存用户数据到文件（第四步：用户系统）
//
// 文件格式建议（每行一个用户）：
//   username highest_level best_steps
// 示例：
//   Alice 3 45
//   Bob 1 0
//
// 实现思路：
//   1. 先读取文件中所有行到一个 vector<UserData>
//   2. 在 vector 中找到并更新该用户的数据
//   3. 如果没找到，追加新用户
//   4. 将所有数据写回文件
// ============================================================
bool saveUserData(const UserData &data, const std::string &filename) {
    // TODO: 打开文件 → 读取所有数据 → 更新目标用户 → 写回文件
    // 使用 std::fstream 或 std::ofstream
    (void)data;
    (void)filename;
    return false;
}

// ============================================================
// 【下一步工作】从文件加载用户数据（第四步：用户系统）
//
// 实现思路：
//   1. 打开文件，逐行读取
//   2. 每行解析为：username highest_level best_steps
//   3. 找到匹配 data.username 的行
//   4. 将数据填入 data
//   5. 如果找到返回 true，否则返回 false
// ============================================================
bool loadUserData(UserData &data, const std::string &filename) {
    // TODO: 打开文件 → 逐行读取 → 解析数据 → 匹配用户名
    (void)data;
    (void)filename;
    return false;
}

// ============================================================
// 【下一步工作】检查用户是否存在（第四步：用户系统）
//
// 实现思路：
//   1. 打开文件
//   2. 逐行读取，解析用户名
//   3. 如果找到匹配的 username 返回 true
//   4. 文件读完仍未找到返回 false
// ============================================================
bool userExists(const std::string &username, const std::string &filename) {
    // TODO: 打开文件 → 逐行检查用户名是否匹配
    (void)username;
    (void)filename;
    return false;
}

// 创建新用户（已完成，无需修改）
UserData createNewUser(const std::string &username) {
    UserData data;
    data.username = username;
    data.highest_level = 1;  // 新用户从第 1 关开始
    data.best_steps = 0;     // 尚无步数记录
    return data;
}

// 加载已有用户或创建新用户（逻辑正确，依赖上面的函数实现后自动生效）
UserData loadOrCreateUser(const std::string &username, const std::string &filename) {
    if (userExists(username, filename)) {
        UserData data;
        loadUserData(data, filename);
        return data;
    }
    return createNewUser(username);
}

// ============================================================
// 【下一步工作】保存游戏状态（第四步：用户系统 / 第六步：存档）
//
// 用途：游戏进行中保存当前进度
// 文件格式建议：
//   level steps
// 示例：
//   2 38
// ============================================================
bool saveGame(const std::string &filename, int level, int steps) {
    // TODO: 打开文件 → 写入关卡号和步数
    (void)filename;
    (void)level;
    (void)steps;
    return false;
}

// ============================================================
// 【下一步工作】加载游戏状态（第四步：用户系统 / 第六步：存档）
//
// 用途：继续游戏时恢复上次保存的进度
// 读取文件格式与 saveGame 一致
// ============================================================
bool loadGame(const std::string &filename, int &level, int &steps) {
    // TODO: 打开文件 → 读取关卡号和步数
    (void)filename;
    (void)level;
    (void)steps;
    return false;
}
