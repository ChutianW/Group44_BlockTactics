#include "file_io.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <ctime>

/*
 * Function: UserData::UserData (default constructor)
 * Purpose:  Initializes a UserData object with empty username and all stats at 0.
 * Inputs:   None.
 * Output:   A UserData with username="", highest_level=1, all step/undo counts 0,
 *           and created_at=0.
 */
UserData::UserData()
    : username(""), highest_level(1), best_steps_easy(0),
      best_steps_medium(0), best_steps_hard(0),
      total_undos_easy(0), total_undos_medium(0), total_undos_hard(0), created_at(0) {}

/*
 * Function: UserData::UserData (named constructor)
 * Purpose:  Creates a new UserData for a first-time user with the given username.
 *           Sets created_at to the current system time.
 * Inputs:   name - the username string for the new user.
 * Output:   A UserData with the given username, highest_level=1, all stats at 0,
 *           and created_at set to the current Unix timestamp.
 */
UserData::UserData(const std::string &name)
    : username(name), highest_level(1), best_steps_easy(0),
      best_steps_medium(0), best_steps_hard(0),
      total_undos_easy(0), total_undos_medium(0), total_undos_hard(0), created_at(time(nullptr)) {}

/*
 * Function: createNewUser
 * Purpose:  Factory function that creates and returns a new UserData for a player
 *           who is registering for the first time.
 * Inputs:   username - the player's chosen username string.
 * Output:   Returns a UserData constructed via UserData(username) with all
 *           stats initialized to zero and created_at set to current time.
 */
UserData createNewUser(const std::string &username) {
    return UserData(username);
}

/*
 * Function: saveUserData
 * Purpose:  Persists a UserData record to the data file. If the user already
 *           exists, their record is updated in place; otherwise it is appended.
 *           File format (one user per line):
 *           username highest_level easy_steps medium_steps hard_steps
 *           easy_undos medium_undos hard_undos created_at
 * Inputs:   data     - the UserData object to save.
 *           filename - path to the data file (default: data/user_data.txt).
 * Output:   Returns true if the file was written successfully; false on I/O error.
 */
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

/*
 * Function: loadUserData
 * Purpose:  Reads a single user's data record from the data file by username.
 * Inputs:   username - the username to search for.
 *           data     - output parameter filled with the loaded UserData.
 *           filename - path to the data file (default: data/user_data.txt).
 * Output:   Returns true and populates data if the user is found.
 *           Returns false if the file cannot be opened or the user is not found.
 */
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

/*
 * Function: userExists
 * Purpose:  Checks whether a given username already has a record in the data file.
 *           Used during login to distinguish new vs. returning users.
 * Inputs:   username - the username to search for.
 *           filename - path to the data file (default: data/user_data.txt).
 * Output:   Returns true if a matching username line is found; false otherwise.
 */
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

/*
 * Function: loadAllUsers
 * Purpose:  Reads all user records from the data file into a vector.
 *           Used internally by saveUserData to perform an in-place update.
 * Inputs:   filename - path to the data file (default: data/user_data.txt).
 * Output:   Returns a vector of UserData objects for every valid line in the file.
 *           Returns an empty vector if the file cannot be opened.
 */
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

/*
 * Function: saveGame
 * Purpose:  Writes a minimal mid-game checkpoint (level and step count) to a file.
 *           Format: "level steps" on a single line.
 * Inputs:   filename - path to the checkpoint file.
 *           level    - the current difficulty level as an integer.
 *           steps    - the current step count to save.
 * Output:   Returns true if the file was written successfully; false on I/O error.
 */
bool saveGame(const std::string &filename, int level, int steps) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << level << " " << steps << "\n";
    file.close();
    return true;
}

/*
 * Function: loadGame
 * Purpose:  Reads a previously saved game checkpoint file to restore level and steps.
 * Inputs:   filename - path to the checkpoint file to read.
 *           level    - output parameter set to the saved difficulty level.
 *           steps    - output parameter set to the saved step count.
 * Output:   Returns true and populates level and steps if successful.
 *           Returns false if the file cannot be opened.
 */
bool loadGame(const std::string &filename, int &level, int &steps) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file >> level >> steps;
    file.close();
    return true;
}

/*
 * Function: getLeaderboard
 * Purpose:  Builds and returns a sorted leaderboard from all user records.
 *           Each user contributes one entry per difficulty level they have completed.
 *           Sort order: difficulty DESC (Hard first), then (steps + undos) ASC,
 *           then creation date ASC as a tiebreaker.
 * Inputs:   filename - path to the data file (default: data/user_data.txt).
 * Output:   Returns a sorted vector of LeaderboardEntry structs ready for display.
 */
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
