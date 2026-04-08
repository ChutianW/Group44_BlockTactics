#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <vector>

using namespace std;

// User data structure
struct UserData {
    string username;
    int highest_level;      // 1=Easy, 2=Medium, 3=Hard
    int best_steps_easy;
    int best_steps_medium;
    int best_steps_hard;

    // Constructor
    UserData();
    UserData(const string& name);
};

// File I/O functions
bool loadUserData(const string& username, UserData& data);
bool saveUserData(const UserData& data);
bool userExists(const string& username);
vector<UserData> loadAllUsers();

// File path
const string USER_DATA_FILE = "data/user_data.txt";

#endif // FILE_IO_H
