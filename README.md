# FileSystem
This is a simple secondary file system implemented in C++, following the design of the UNIX-like file system.

This file system is capable of handling multiple clients concurrently. 

# Requirements
This project is developed and tested on Ubuntu 20.04.


# Build
To build this project, you need to satisfy the following requirements:
- make
- build-essential

To build the server, please change your current directory to the server directory and run the following commands:
```bash
$ cd server
$ make clean
$ make
```

To build the client, please change your current directory to the client directory and run the following commands:
```bash
$ cd client
$ make clean
$ make
```

# Run
To run this project, you should run the server first through the following command:
```bash
$ cd server
$ ./build/FileSystemServer
```

Then, you can run the client through the following command:
```bash
$ cd client
$ ./build/FileSystemClient
```