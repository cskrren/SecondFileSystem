#pragma once
#include "Kernel.h"
#include "File.h"
class Systemcall
{
public:
    static FD Sys_Open(std::string& fpath,int mode=File::FWRITE);
    static int Sys_Close(FD fd);
    static int Sys_CreatDir(std::string &fpath);
    static int Sys_Creat(std::string &fpath,int mode=File::FWRITE);
    static int Sys_Delete(std::string &fpath);
    static int Sys_Read(FD fd, size_t size, size_t nmemb, void* ptr);
    static int Sys_Write(FD fd, size_t size, size_t nmemb, void* ptr);
    static int Sys_Seek(FD fd, long int offset, int whence);
private:
    Systemcall(){};
};