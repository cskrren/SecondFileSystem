#include "Kernel.h"
#include "UserManager.h"

UserManager::UserManager()
{
    // clean all the user space
    for (int i = 0; i < USER_N; ++i)
    {
        (this->user_pool)[i] = NULL;
    }
    this->map_thread_user.clear();
    // alloc a space for the first user(root)
    pthread_t pid = pthread_self();
    user_pool[0] = (User*)malloc(sizeof(User));
    map_thread_user[pid] = 0;

}

UserManager::~UserManager()
{
    for (int i = 0; i < USER_N; ++i)
    {
        if ((this->user_pool)[i] != NULL)
            free((this->user_pool)[i]);
    }
}

/**
 * @brief User Login
*/
bool UserManager::Login(string uname)
{
    // get thread id
    pthread_t pthread_id = pthread_self();
    // check if the thread has already login
    if (map_thread_user.find(pthread_id) != map_thread_user.end())
    {
        printf("[ERROR] Thread %llu has already login\n", pthread_id);
        return false;
    }
    // get a free space for user
    int i;
    for (i = 0; i < USER_N; ++i)
    {
        if (user_pool[i] == NULL)
        {
            break;
        }
    }
    if (i == USER_N)
    {
        printf("[ERROR] UserManager has no free space\n");
        return false;
    }
    // i is the free space
    user_pool[i] = (User *)malloc(sizeof(User));
    if (user_pool[i] == NULL)
    {
        printf("[ERROR] UserManager malloc failed\n");
        return false;
    }
    // link the thread id and the user
    map_thread_user[pthread_id] = i;
    user_pool[i]->u_uid = 0;
    printf("-> Thread %llu login successed.\n", pthread_id);
    // set the user's current directory
    // find the root index first
    user_pool[i]->u_cdir = g_InodeTable.IGet(FileSystem::ROOTINO);
    user_pool[i]->u_cdir->NFrele();
    strcpy(user_pool[i]->u_curdir, "/");
    // try to create the user's home directory
    Systemcall::Sys_CreatDir(uname);
    // turn to the home directory
    user_pool[i]->u_error = NOERROR;
    char dirname[512] = {0};
    strcpy(dirname, uname.c_str());
    user_pool[i]->u_dirp = dirname;
    user_pool[i]->u_arg[0] = (unsigned long long)(dirname);
    FileManager &fimanag = Kernel::Instance().GetFileManager();
    fimanag.ChDir();
    return true;
}

/**
 * @brief User Logout
*/
bool UserManager::Logout()
{
    // sync the information in Critial Section
    Kernel::Instance().Quit();

    // get thread id
    pthread_t pthread_id = pthread_self();
    if (map_thread_user.find(pthread_id) == map_thread_user.end())
    {
        printf("[ERROR] Thread %d has not login\n", pthread_id);
        return false;
    }
    int i = map_thread_user[pthread_id];
    free(user_pool[i]);
    map_thread_user.erase(pthread_id);
    printf("-> Thread %d Logout successd.\n", pthread_id);
    return true;
}

/**
 * @brief Get the User Structure of the current thread
*/
User *UserManager::GetUser()
{
    pthread_t pthread_id = pthread_self();
    if (map_thread_user.find(pthread_id) == map_thread_user.end())
    {
        printf("[ERROR] Could not get the User Structure of thread %d\n", pthread_id);
        exit(1);
    }
    return user_pool[map_thread_user[pthread_id]];
}
