#include "Kernel.h"
#include <string.h>

Kernel Kernel::instance;
bool Kernel::exist;

DiskDriver g_DiskDriver;
BufferManager g_BufferManager;
FileSystem g_FileSystem;
FileManager g_FileManager;
User g_User;
UserManager g_UserManager;

char returnBuf[256];

Kernel::Kernel()
{
}
Kernel::~Kernel()
{
}

/**
 * @brief Initialize the kernel disk driver
 */
void Kernel::InitDiskDriver()
{
    this->m_DiskDriver = &g_DiskDriver;
    this->m_DiskDriver->Initialize(); // 进行img的初始化和mmap    
}

/**
 * @brief Initialize the kernel buffer manager
*/
void Kernel::InitBuffer()
{
    this->m_BufferManager = &g_BufferManager;
    this->m_BufferManager->Initialize(); // 进行img的初始化和mmap   
}

/**
 * @brief Initialize the kernel file system
*/
void Kernel::InitFileSystem()
{
    this->m_FileSystem = &g_FileSystem;
    this->m_FileSystem->Initialize();
    this->m_FileManager = &g_FileManager;
    this->m_FileManager->Initialize();
}

/**
 * @brief Initialize the kernel user
*/
void Kernel::InitUser()
{
    this->m_User = &g_User;
    this->m_UserManager = &g_UserManager;
    //unix中u_ar0指向寄存器，此处为其分配一段空间即可
    // this->m_User->u_ar0 = (unsigned int*)&returnBuf;
}

/**
 * @brief Initialize the kernel
*/
void Kernel::Initialize()
{
    InitBuffer();
    // init disk driver
    InitDiskDriver();
    InitFileSystem();
    InitUser();

    exist = m_DiskDriver->exist;
    // system initialize
    FileManager &fileMgr = Kernel::Instance().GetFileManager();
    fileMgr.rootDirInode = g_InodeTable.IGet(FileSystem::ROOTINO);
    fileMgr.rootDirInode->i_flag &= (~Inode::ILOCK);
	pthread_mutex_unlock(& fileMgr.rootDirInode->mutex);

    Kernel::Instance().GetFileSystem().LoadSuperBlock();
    User &us = Kernel::Instance().GetUser();
    us.u_cdir = g_InodeTable.IGet(FileSystem::ROOTINO);
    us.u_cdir->i_flag &= (~Inode::ILOCK);
	pthread_mutex_unlock(& us.u_cdir->mutex);
    strcpy(us.u_curdir, "/");
    if(!exist){
        string str="/usr";
        Systemcall::Sys_CreatDir(str);
        str="/dev";
        Systemcall::Sys_CreatDir(str);
        str="/bin";
        Systemcall::Sys_CreatDir(str);
        str="/etc";
        Systemcall::Sys_CreatDir(str);
        str="/tmp";
        Systemcall::Sys_CreatDir(str);
        str="/home";
        Systemcall::Sys_CreatDir(str);

    }

    cout << "Kernel: initialize end" << endl;
}

void Kernel::Quit()
{
    this->m_BufferManager->Bflush();
    this->m_FileManager->m_InodeTable->UpdateInodeTable();
    this->m_FileSystem->Update();
    this->m_DiskDriver->uninstall();
}

// get file system management object
Kernel& Kernel::Instance()
{
    return Kernel::instance;
}

DiskDriver &Kernel::GetDiskDriver()
{
    return *(this->m_DiskDriver);
}

BufferManager & Kernel::GetBufferManager()
{
    return *(this->m_BufferManager);
}

FileSystem & Kernel::GetFileSystem()
{
    return *(this->m_FileSystem);
}

FileManager &Kernel::GetFileManager()
{
    return *(this->m_FileManager);
}

User &Kernel::GetUser()
{
    return *(this->m_UserManager->GetUser());
}

UserManager& Kernel::GetUserManager()
{
    return *(this->m_UserManager);
}

