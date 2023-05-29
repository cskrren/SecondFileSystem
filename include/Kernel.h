#ifndef KERNEL_H
#define KERNEL_H

#include <string>
#include "DiskDriver.h"

#include "Systemcall.h"
#include "FileManager.h"
#include "FileSystem.h"
#include "BufferManager.h"
#include "User.h"
#include "UserManager.h"




class Kernel
{
public:
    Kernel();
    ~Kernel();
    static Kernel& Instance();
    void Initialize();
    void Quit();
    DiskDriver& GetDiskDriver();
    BufferManager& GetBufferManager();
    FileSystem& GetFileSystem();
    FileManager& GetFileManager();
    User& GetSuperUser();
    User& GetUser();
    UserManager& GetUserManager();

private:
    void InitDiskDriver();
    void InitFileSystem();
    void InitBuffer();
    void InitUser();

private:
    static Kernel instance;

    DiskDriver* m_DiskDriver;
    BufferManager*  m_BufferManager;
    FileSystem*  m_FileSystem;
    FileManager* m_FileManager;
    User* m_User;
    UserManager* m_UserManager;
public:
    static bool exist;
};
#endif

