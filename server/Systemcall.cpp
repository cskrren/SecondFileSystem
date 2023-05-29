#include "Kernel.h"
#include "User.h"
#include"string.h"
#include"string"

FD Systemcall::Sys_Open(std::string& fpath,int mode)
{
    User& u = Kernel::Instance().GetUser();
    char path[256];
    strcpy(path,fpath.c_str());
	u.u_dirp = path;
    // put the arguments into user structure
    u.u_arg[1] = mode;

    FileManager& fileMgr = Kernel::Instance().GetFileManager();
	fileMgr.Open();

	return u.u_ar0[User::EAX];	

}

int Systemcall::Sys_Close(FD fd)
{
    User& u = Kernel::Instance().GetUser();
    u.u_arg[0] = fd;

    FileManager& fileMgr = Kernel::Instance().GetFileManager();
	fileMgr.Close();

    return u.u_ar0[User::EAX];
}

int Systemcall::Sys_CreatDir(std::string &fpath)
{
    int default_mode = 040755;
    User &u = Kernel::Instance().GetUser();
    u.u_error = NOERROR;
    char filename_char[300]={0};
    strcpy(filename_char, fpath.c_str());
    u.u_dirp = filename_char;
    u.u_arg[1] = default_mode;
    u.u_arg[2] = 0;
    FileManager &fimanag = Kernel::Instance().GetFileManager();
    fimanag.MkNod();
    return u.u_ar0[User::EAX];
}

int Systemcall::Sys_Creat(std::string &fpath,int mode)
{
    User& u = Kernel::Instance().GetUser();
    char path[256];
    strcpy(path,fpath.c_str());
	u.u_dirp = path;
    u.u_arg[0] = (long long)&path;
    u.u_arg[1] = mode;

    FileManager& fileMgr = Kernel::Instance().GetFileManager();
	fileMgr.Creat();

	return u.u_ar0[User::EAX];	
}

int Systemcall::Sys_Delete(std::string &fpath)
{
    User& u = Kernel::Instance().GetUser();
    char path[256];
    strcpy(path,fpath.c_str());
	u.u_dirp = path;
    u.u_arg[0] = (long long)&path;

    FileManager& fileMgr = Kernel::Instance().GetFileManager();
	fileMgr.UnLink();

	return u.u_ar0[User::EAX];	
}

int Systemcall::Sys_Read(FD fd, size_t size, size_t nmemb, void *ptr)
{
    if(size>nmemb)return -1;
    User& u = Kernel::Instance().GetUser();

    u.u_arg[0] = fd;
    u.u_arg[1] = (long long)ptr;
    u.u_arg[2] = size;

    FileManager& fileMgr = Kernel::Instance().GetFileManager();
	fileMgr.Read();

	return u.u_ar0[User::EAX];	

}

int Systemcall::Sys_Write(FD fd, size_t size, size_t nmemb, void *ptr)
{
    if(size>nmemb)return -1;
    User& u = Kernel::Instance().GetUser();

    u.u_arg[0] = fd;
    u.u_arg[1] = (unsigned long long)ptr;
    u.u_arg[2] = size;

    FileManager& fileMgr = Kernel::Instance().GetFileManager();
	fileMgr.Write();

	return u.u_ar0[User::EAX];	
}

int Systemcall::Sys_Seek(FD fd, long int offset, int whence)
{
    User& u = Kernel::Instance().GetUser();

    u.u_arg[0] = fd;
    u.u_arg[1] = offset;
    u.u_arg[2] = whence;

    FileManager& fileMgr = Kernel::Instance().GetFileManager();
	fileMgr.Seek();

	return u.u_ar0[User::EAX];	
}
