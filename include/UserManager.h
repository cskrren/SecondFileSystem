#ifndef USERMG_H
#define USERMG_H
#define NOERROR 0
typedef int ErrorCode;
#include "FileManager.h"
#include "User.h"
#include "File.h"
#include "INode.h"
#include <iostream>
#include <string.h>
#include <map>
using namespace std;

class UserManager
{
public:
    static const int USER_N = 100;
    UserManager();
    ~UserManager();
    // User Login
    bool Login(string uname);
    // User Logout
    bool Logout();
    // Get User Structure of the current thread
    User* GetUser();

public:
    // connect between thread and user
    std::map<pthread_t, int> map_thread_user;
    // User array
    User* user_pool[USER_N]; 
};

#endif
