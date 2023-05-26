#include "Kernel.h"
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <map>
#include "RemoteServer.h"
#include "Services.h"
#include "Kernel.h"
<<<<<<< HEAD
#include "cJSON.h"
#define PORT 8888
#define BACKLOG 128
#define MAX_FILE_SIZE 960
#define SEND_INTERVAL 1
=======
#define PORT 8888
#define BACKLOG 128
>>>>>>> origin/dev_wjc

RemoteServer server;

using namespace std;

void handle_pipe(int sig)
{
    // 不做任何处理即可
}

bool isNumber(const string &str)
{
    for (char const &c : str)
    {
        if (std::isdigit(c) == 0)
            return false;
    }
    return true;
}

stringstream print_head()
{
    stringstream send_str;
<<<<<<< HEAD
=======
    // send_str << "===============================================" << endl;
    // send_str << "||请在一行中依次输入需要调用的函数名称及其参数  ||" << endl;
    // send_str << "||open(char *name, int mode)                 ||" << endl;
    // send_str << "||close(int fd)                              ||" << endl;
    // send_str << "||read(int fd, int length)                   ||" << endl;
    // send_str << "||write(int fd, char *buffer, int length)    ||" << endl;
    // send_str << "||seek(int fd, int position, int ptrname)    ||" << endl;
    // send_str << "||mkfile(char *name, int mode)               ||" << endl;
    // send_str << "||rm(char *name)                             ||" << endl;
    // send_str << "||ls()                                       ||" << endl;
    // send_str << "||mkdir(char* dirname)                       ||" << endl;
    // send_str << "||cd(char* dirname)                          ||" << endl;
    // send_str << "||cat(char* dirname)                         ||" << endl;
    // send_str << "||copyin(char* ofpath, char *  ifpath)       ||" << endl;
    // send_str << "||copyout(char* ifpath, char *  ofpath)      ||" << endl;
    // send_str << "||q/Q 退出文件系统                           ||" << endl
    //          << endl
    //          << endl;
>>>>>>> origin/dev_wjc
    send_str <<"---------------------------------------------------------------"<<endl;
    send_str << ">>>help menu<<<" << endl;
    send_str << "open [filename] [mode] : to open a file with selected mode" << endl;
    send_str << "close [fd] : to close a file with selected fd" << endl;
    send_str << "read [fd] [length] : to read the file with selected fd" << endl;
    send_str << "write [fd] [text] : to write text into the file with selected fd" << endl;
    send_str << "lseek [fd] [position] [ptrname] : to seek the file with selected fd" << endl;
<<<<<<< HEAD
    send_str << "create [filename] : to create a file" << endl;
=======
    send_str << "create [filename] [mode] : to create a file with selected mode" << endl;
>>>>>>> origin/dev_wjc
    send_str << "rm [filename] : to remove a file" << endl;
    send_str << "ls : to list all files in current directory" << endl;
    send_str << "mkdir [dirname] : to create a directory" << endl;
    send_str << "cd [dirname] : to change current directory" << endl;
    send_str << "cat [filename] : to print the content of a file" << endl;
    send_str << "upload [localpath] [remotepath] : to upload a file from local to remote" << endl;
    send_str << "download [remotepath] [localpath] : to download a file from remote to local" << endl;
    send_str << "q/Q : to quit the file system" << endl;
    send_str <<"---------------------------------------------------------------"<<endl;


    return send_str;
}
class sendU
{
private:
    int fd;
    string username;

public:
    int send_(const stringstream &send_str)
    {
        // cout<<send_str.str()<<endl;
        int numbytes = send(fd, send_str.str().c_str(), send_str.str().length(), 0);
        cout << "[[ " << username << " ]] send " << numbytes << " bytes" << endl;
        cout << "====message send====" << endl;
        cout << send_str.str() << endl;
        cout << "====================" << endl;
        return numbytes;
    };
    sendU(int fd, string username)
    {
        this->fd = fd;
        this->username = username;
    };
<<<<<<< HEAD

    int send_command_to_json(const stringstream& send_str)
    {
        cJSON *command = cJSON_CreateObject();
        cJSON_AddStringToObject(command, "command", send_str.str().c_str());
        char *command_char = cJSON_Print(command);
        string command_strs = command_char;
        int numbytes = send(fd, command_strs.c_str(), command_strs.length(), 0);
        cout << "[[ " << username << " ]] send " << numbytes << " bytes" << endl;
        cout << "====message send====" << endl;
        cout << command_strs << endl;
        cout << "====================" << endl;
        return numbytes;
    }
=======
>>>>>>> origin/dev_wjc
};

void *start_routine(void *ptr)
{
    int fd = *(int *)ptr;
<<<<<<< HEAD
    char buf[MAX_PACKAGE_LENGTH];
    int numbytes;
    // numbytes=send(fd,"请输入用户名：",sizeof("请输入用户名："),0);
    cJSON *welcome = cJSON_CreateObject();
    cJSON_AddStringToObject(welcome, "command", "please type in the username:");
    char *welcome_char = cJSON_Print(welcome);
    string welcome_strs = welcome_char;

    numbytes = server.send_message(welcome_strs, fd);
=======
    char buf[1024];
    int numbytes;
    // numbytes=send(fd,"请输入用户名：",sizeof("请输入用户名："),0);
    numbytes = server.send_message("please type in the username:", fd);
>>>>>>> origin/dev_wjc
    if (numbytes == -1)
    {
        cout << "send() error" << endl;
        return (void *)NULL;
    }

<<<<<<< HEAD
    printf("进入用户线程，fd=%d\n", fd);

    memset(buf, 0, sizeof(buf));
    if ((numbytes = recv(fd, buf, MAX_PACKAGE_LENGTH, 0)) == -1)
=======
    printf("enter a user thread fd=%d\n", fd);

    memset(buf, 0, sizeof(buf));
    if ((numbytes = recv(fd, buf, 1024, 0)) == -1)
>>>>>>> origin/dev_wjc
    {
        cout << ("recv() error\n");
        return (void *)NULL;
    }
<<<<<<< HEAD
    cJSON *root = cJSON_Parse(buf);
    string username = cJSON_GetObjectItem(root, "command")->valuestring;
    
    
    cout << "[info] 用户输入用户名：" << username << endl;
=======

    string username = buf;
    cout << "[info] User name inputed" << username << endl;
>>>>>>> origin/dev_wjc

    sendU sd(fd, username);

    // 初始化用户User结构和目录
    Kernel::Instance().GetUserManager().Login(username);
    // init the prompt
    stringstream welcome_str = print_head();
    string tipswords = username + "@FileSystem:" + Kernel::Instance().GetUserManager().GetUser()->u_curdir + "$";

    // send the welcome message
    welcome_str << tipswords;
<<<<<<< HEAD
    // make welcome str to a cjson object
    // cJSON *welcome = cJSON_CreateObject();
    // cJSON_AddStringToObject(welcome, "command", welcome_str.str().c_str());
    // char *welcome_char = cJSON_Print(welcome);
    // stringstream welcome_stream;
    // welcome_stream << welcome_char;
    sd.send_command_to_json(welcome_str);
    bool first_output = true;
    while (true)
    {
        char buf_recv[MAX_PACKAGE_LENGTH] = {0};

        // 读取用户输入的命令行
        if ((numbytes = recv(fd, buf_recv, MAX_PACKAGE_LENGTH, 0)) == -1)
=======
    sd.send_(welcome_str);
    bool first_output = true;
    while (true)
    {
        char buf_recv[1024] = {0};

        // 读取用户输入的命令行
        if ((numbytes = recv(fd, buf_recv, 1024, 0)) == -1)
>>>>>>> origin/dev_wjc
        {
            cout << "recv() error" << endl;
            Kernel::Instance().GetUserManager().Logout();
            return (void *)NULL;
        }
<<<<<<< HEAD
        cout<<"recv from client of "<<numbytes<<" bytes"<<endl;
        cout<<"buf_recv : "<<buf_recv<<endl;
        cJSON *root = cJSON_Parse(buf_recv);
        cJSON *command = cJSON_GetObjectItem(root, "command");
        if (command == NULL)
        {
            cout << "command is null" << endl;
            if (numbytes <= 0)
            {
                cout << "[NETWORK] user " << username << " disconnect." << endl;
                Kernel::Instance().GetUserManager().Logout();
                return (void *)NULL;
            }
            printf("[NETWORK] send %d bytes\n", numbytes);
            cJSON_Delete(root);
            continue;
        }
        stringstream ss(command->valuestring);
=======
        // 解析命令名称
        stringstream ss(buf_recv);
>>>>>>> origin/dev_wjc
        stringstream send_str;

        cout << "buf_recv : " << buf_recv << endl;
        string api;
        ss >> api;

        cout << "api : " << api << endl;
<<<<<<< HEAD
        
        if (api == "q" || api == "quit")
        {
            Kernel::Instance().GetUserManager().Logout();
            send_str << "user logout\n";
=======
        // if(api == "cd"){
        //     // string param1;
        //     // ss >> param1;
        //     // if(param1 == ""){
        //     //     send_str<< "cd [fpath]";
        //     //     send_str << "invalid arguments" << endl;
        //     //     sd.send_(send_str);
        //     //     continue;
        //     // }
        //     // // 调用
        //     // User &u=Kernel::Instance().GetUser();
        // 	// u.u_error= NOERROR;
        // 	// char dirname[300]={0};
        //     // strcpy(dirname,param1.c_str());
        //     // u.u_dirp=dirname;
        //     // u.u_arg[0]=(unsigned long long)(dirname);
        //     // FileManager &fimanag = Kernel::Instance().GetFileManager();
        //     // fimanag.ChDir();
        //     // // 打印结果
        //     // send_str << "[result]:\n" << "now dir=" << dirname << endl;
        //     // sd.send_(send_str);
        //     // continue;
        //     send_str = Services::process(api,ss);
        //     continue;
        // }
        // if(api == "ls"){
        // 	User &u=Kernel::Instance().GetUser();
        // 	u.u_error=NOERROR;
        // 	string cur_path=u.u_curdir;
        // 	FD fd = Kernel::Instance().Sys_Open(cur_path,(File::FREAD));
        //     send_str << " cur_path:" << cur_path << endl;
        //     char buf[33]={0};
        // 	while(1){
        // 		if(Kernel::Instance().Sys_Read(fd, 32, 33, buf)==0)
        // 			break;
        // 		else{
        //             // send_str << "cur_path:" << cur_path << endl << "buf:" << buf;
        // 			DirectoryEntry *mm=(DirectoryEntry*)buf;
        // 			if(mm->m_ino==0)
        // 				continue;
        // 			send_str << "====== " << mm->m_name << " ======" << endl;
        // 			memset(buf, 0, 32);
        // 		}
        // 	}
        // 	Kernel::Instance().Sys_Close(fd);
        //     sd.send_(send_str);
        //     continue;
        // }
        // if(api == "mkdir"){
        //     string path;
        //     ss >> path;
        //     if(path == ""){
        //         send_str << "mkdir [dirpath]";
        //         send_str << "invalid arguments" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     int ret = Kernel::Instance().Sys_CreatDir(path);
        //     send_str<<"mkdir success (ret=" << ret << ")" <<endl;
        //     sd.send_(send_str);
        //     continue;
        // }
        // if(api == "mkfile"){
        //     string filename;
        //     ss >> filename;
        //     if(filename == ""){
        //         send_str << "mkfile [filepath]";
        //         send_str << "invalid arguments" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     User &u =Kernel::Instance().GetUser();
        // 	u.u_error = NOERROR;
        // 	u.u_ar0[0] = 0;
        //     u.u_ar0[1] = 0;
        //     char filename_char[512];
        // 	strcpy(filename_char,filename.c_str());
        //     u.u_dirp=filename_char;
        // 	u.u_arg[1] = Inode::IRWXU;
        // 	FileManager &fimanag=Kernel::Instance().GetFileManager();
        // 	fimanag.Creat();
        //     send_str<<"mkfile sucess"<<endl;
        //     sd.send_(send_str);
        //     continue;
        // }
        // if(api == "rm"){
        //     string filename;
        //     ss >> filename;
        //     if(filename == ""){
        //         send_str << "rm [filepath]";
        //         send_str << "invalid arguments" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     User &u =Kernel::Instance().GetUser();
        // 	u.u_error = NOERROR;
        // 	u.u_ar0[0] = 0;
        //     u.u_ar0[1] = 0;
        //     char filename_char[512];
        // 	strcpy(filename_char,filename.c_str());
        //     u.u_dirp=filename_char;
        // 	FileManager &fimanag=Kernel::Instance().GetFileManager();
        // 	fimanag.UnLink();
        //     send_str<<"rm success"<<endl;
        //     sd.send_(send_str);
        //     continue;
        // }
        // if(api == "seek"){
        //     string fd,position,ptrname;
        //     ss >> fd>>position>>ptrname;
        //     if(fd == ""||position==""||ptrname==""){
        //         send_str << "seek [fd] [position] [ptrname]";
        //         send_str << "invalid arguments" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     if(!isNumber(fd)){
        //         send_str << "[fd] 参数错误" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     int fd_int = atoi(fd.c_str());
        //     if(!isNumber(position)){
        //         send_str << "[position] 参数错误" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     int position_int = atoi(position.c_str());
        // 	if(!isNumber(ptrname)){
        //         send_str << "[ptrname] 参数错误" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     int ptrname_int = atoi(ptrname.c_str());
        //     User &u =Kernel::Instance().GetUser();
        // 	u.u_error = NOERROR;
        // 	u.u_ar0[0] = 0;
        //     u.u_ar0[1] = 0;
        // 	u.u_arg[0]=fd_int;
        // 	u.u_arg[1]=position_int;
        // 	u.u_arg[2]=ptrname_int;
        // 	FileManager &fimanag=Kernel::Instance().GetFileManager();
        // 	fimanag.Seek();
        // 	send_str<<"[Results:]\n"<<"u.u_ar0="<<u.u_ar0<<endl;
        //     sd.send_(send_str);
        //     continue;
        // }
        // if (api == "open"){
        //     // FD Kernel::Sys_Open(std::string& fpath,int mode)
        //     string param1;
        //     string param2;
        //     ss >> param1 >> param2;
        //     if(param1 == "" || param2 == ""){
        //         send_str << "open [fpath] [mode]\n";
        //         send_str << "invalid arguments" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     string fpath = param1;
        //     if(!isNumber(param2)){
        //         send_str << "[mode] 参数错误" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     int mode = atoi(param2.c_str());

        //     // 调用
        //     FD fd = Kernel::Instance().Sys_Open(fpath, mode);
        //     // 打印结果
        //     send_str << "[ return ]:\n" << "fd=" << fd << endl;
        //     sd.send_(send_str);
        //     continue;
        // }
        // if (api == "read")
        // {
        //     string p1_fd;
        //     string p2_size;
        //     ss >> p1_fd >> p2_size;
        //     if(p1_fd == "" || p2_size == ""){
        //         send_str << "read [fd] [size]\n";
        //         send_str << "invalid arguments" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     if(!isNumber(p1_fd)){
        //         send_str << "[fd] 参数错误" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     if(!isNumber(p2_size)){
        //         send_str << "[size] 参数错误" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     int fd = atoi(p1_fd.c_str());
        //     if(fd < 0){
        //         send_str << "[fd] 应当为正整数" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     int size = atoi(p2_size.c_str());
        //     if(size <= 0 || size > 1024){
        //         send_str << "[size] size 的取值范围是(0,1024]." << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     // 调用 API
        //     char buf[1025];
        //     memset(buf, 0, sizeof(buf));
        //     int ret = Kernel::Instance().Sys_Read(fd, size, 1025, buf);
        //     // 结果返回
        //     send_str << "[ return ]:\n"
        //          << "ret=" << ret << endl
        //          << buf << endl;
        //     sd.send_(send_str);
        //     continue;
        // }
        // if (api == "write")
        // {
        //     string p1_fd = "";
        //     string p2_content = "";
        //     ss >> p1_fd >> p2_content;
        //     if (p1_fd == "") {
        //         send_str << "write [fd] [content]\n";
        //         send_str << "invalid arguments" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     if (!isNumber(p1_fd)){
        //         send_str << "[fd] 参数错误" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     int fd = atoi(p1_fd.c_str());
        //     if(fd < 0){
        //         send_str << "[fd] 应当为正整数" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     if(p2_content.length() > 1024){
        //         send_str << "[content] 内容过长（不超过1024字节）" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     char buf[1025];
        //     memset(buf, 0, sizeof(buf));
        //     strcpy(buf, p2_content.c_str());
        //     int size = p2_content.length();
        //     // 调用 API
        //     int ret = Kernel::Instance().Sys_Write(fd, size, 1024, buf);
        //     // 打印结果
        //     send_str << "[ return ]\n" << "ret=" << ret << endl;
        //     sd.send_(send_str);
        //     continue;
        // }
        // if (api == "close")
        // {
        //     string p1_fd;
        //     ss >> p1_fd;
        //     if(p1_fd == ""){
        //         send_str << "close [fd]\n";
        //         send_str << "invalid arguments" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     if(!isNumber(p1_fd)){
        //         send_str <<  "[fd] 参数错误" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     int fd = atoi(p1_fd.c_str());
        //     if(fd < 0){
        //         send_str << "[fd] fd应当为正整数" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     // 调用 API
        //     int ret = Kernel::Instance().Sys_Close(fd);
        //     send_str << "[ return ]\n" << "ret=" << ret << endl;
        //     sd.send_(send_str);
        //     continue;
        // }
        // if (api == "cat"){
        //     string p1_fpath;
        //     ss >> p1_fpath;
        //     if(p1_fpath == "")
        //     {
        //         send_str << "cat [fpath]\n";
        //         send_str << "invalid arguments" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     string fpath = p1_fpath;
        //     // Open
        //     FD fd = Kernel::Instance().Sys_Open(fpath, 0x1);
        //     if(fd < 0){
        //         send_str << "[cat] 打开文件出错." << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     // Read
        //     char buf[257];
        //     while(true){
        //         memset(buf, 0, sizeof(buf));
        //         int ret = Kernel::Instance().Sys_Read(fd, 256, 256, buf);
        //         if(ret <= 0){
        //             break;
        //         }
        //         send_str << buf;
        //     }
        //     // Close
        //     Kernel::Instance().Sys_Close(fd);
        //     sd.send_(send_str);
        //     continue;
        // }
        // if (api == "copyin"){
        //     string p1_ofpath;
        //     string p2_ifpath;
        //     ss >> p1_ofpath >> p2_ifpath;
        //     if(p1_ofpath == "" || p2_ifpath == ""){
        //         send_str << "copyin ofpath ifpath\n";
        //         send_str << "invalid arguments" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     // 打开外部文件
        //     int ofd = open(p1_ofpath.c_str(), O_RDONLY); //只读方式打开外部文件
        //     if(ofd < 0){
        //         send_str << "[ERROR] failed to open file:" << p1_ofpath << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     // 创建内部文件
        //     Kernel::Instance().Sys_Creat(p2_ifpath, 0x1|0x2);
        //     int ifd = Kernel::Instance().Sys_Open(p2_ifpath, 0x1|0x2);
        //     if(ifd < 0){
        //         close(ofd);
        //         send_str << "[ERROR] failed to open file:" << p2_ifpath << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     // 开始拷贝，一次 256 字节
        //     char buf[256];
        //     int all_read_num = 0;
        //     int all_write_num = 0;
        //     while(true){
        //         memset(buf, 0, sizeof(buf));
        //         int read_num = read(ofd, buf, 256);
        //         if(read_num <= 0){
        //             break;
        //         }
        //         all_read_num += read_num;
        //         int write_num = \
        //             Kernel::Instance().Sys_Write(ifd, read_num, 256, buf);
        //         if(write_num <= 0){
        //             send_str << "[ERROR] 写入文件失败：" << p2_ifpath;
        //             break;
        //         }
        //         all_write_num += write_num;
        //     }
        //     send_str << "共读取字节：" << all_read_num
        //          << " 共写入字节：" << all_write_num << endl;
        //     close(ofd);
        //     Kernel::Instance().Sys_Close(ifd);
        //     sd.send_(send_str);
        //     continue;
        // }
        // if (api == "copyout"){
        //     string p1_ifpath;
        //     string p2_ofpath;
        //     ss >> p1_ifpath >> p2_ofpath;
        //     if (p1_ifpath == "" || p2_ofpath == "")
        //     {
        //         send_str << "copyout [ifpath] [ofpath]\n";
        //         send_str << "invalid arguments" << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     // 创建外部文件
        //     int ofd = open(p2_ofpath.c_str(), O_WRONLY| O_TRUNC | O_CREAT); //截断写入方式打开外部文件
        //     if (ofd < 0)
        //     {
        //         send_str << "[ERROR] 创建文件失败：" << p2_ofpath << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     // 打开内部文件
        //     int ifd = Kernel::Instance().Sys_Open(p1_ifpath, 0x1 | 0x2);
        //     if (ifd < 0)
        //     {
        //         close(ofd);
        //         send_str << "[ERROR] failed to open file:" << p1_ifpath << endl;
        //         sd.send_(send_str);
        //         continue;
        //     }
        //     // 开始拷贝，一次 256 字节
        //     char buf[256];
        //     int all_read_num = 0;
        //     int all_write_num = 0;
        //     while (true)
        //     {
        //         memset(buf, 0, sizeof(buf));
        //         int read_num = \
        //             Kernel::Instance().Sys_Read(ifd, 256, 256, buf);
        //         if (read_num <= 0)
        //         {
        //             break;
        //         }
        //         all_read_num += read_num;
        //         int write_num = write(ofd, buf, read_num);
        //         if (write_num <= 0)
        //         {
        //             send_str << "[ERROR] 写入文件失败：" << p1_ifpath;
        //             break;
        //         }
        //         all_write_num += write_num;
        //     }
        //     send_str << "共读取字节：" << all_read_num
        //          << " 共写入字节：" << all_write_num << endl;
        //     close(ofd);
        //     Kernel::Instance().Sys_Close(ifd);
        //     sd.send_(send_str);
        //     continue;
        // }
        if (api == "q" || api == "quit")
        {
            Kernel::Instance().GetUserManager().Logout();
            send_str << "User Logout\n";
>>>>>>> origin/dev_wjc
            sd.send_(send_str);
            break;
        }

<<<<<<< HEAD
        int code = 0;
        send_str = Services::process(api, ss, code, root);
        //check whether the root has "content"
        
=======
        int code;
        send_str = Services::process(api, ss, code);
>>>>>>> origin/dev_wjc
        if (code)
        {
            cout << "unrecognized command!" << endl;
            send_str = print_head();
            send_str << "\n"
                     << "wrong command!\n";
        }
        else
        {
            cout << "command finished!" << endl;
<<<<<<< HEAD
            if(cJSON_GetObjectItem(root,"content")!=NULL){
                cout<<"the root has content"<<endl;
            }
=======
>>>>>>> origin/dev_wjc
        }

        // show the prompt with username and current directory
        string tipswords = username + "@FileSystem:" + Kernel::Instance().GetUserManager().GetUser()->u_curdir + "$";

        send_str << tipswords;

        // 发送提示
<<<<<<< HEAD
        // cJSON_Delete(root);
        cJSON *response = cJSON_CreateObject();
        cJSON_AddStringToObject(response, "command", send_str.str().c_str());
        char *response_char = cJSON_Print(response);
        cout << "response_char : " << response_char << endl;
        stringstream response_str;
        response_str << response_char;
        cout <<response_str.str()<< endl;

        if(api=="download")
        {
            cout<<"attatch the file content"<<endl;
            string file_content_json=cJSON_GetObjectItem(root,"content")->valuestring;
            if(file_content_json=="")
            {
                cout<<"file content is null"<<endl;
                return;
            }
            printf("file_content_json : %s\n",file_content_json.c_str());
            int len = file_content_json.length();
            int totalNum = (len + MAX_FILE_SIZE - 1)/MAX_FILE_SIZE;
            int count = 0;
            int remainNum = totalNum;
            printf("len : %d\n",len);
            printf("totalNum : %d\n",totalNum);
            while(len>0)
            {
                cJSON *newresponse = cJSON_CreateObject();
                remainNum = remainNum - 1; 
                cJSON_AddStringToObject(newresponse, "content", file_content_json.substr(count,MAX_FILE_SIZE).c_str());
                cJSON_AddStringToObject(newresponse, "remainNum", to_string(remainNum).c_str());
                cJSON_AddStringToObject(newresponse, "totalNum", to_string(totalNum).c_str());
                cJSON_AddStringToObject(newresponse, "command", "download");
                char *newresponse_char = cJSON_Print(newresponse);  
                stringstream newresponse_str;
                newresponse_str << newresponse_char;
                cout <<newresponse_str.str()<< endl;
                numbytes = sd.send_(newresponse_str);
                len-=MAX_FILE_SIZE;
                count+=MAX_FILE_SIZE;
                sleep(SEND_INTERVAL);
            }
        }
        numbytes = sd.send_(response_str);
        // if (numbytes <= 0)
        // {
        //     cout << "[NETWORK] user " << username << " disconnect." << endl;
        //     Kernel::Instance().GetUserManager().Logout();
        //     return (void *)NULL;
        // }
        printf("[NETWORK] send %d bytes\n", numbytes);
        cJSON_Delete(root);
    }
=======
        numbytes = sd.send_(send_str);
        if (numbytes <= 0)
        {
            cout << "[info] User " << username << " connect broken." << endl;
            Kernel::Instance().GetUserManager().Logout();
            return (void *)NULL;
        }
        printf("[INFO] send %d bytes\n", numbytes);

        // if(api != "" && api != " "){
        //     stringstream tishi;
        //     tishi = print_head();
        //     tishi << "\n" << "温馨提示：您的指令错误！\n";
        //     sd.send_(tishi);
        //     continue;
        // }
    }

>>>>>>> origin/dev_wjc
    close(fd);
    return (void *)NULL;
}

<<<<<<< HEAD
=======
// int main()
// {

//     // 进行信号处理
//     struct sigaction action;
//     action.sa_handler = handle_pipe;
//     sigemptyset(&action.sa_mask);
//     action.sa_flags = 0;
//     sigaction(SIGPIPE, &action, NULL);

//     int listenfd, connectfd;
//     struct sockaddr_in server;
//     struct sockaddr_in client;
//     int sin_size;
//     sin_size=sizeof(struct sockaddr_in);

//     // 创建监听fd
//     if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
//         perror("Creating socket failed.");
//         exit(1);
//     }

//     int opt = SO_REUSEADDR;
//     setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //使得端口释放后立马被复用
//     bzero(&server,sizeof(server));
//     server.sin_family=AF_INET;
//     server.sin_port=htons(PORT);
//     server.sin_addr.s_addr = htonl (INADDR_ANY);
//     // 绑定
//     if (bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
//         perror("Bind error.");
//         exit(1);
//     }
//     // 监听
//     if(listen(listenfd,BACKLOG) == -1){  /* calls listen() */
//         perror("listen() error\n");
//         exit(1);
//     }
//     // 初始化文件系统
//     Kernel::Instance().Initialize();
//     cout << "[info] 等待用户接入..." << endl;
//     // 进入通信循环
//     while(1){
//         // accept
//         if ((connectfd = accept(listenfd,(struct sockaddr *)&client, (socklen_t*)&sin_size))==-1) {
//             perror("accept() error\n");
//             continue;
//         }
//         printf("客户端接入：%s\n",inet_ntoa(client.sin_addr) );
//         string str="hello";
//         //send(connectfd,str.c_str(),6,0);
//         pthread_t thread; //定义一个线程号
//         pthread_create(&thread,NULL,start_routine,(void *)&connectfd);
//     }
//     close(listenfd);
//     return 0;
// }

>>>>>>> origin/dev_wjc
int main()
{
    // init the server
    server.bind_process_function(start_routine);
    server.run();
    // init the file system
    Kernel::Instance().Initialize();
    while (true)
    {
        // get the connection
        if (-1 == server.wait_for_connection())
        {
            cout << "[ERROR] wait for connection failed" << endl;
            continue;
        }
    }
    return 0;
}
