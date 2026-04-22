#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <vector>
#include <ctime>

// User data struct - player progress
struct UserData {
    std::string username;
    int highest_level;
    int best_steps_easy;
    int best_steps_medium;
    int best_steps_hard;
    int total_undos_easy;
    int total_undos_medium;
    int total_undos_hard;
    time_t created_at;

    UserData();
    UserData(const std::string &name);
};

// User data I/O
bool saveUserData(const UserData &data, const std::string &filename = "data/user_data.txt");
bool loadUserData(const std::string &username, UserData &data, const std::string &filename = "data/user_data.txt");

// User management
bool userExists(const std::string &username, const std::string &filename = "data/user_data.txt");
std::vector<UserData> loadAllUsers(const std::string &filename = "data/user_data.txt");
UserData createNewUser(const std::string &username);

// Mid-game save/load
bool saveGame(const std::string &filename, int level, int steps);
bool loadGame(const std::string &filename, int &level, int &steps);

// Leaderboard
struct LeaderboardEntry {
    std::string username;
    int difficulty;
    int best_steps;
    int total_undos;
    time_t created_at;
};
std::vector<LeaderboardEntry> getLeaderboard(const std::string &filename = "data/user_data.txt");
void displayLeaderboard(const std::vector<LeaderboardEntry> &leaderboard);

#endif // FILE_IO_H
