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
#include "cJSON.h"
#define PORT 8888
#define MAX_FILE_SIZE 960
#define SEND_INTERVAL 1

RemoteServer server;

using namespace std;


stringstream print_head()
{
    stringstream send_str;
    send_str <<"---------------------------------------------------------------"<<endl;
    send_str << ">>>help menu<<<" << endl;
    send_str << "open [filename] [mode] : to open a file with selected mode" << endl;
    send_str << "close [fd] : to close a file with selected fd" << endl;
    send_str << "read [fd] [length] : to read the file with selected fd" << endl;
    send_str << "write [fd] [text] : to write text into the file with selected fd" << endl;
    send_str << "lseek [fd] [position] [ptrname] : to seek the file with selected fd" << endl;
    send_str << "create [filename] : to create a file" << endl;
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
class thread_user_struct
{
private:
    int socket_id;
    string current_user_str;

public:
    int send_plaint_string(const stringstream &send_str)
    {
        // cout<<send_str.str()<<endl;
        int numbytes = send(socket_id, send_str.str().c_str(), send_str.str().length(), 0);
        cout << "[[ " << current_user_str << " ]] send " << numbytes << " bytes" << endl;
        cout << "====message send====" << endl;
        cout << send_str.str() << endl;
        cout << "====================" << endl;
        return numbytes;
    };
    thread_user_struct(int socket_id, string current_user_str)
    {
        this->socket_id = socket_id;
        this->current_user_str = current_user_str;
    };

    int send_command_to_json(const stringstream& send_str)
    {
        cJSON *command = cJSON_CreateObject();
        cJSON_AddStringToObject(command, "command", send_str.str().c_str());
        char *command_char = cJSON_Print(command);
        string command_strs = command_char;
        int numbytes = send(socket_id, command_strs.c_str(), command_strs.length(), 0);
        cout << "[[ " << current_user_str << " ]] send " << numbytes << " bytes" << endl;
        cout << "====message send====" << endl;
        cout << command_strs << endl;
        cout << "====================" << endl;
        return numbytes;
    }
};

void *start_routine(void *ptr)
{
    int fd = *(int *)ptr;
    char buf[MAX_PACKAGE_LENGTH];
    int numbytes;
    cJSON *welcome = cJSON_CreateObject();
    cJSON_AddStringToObject(welcome, "command", "please type in the username:");
    char *welcome_char = cJSON_Print(welcome);
    string welcome_strs = welcome_char;

    numbytes = server.send_message(welcome_strs, fd);
    if (numbytes == -1)
    {
        cout << "send() error" << endl;
        return (void *)NULL;
    }

    cout<<"start thread to serve at fd=%d"<<fd<<endl;

    memset(buf, 0, sizeof(buf));
    if ((numbytes = recv(fd, buf, MAX_PACKAGE_LENGTH, 0)) == -1)
    {
        cout << ("recv() error\n");
        return (void *)NULL;
    }
    cJSON *root = cJSON_Parse(buf);
    string username = cJSON_GetObjectItem(root, "command")->valuestring;
    
    
    cout << "->" << username<<" logged in" << endl;

    thread_user_struct sd(fd, username);

    // login the user using username and pthread_self
    Kernel::Instance().GetUserManager().Login(username);
    // init the prompt
    stringstream welcome_str = print_head();
    string tipswords = username + "@FileSystem:" + Kernel::Instance().GetUserManager().GetUser()->u_curdir + "$";

    // send the welcome message
    welcome_str << tipswords;
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
        char from_socket[MAX_PACKAGE_LENGTH] = {0};

        // read the command from client
        if ((numbytes = recv(fd, from_socket, MAX_PACKAGE_LENGTH, 0)) == -1)
        {
            cout << "recv() error" << endl;
            Kernel::Instance().GetUserManager().Logout();
            return (void *)NULL;
        }
        cout<<"recv from client of "<<numbytes<<" bytes"<<endl;
        cout<<"from_socket : "<<from_socket<<endl;
        cJSON *root = cJSON_Parse(from_socket);
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
        stringstream send_str;

        cout << "from_socket : " << from_socket << endl;
        string current_command;
        ss >> current_command;

        cout << "parse command: " << current_command << endl;
        
        if (current_command == "q" || current_command == "quit")
        {
            Kernel::Instance().GetUserManager().Logout();
            send_str << "user logout\n";
            sd.send_plaint_string(send_str);
            break;
        }

        int code = 0;
        send_str = Services::process(current_command, ss, code, root);
        //check whether the root has "content"
        
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
            if(cJSON_GetObjectItem(root,"content")!=NULL){
                cout<<"the root has content"<<endl;
            }
        }

        // show the prompt with username and current directory
        string tipswords = username + "@FileSystem:" + Kernel::Instance().GetUserManager().GetUser()->u_curdir + "$";

        send_str << tipswords;

        // cJSON_Delete(root);
        cJSON *response = cJSON_CreateObject();
        cJSON_AddStringToObject(response, "command", send_str.str().c_str());
        char *response_char = cJSON_Print(response);
        cout << "response_char : " << response_char << endl;
        stringstream response_str;
        response_str << response_char;
        cout <<response_str.str()<< endl;

        if(current_command=="download")
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
                numbytes = sd.send_plaint_string(newresponse_str);
                len-=MAX_FILE_SIZE;
                count+=MAX_FILE_SIZE;
                sleep(SEND_INTERVAL);
            }
        }
        numbytes = sd.send_plaint_string(response_str);
        printf("[NETWORK] send %d bytes\n", numbytes);
        cJSON_Delete(root);
    }
    close(fd);
    return (void *)NULL;
}

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
