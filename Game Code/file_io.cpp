#include "file_io.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <ctime>

// ============================================================
// UserData constructors
// ============================================================

UserData::UserData()
    : username(""), highest_level(1), best_steps_easy(0),
      best_steps_medium(0), best_steps_hard(0),
      total_undos_easy(0), total_undos_medium(0), total_undos_hard(0), created_at(0) {}

UserData::UserData(const std::string &name)
    : username(name), highest_level(1), best_steps_easy(0),
      best_steps_medium(0), best_steps_hard(0),
      total_undos_easy(0), total_undos_medium(0), total_undos_hard(0), created_at(time(nullptr)) {}

// ============================================================
// Create new user
// ============================================================
UserData createNewUser(const std::string &username) {
    return UserData(username);
}

// ============================================================
// Save user data to file
// Format (one user per line):
//   username highest_level easy_steps medium_steps hard_steps easy_undos medium_undos hard_undos
// Example: Alice 3 45 78 120 12 8 0
// ============================================================
bool saveUserData(const UserData &data, const std::string &filename) {
    std::vector<UserData> all_users = loadAllUsers(filename);

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

    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (const auto &user : all_users) {
        file << user.username << " "
             << user.highest_level << " "
             << user.best_steps_easy << " "
             << user.best_steps_medium << " "
             << user.best_steps_hard << " "
             << user.total_undos_easy << " "
             << user.total_undos_medium << " "
             << user.total_undos_hard << " "
             << user.created_at << "\n";
    }

    file.close();
    return true;
}

// ============================================================
// Load user data from file
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
               >> data.best_steps_hard
               >> data.total_undos_easy
               >> data.total_undos_medium
               >> data.total_undos_hard
               >> data.created_at;
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// ============================================================
// Check if username exists
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
// Load all users (used by saveUserData for update logic)
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
           >> user.best_steps_hard
           >> user.total_undos_easy
           >> user.total_undos_medium
           >> user.total_undos_hard
           >> user.created_at;

        if (!user.username.empty()) {
            users.push_back(user);
        }
    }

    file.close();
    return users;
}

// ============================================================
// Save game state (mid-game save)
// Format: level steps
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
// Load game state (continue saved progress)
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

// ============================================================
// Get leaderboard data
// Sort: difficulty DESC, (steps+undos) ASC
// ============================================================
std::vector<LeaderboardEntry> getLeaderboard(const std::string &filename) {
    std::vector<LeaderboardEntry> entries;
    std::vector<UserData> users = loadAllUsers(filename);

    for (const auto &user : users) {
        if (user.best_steps_hard > 0) {
            LeaderboardEntry e;
            e.username = user.username;
            e.difficulty = 3;
            e.best_steps = user.best_steps_hard;
            e.total_undos = user.total_undos_hard;
            e.created_at = user.created_at;
            entries.push_back(e);
        }
        if (user.best_steps_medium > 0) {
            LeaderboardEntry e;
            e.username = user.username;
            e.difficulty = 2;
            e.best_steps = user.best_steps_medium;
            e.total_undos = user.total_undos_medium;
            e.created_at = user.created_at;
            entries.push_back(e);
        }
        if (user.best_steps_easy > 0) {
            LeaderboardEntry e;
            e.username = user.username;
            e.difficulty = 1;
            e.best_steps = user.best_steps_easy;
            e.total_undos = user.total_undos_easy;
            e.created_at = user.created_at;
            entries.push_back(e);
        }
    }

    std::sort(entries.begin(), entries.end(), [](const LeaderboardEntry &a, const LeaderboardEntry &b) {
        if (a.difficulty != b.difficulty) return a.difficulty > b.difficulty;
        int score_a = a.best_steps + a.total_undos;
        int score_b = b.best_steps + b.total_undos;
        if (score_a != score_b) return score_a < score_b;
        return a.created_at < b.created_at;
    });

    return entries;
}
