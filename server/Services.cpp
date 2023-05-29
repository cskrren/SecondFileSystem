#include "Services.h"
#include "base64.h"
#include "Systemcall.h"
#include <vector>
#define MAX_FILE_SIZE 960
#define SEND_INTERVAL 1
using namespace std;
const int buf_size = 256;

map<string, stringstream (Services::*)(stringstream &,cJSON*)> Services::command_service_map = {
    {"open", &Services::open_service},
    {"close", &Services::close_service},
    {"read", &Services::read_service},
    {"write", &Services::write_service},
    {"lseek", &Services::lseek_service},
    {"create", &Services::create_service},
    {"rm", &Services::rm_service},
    {"ls", &Services::ls_service},
    {"mkdir", &Services::mkdir_service},
    {"cd", &Services::cd_service},
    {"cat", &Services::cat_service},
    {"upload", &Services::upload_service},
    {"download", &Services::download_service}};

stringstream Services::open_service(stringstream &ss,cJSON* root)
{
    string open_filename;
    string open_mode;
    stringstream ret;
    ss >> open_filename >> open_mode;
    if (open_filename == "" || open_mode == "")
    {
        ret << "open [filename] [mode]\n";
        ret << "invalid arguments" << endl;
        return ret;
    }
    string fpath = open_filename;
    if (!Utility::is_number(open_mode))
    {
        ret << "[mode] invalid" << endl;
        return ret;
    }
    int mode = atoi(open_mode.c_str());

    
    FD fd;
    fd = Systemcall::Sys_Open(fpath, mode);
    if (Kernel::Instance().GetUser().u_error != 0)
    {
        ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
        return ret;
    }
    
    ret << "The file opened successfully\n"
             << "open file [fd] = " << fd << endl;
    return ret;
}

stringstream Services::close_service(stringstream &ss,cJSON* root)
{
    string openfile_index;
    stringstream ret;
    ss >> openfile_index;
    if (openfile_index == "")
    {
        ret << "close [fd]\n";
        ret << "invalid arguments" << endl;
        return ret;
    }
    if (!Utility::is_number(openfile_index))
    {
        ret << "[fd] should be a number" << endl;
        return ret;
    }
    int fd = atoi(openfile_index.c_str());
    if (fd < 0)
    {
        ret << "[fd] should be positive" << endl;
        return ret;
    }
    
    int close_return = Systemcall::Sys_Close(fd);
    if (Kernel::Instance().GetUser().u_error != 0)
    {
        ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
        return ret;
    }
    ret << "The file opened successfully\n"
             << "close file [fd] = " << close_return << endl;
    return ret;
}

stringstream Services::read_service(stringstream &ss,cJSON* root)
{
    string openfile_index;
    string read_size;
    stringstream ret;
    ss >> openfile_index >> read_size;
    if (openfile_index == "" || read_size == "")
    {
        ret << "read [fd] [length]\n";
        ret << "invalid arguments" << endl;

        return ret;
    }
    if (!Utility::is_number(openfile_index))
    {
        ret << "[fd] should be a number" << endl;

        return ret;
    }
    if (!Utility::is_number(read_size))
    {
        ret << "[length] should be a number" << endl;

        return ret;
    }
    int fd = atoi(openfile_index.c_str());
    if (fd < 0)
    {
        ret << "[fd] should be positive" << endl;

        return ret;
    }
    int size = atoi(read_size.c_str());
    if (size <= 0 || size > 1024)
    {
        ret << "[length] valid range:(0,1024]" << endl;

        return ret;
    }
    
    char buf[1025];
    memset(buf, 0, sizeof(buf));
    int read_return = Systemcall::Sys_Read(fd, size, 1025, buf);
    if (Kernel::Instance().GetUser().u_error != 0)
    {
        ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
        return ret;
    }
    
    ret << "successfully read " << read_return << " bytes from file of fd: " << fd << endl;
    ret << "content:" << endl;
    ret << buf << endl;
    return ret;
}

stringstream Services::write_service(stringstream &ss,cJSON* root)
{
    string openfile_index = "";
    string write_content = "";
    stringstream ret;
    ss >> openfile_index >> write_content;
    if (openfile_index == "")
    {
        ret << "write [fd] [text]\n";
        ret << "invalid arguments" << endl;
        return ret;
    }
    if (!Utility::is_number(openfile_index))
    {
        ret << "[fd] should be a number" << endl;
        return ret;
    }
    int fd = atoi(openfile_index.c_str());
    if (fd < 0)
    {
        ret << "[fd] should be positive" << endl;
        return ret;
    }
    if (write_content.length() > 1024)
    {
        ret << "[text] is too long" << endl;
        return ret;
    }
    char buf[1025];
    memset(buf, 0, sizeof(buf));
    strcpy(buf, write_content.c_str());
    int size = write_content.length();
    int write_return = Systemcall::Sys_Write(fd, size, 1024, buf);
    if (Kernel::Instance().GetUser().u_error != 0)
    {
        ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
        return ret;
    }
    ret << "successfully write " << write_return << " bytes to file [fd] = " << fd << endl;
    ret << "content:" << endl;
    ret << buf << endl;
    return ret;
}

stringstream Services::lseek_service(stringstream &ss,cJSON* root)
{
    string fd, position, locate_mode;
    stringstream ret;
    ss >> fd >> position >> locate_mode;
    if (fd == "" || position == "" || locate_mode == "")
    {
        ret << "lseek [fd] [position] [locate_mode]";
        ret << "invalid arguments" << endl;
        return ret;
    }
    if (!Utility::is_number(fd))
    {
        ret << "[fd] invalid" << endl;
        return ret;
    }
    int fd_int = atoi(fd.c_str());
    if (!Utility::is_number(position))
    {
        ret << "[position] invalid" << endl;
        return ret;
    }
    int position_int = atoi(position.c_str());
    if (!Utility::is_number(locate_mode))
    {
        ret << "[locate_mode] invalid" << endl;
        return ret;
    }
    int locate_mode_int = atoi(locate_mode.c_str());
    User &u = Kernel::Instance().GetUser();
    u.u_error = NOERROR;
    u.u_ar0[0] = 0;
    u.u_ar0[1] = 0;
    u.u_arg[0] = fd_int;
    u.u_arg[1] = position_int;
    u.u_arg[2] = locate_mode_int;
    FileManager &fimanag = Kernel::Instance().GetFileManager();
    fimanag.Seek();
    if (Kernel::Instance().GetUser().u_error != 0)
    {
        ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
        return ret;
    }
    ret << "successfully seek file [fd] = " << fd_int << " to position " << position_int << "in locate_mode " << locate_mode_int << endl;
    ret << "new position: " << u.u_ar0[0] << endl;
    return ret;
}

stringstream Services::create_service(stringstream &ss,cJSON* root)
{
    string create_filename;
    stringstream ret;
    ss >> create_filename;
    if (create_filename == "")
    {
        ret << "mkfile [filepath]";
        ret << "invalid arguments" << endl;
        return ret;
    }
    User &u = Kernel::Instance().GetUser();
    u.u_error = NOERROR;
    u.u_ar0[0] = 0;
    u.u_ar0[1] = 0;
    char create_filename_char[512];
    strcpy(create_filename_char, create_filename.c_str());
    u.u_dirp = create_filename_char;
    u.u_arg[1] = Inode::IRWXU;
    FileManager &fimanag = Kernel::Instance().GetFileManager();
    fimanag.Creat();
    if (Kernel::Instance().GetUser().u_error != 0)
    {
        ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
        return ret;
    }
    ret << "successfully create file " << create_filename << endl;
    ret << "inode number: " << u.u_ar0[0] << endl;
    return ret;
}

stringstream Services::cd_service(stringstream &ss,cJSON* root)
{
    stringstream ret;
    string cd_path;
    ss >> cd_path;
    if (cd_path == "")
    {
        ret << "cd [fpath]";
        ret << "invalid arguments" << endl;
        return ret;
    }
    
    User &u = Kernel::Instance().GetUser();
    u.u_error = NOERROR;
    char dirname[300] = {0};
    strcpy(dirname, cd_path.c_str());
    u.u_dirp = dirname;
    u.u_arg[0] = (unsigned long long)(dirname);
    FileManager &fimanag = Kernel::Instance().GetFileManager();

    fimanag.ChDir();

    if (Kernel::Instance().GetUser().u_error != 0)
    {
        ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
        return ret;
    }
    
    ret << "successfully change directory to " << cd_path << endl;
    ret << "current directory: " << u.u_cdir->i_number << endl;
    return ret;
}

stringstream Services::rm_service(stringstream &ss,cJSON* root)
{
    string rm_filename;
    stringstream ret;
    ss >> rm_filename;
    if (rm_filename == "")
    {
        ret << "rm [rm_filename]";
        ret << "invalid arguments" << endl;
        return ret;
    }
    User &u = Kernel::Instance().GetUser();
    u.u_error = NOERROR;
    u.u_ar0[0] = 0;
    u.u_ar0[1] = 0;
    char rm_filename_char[512];
    strcpy(rm_filename_char, rm_filename.c_str());
    u.u_dirp = rm_filename_char;
    FileManager &fimanag = Kernel::Instance().GetFileManager();
    fimanag.UnLink();
    if (Kernel::Instance().GetUser().u_error != 0)
    {
        ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
        return ret;
    }
    ret << "successfully remove file " << rm_filename << endl;
    return ret;
}

stringstream Services::ls_service(stringstream &ss,cJSON* root)
{
    stringstream ret;
    User &u = Kernel::Instance().GetUser();
    u.u_error = NOERROR;
    string cur_path = u.u_curdir;
    FD fd = Systemcall::Sys_Open(cur_path, (File::FREAD));
    // error handle
    if (Kernel::Instance().GetUser().u_error != 0)
    {
        ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
        return ret;
    }
    ret << " cur_path:" << cur_path << endl;
    char buf[33] = {0};
    while (1)
    {
        if (Systemcall::Sys_Read(fd, 32, 33, buf) == 0)
            break;
        else
        {
            DirectoryEntry *mm = (DirectoryEntry *)buf;
            if (mm->m_ino == 0)
                continue;
            ret << mm->m_name << endl;

            memset(buf, 0, 32);
        }
    }
    Systemcall::Sys_Close(fd);
    ret << "ls success" << endl;
    return ret;
}

stringstream Services::mkdir_service(stringstream &ss,cJSON* root)
{
    stringstream ret;
    string dir_path;
    ss >> dir_path;
    if (dir_path == "")
    {
        ret << "mkdir [dirname]";
        ret << "invalid arguments" << endl;
        return ret;
    }
    ret << "doing mkdir " << dir_path << endl;
    int create_return = Systemcall::Sys_CreatDir(dir_path);
    if (Kernel::Instance().GetUser().u_error != 0)
    {
        ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
        return ret;
    }
    ret << "mkdir success" << endl;
    ret << "inode number: " << create_return << endl;
    return ret;
}

stringstream Services::cat_service(stringstream &ss,cJSON* root)
{
    string cat_path;
    stringstream ret;
    ss >> cat_path;
    if (cat_path == "")
    {
        ret << "cat [filename]\n";
        ret << "invalid arguments" << endl;
        return ret;
    }
    string fpath = cat_path;
    // Open
    FD fd = Systemcall::Sys_Open(fpath, 0x1);

    // error handle
    if (Kernel::Instance().GetUser().u_error != 0)
    {
        ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
        return ret;
    }

    if (fd < 0)
    {
        ret << "cannot open file." << endl;
        return ret;
    }
    // Read
    char buf[257];
    while (true)
    {
        memset(buf, 0, sizeof(buf));
        int read_ret = Systemcall::Sys_Read(fd, 256, 256, buf);
        if (read_ret <= 0)
        {
            break;
        }
        ret << buf;
    }
    // add endl
    ret<<endl;
    // Close
    Systemcall::Sys_Close(fd);
    ret << "cat success" << endl;
    return ret;
}

void Services::save_file()
{
    int fd = Systemcall::Sys_Open(uploadFileName, 0x1 | 0x2);
    if (fd < 0)
    {
        cout << "cannot open file." << endl;
        return;
    }
    //start copying file using buf
    totalStr = base64_decode(totalStr);
    int all_read_num = totalStr.length();
    int all_write_num = 0;
    while (true)
    {
        int current_upload_num=min(buf_size,(int)totalStr.length() - all_write_num);
        int write_num = Systemcall::Sys_Write(fd, current_upload_num, buf_size, (void*)(totalStr.c_str()+all_write_num));
        all_write_num += write_num;
        printf("write_num:%d\n",write_num);
        if(write_num<=0 || all_write_num >= totalStr.length())
        {
            break;
        }
    }
    Systemcall::Sys_Close(fd);
}

stringstream Services::upload_service(stringstream &ss, cJSON* root)
{
    stringstream ret;
    if(!cJSON_HasObjectItem(root,"fileNum")){
        string p2_ifpath = cJSON_GetObjectItem(root,"filename")->valuestring;
        string file_content_raw = cJSON_GetObjectItem(root,"content")->valuestring;
        string remainNumStr = cJSON_GetObjectItem(root,"remainNum")->valuestring;
        string totalNumStr = cJSON_GetObjectItem(root,"totalNum")->valuestring;
        int remainNum = atoi(remainNumStr.c_str());
        int totalNum = atoi(totalNumStr.c_str());
        Systemcall::Sys_Creat(p2_ifpath, 0x1 | 0x2);
        int ifd = Systemcall::Sys_Open(p2_ifpath, 0x1 | 0x2);
        // error
        if (Kernel::Instance().GetUser().u_error != 0)
        {
            ret << "error occur when open file" << endl;
            ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
            return ret;
        }
        if (ifd < 0)
        {
            // close(ofd);
            ret << "[ERROR] failed to open file:" << p2_ifpath << endl;
            return ret;
        }
        // start copying file using buf
        int all_read_num = file_content_raw.length();
        int all_write_num = 0;
        while (true)
        {
            int current_upload_num=min(buf_size,(int)file_content_raw.length() - all_write_num);
            int write_num = Systemcall::Sys_Write(ifd, current_upload_num, buf_size, (void*)(file_content_raw.c_str()+all_write_num));
            all_write_num += write_num;
            printf("write_num:%d\n",write_num);
            if(write_num<=0)
            {
                ret << "[ERROR] failed to write:" << p2_ifpath<<endl;
                break;
            }
            if(all_write_num >= file_content_raw.length())
            {
                break;
            }
        }
        ret << "Bytes read:" << all_read_num
                << "Bytes written:" << all_write_num 
                << "Remain Packets:" << remainNum
                << "Total Packets:" << totalNum << endl;
        Systemcall::Sys_Close(ifd);
        totalStr += file_content_raw;
        if(remainNum==0)
        {
            printf(totalStr.c_str());
            save_file();
        }
        return ret;    
    }
    else{
        string p1_ofpath;
        string p2_ifpath;
        ss >> p1_ofpath >> p2_ifpath;
        printf("p1_ofpath:%s\n",p1_ofpath.c_str());
        printf("p2_ifpath:%s\n",p2_ifpath.c_str());
        if (p1_ofpath == "" || p2_ifpath == "")
        {
            ret << "upload [localpath] [remotepath]\n";
            ret << "invalid arguments" << endl;
            return ret;
        }
        string fileNumStr = cJSON_GetObjectItem(root,"fileNum")->valuestring;
        int fileNum = atoi(fileNumStr.c_str());
        cout<<"receive fileNum:"<<fileNum<<endl;
        uploadFileName = p2_ifpath;
        totalStr = "";
        Systemcall::Sys_Creat(p2_ifpath, 0x1 | 0x2);
        // error
        if (Kernel::Instance().GetUser().u_error != 0)
        {
            ret << "error occur when create file" << endl;
            ret << get_error_msg(Kernel::Instance().GetUser().u_error) << endl;
            return ret;
        } 
        return ret;
    }
}

stringstream Services::download_service(stringstream &ss,cJSON* root)
{
    string path_from;
    string path_to;
    stringstream ret;
    ss >> path_from >> path_to;
    if (path_from == "" || path_to == "")
    {
        ret << "download [remotepath] [localpath]\n";
        ret << "invalid arguments" << endl;
        return ret;
    }
    // open inner file
    if(Kernel::Instance().GetUser().u_error!=0){
        ret<<get_error_msg(Kernel::Instance().GetUser().u_error)<<endl;
        return ret;
    }
    cout<<"try to open file:"<<path_from<<endl;
    int ifd = Systemcall::Sys_Open(path_from, 0x1 | 0x2);
    if (ifd < 0)
    {
        // close(ofd);
        ret << "[ERROR] failed to open file:" << path_from << endl;
        return ret;
    }
    char buf[2048];
    int all_read_num = 0;
    int all_write_num = 0;
    // string file_content;
    vector<char> file_content;
    while (true)
    {
        memset(buf, 0, sizeof(buf));
        int read_num =
            Systemcall::Sys_Read(ifd, buf_size, buf_size, buf);
        if (read_num <= 0)
        {
            break;
        }
        all_read_num += read_num;
        // file_content += buf;
        for(int i=0;i<read_num;i++)
        {
            file_content.push_back(buf[i]);
        }
    }
    cout<<"string byte length:"<<file_content.size()<<endl;
    string file_content_encode = base64_encode((const unsigned char*)file_content.data(),file_content.size());
    // write the file content
    cout<<"all_read_num:"<<all_read_num<<endl;
    cout<<"write file content to cjson:"<<file_content_encode<<endl;
    cJSON_AddStringToObject(root,"content",file_content_encode.c_str());
    return ret;
}

std::string Services::get_error_msg(int error_code)
{
    std::string error_msg;
    switch (error_code)
    {
    case 0:
        error_msg = "NOERROR";
        break;
    case EPERM:
        error_msg = "operation not permitted";
        break;
    case ENOENT:
        error_msg = "no such file or directory";
        break;
    case ESRCH:
        error_msg = "no such process";
        break;
    case EINTR:
        error_msg = "interrupted system call";
        break;
    case EIO:
        error_msg = "I/O error";
        break;
    case ENXIO:
        error_msg = "no such device or address";
        break;
    case E2BIG:
        error_msg = "argument list too long";
        break;
    case ENOEXEC:
        error_msg = "exec format error";
        break;
    case EBADF:
        error_msg = "bad file number";
        break;
    case ECHILD:
        error_msg = "no child processes";
        break;
    case EAGAIN:
        error_msg = "try again";
        break;
    case ENOMEM:
        error_msg = "out of memory";
        break;
    case EACCES:
        error_msg = "permission denied";
        break;
    case EFAULT:
        error_msg = "bad address";
        break;
    case ENOTBLK:
        error_msg = "block device required";
        break;
    case EBUSY:
        error_msg = "device or resource busy";
        break;
    case EEXIST:
        error_msg = "file exists";
        break;
    case EXDEV:
        error_msg = "cross-device link";
        break;
    case ENODEV:
        error_msg = "no such device";
        break;
    case ENOTDIR:
        error_msg = "not a directory";
        break;
    case EISDIR:
        error_msg = "is a directory";
        break;
    case EINVAL:
        error_msg = "invalid argument";
        break;
    case ENFILE:
        error_msg = "file table overflow";
        break;
    case EMFILE:
        error_msg = "too many open files";
        break;
    case ENOTTY:
        error_msg = "not a typewriter";
        break;
    case ETXTBSY:
        error_msg = "text file busy";
        break;
    case EFBIG:
        error_msg = "file too large";
        break;
    case ENOSPC:
        error_msg = "no space left on device";
        break;
    case ESPIPE:
        error_msg = "illegal seek";
        break;
    case EROFS:
        error_msg = "read-only file system";
        break;
    case EMLINK:
        error_msg = "too many links";
        break;
    case EPIPE:
        error_msg = "broken pipe";
        break;
    case EDOM:
        error_msg = "math argument out of domain of func";
        break;
    case ERANGE:
        error_msg = "math result not representable";
        break;
    default:
        error_msg = "unknown error";
        break;
    }
    return error_msg;
}

Services &Services::Instance()
{
    static Services instance;
    return instance;
}

std::stringstream Services::process(const std::string &command, std::stringstream &ss, int &code,cJSON *root)
{
    if (command_service_map.find(command) != command_service_map.end())
    {
        code = 0;
        Kernel::Instance().GetUser().u_error = 0;
        stringstream ret=(Instance().*(command_service_map[command]))(ss,root);

        return ret;
    }
    else
    {
        std::stringstream ret;
        ret << "[SERVICES] command "
                 << "\"" << command << "\""
                 << " not found" << std::endl;

        cout << "[SERVICES] command "
             << "\"" << command << "\""
             << "not found" << endl;
        code = -1;
        return ret;
    }
}