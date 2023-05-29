#pragma once
#include "FileSystem.h"
#include "BufferManager.h"
#define DISKFILE_PATH "diskfile.img"

struct block_table{
    int free_top; 
    int free_block_table[SuperBlock::GROUP_BLOCK_NUMBER];
};

class DiskDriver
{
public:
DiskDriver();
~DiskDriver();

void Initialize();
void uninstall();

bool exist;

private:
    void write_super_block(SuperBlock &sb);
    void write_disk_block(char* data);
    void disk_formatting(int fd);
    void disk_mapping(int fd);
    int disk_mapped_fd;
    BufferManager *m_BufferManager; 


};
