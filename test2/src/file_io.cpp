#include "file_io.h"
#include <fstream>
#include <sstream>

// Default constructor
UserData::UserData() 
    : username(""), highest_level(1), best_steps_easy(0), 
      best_steps_medium(0), best_steps_hard(0) {}

// Constructor with username
UserData::UserData(const string& name) 
    : username(name), highest_level(1), best_steps_easy(0), 
      best_steps_medium(0), best_steps_hard(0) {}

// Check if user exists in file
bool userExists(const string& username) {
    ifstream file(USER_DATA_FILE);
    if (!file.is_open()) {
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string stored_name;
        ss >> stored_name;
        if (stored_name == username) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// Load user data from file
bool loadUserData(const string& username, UserData& data) {
    ifstream file(USER_DATA_FILE);
    if (!file.is_open()) {
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string stored_name;
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

// Save user data to file
bool saveUserData(const UserData& data) {
    // Load all existing users
    vector<UserData> all_users = loadAllUsers();

    // Update or add user
    bool found = false;
    for (auto& user : all_users) {
        if (user.username == data.username) {
            user = data;
            found = true;
            break;
        }
    }

    if (!found) {
        all_users.push_back(data);
    }

    // Write all users back to file
    ofstream file(USER_DATA_FILE);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& user : all_users) {
        file << user.username << " "
             << user.highest_level << " "
             << user.best_steps_easy << " "
             << user.best_steps_medium << " "
             << user.best_steps_hard << "\n";
    }

    file.close();
    return true;
}

// Load all users from file
vector<UserData> loadAllUsers() {
    vector<UserData> users;
    ifstream file(USER_DATA_FILE);

    if (!file.is_open()) {
        return users;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
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
