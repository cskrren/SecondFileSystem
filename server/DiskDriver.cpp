#include "DiskDriver.h"
#include "Kernel.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <iostream>
using namespace std;

DiskDriver::DiskDriver()
{
}
DiskDriver::~DiskDriver()
{
}
void DiskDriver::Initialize()
{
    exist = true;

    this->m_BufferManager = &Kernel::Instance().GetBufferManager();

    int fd = open(DISKFILE_PATH, O_RDWR);
    if (fd == -1)
    {
        exist = false;
        fd = open(DISKFILE_PATH, O_RDWR | O_CREAT, 0666);
        if (fd == -1)
        {
            printf("[error] DiskDriver Init Error: failed to create %s\n", DISKFILE_PATH);
            exit(-1);
        }
        this->disk_formatting(fd);
    }
    disk_mapping(fd);
    this->disk_mapped_fd = fd;

    cout << "diskdriver: DiskDriver Initialize Done." << endl;
}

void DiskDriver::uninstall()
{
    struct stat st; 
    int r = fstat(this->disk_mapped_fd, &st);
    if (r == -1)
    {
        printf("[error]fail to get img file info, file system exit\n");
        close(this->disk_mapped_fd);
        exit(-1);
    }
    int len = st.st_size;
    msync((void *)(this->m_BufferManager->GetP()), len, MS_SYNC);
    
}

void DiskDriver::write_super_block(SuperBlock& sb)
{
    sb.s_isize = FileSystem::INODE_ZONE_SIZE;
    sb.s_fsize = FileSystem::DATA_ZONE_END_SECTOR + 1;
    sb.s_nfree = (FileSystem::DATA_ZONE_SIZE - SuperBlock::MAX_INODE_NUMBER + 1) % SuperBlock::GROUP_BLOCK_NUMBER;

    int start_last_datablk = FileSystem::DATA_ZONE_START_SECTOR;
    while(true){
        if((start_last_datablk + SuperBlock::GROUP_BLOCK_NUMBER -1) < FileSystem::DATA_ZONE_END_SECTOR)
            start_last_datablk += SuperBlock::GROUP_BLOCK_NUMBER;
        else
            break;
    }
    start_last_datablk--;

    for(int i = 0; i < sb.s_nfree; ++i)
        sb.s_free[i] = start_last_datablk + i;

    sb.s_ninode = SuperBlock::MAX_INODE_NUMBER;
    for(int i = 0; i < sb.s_ninode; ++i)
        sb.s_inode[i] = i;
    
    sb.s_fmod  = 0;
    sb.s_ronly = 0;
}

/**
 * @brief init a data block
 * @param data 
 */
void DiskDriver::write_disk_block(char* data)
{
    block_table tmp_table;

    int last_datablk_num = FileSystem::DATA_ZONE_SIZE;
    for(int i = 0; ; i++)
    {
        if (last_datablk_num >= SuperBlock::GROUP_BLOCK_NUMBER)
            tmp_table.free_top = SuperBlock::GROUP_BLOCK_NUMBER;
        else
            tmp_table.free_top = last_datablk_num;
        last_datablk_num -= tmp_table.free_top;

        for (int j = 0; j < tmp_table.free_top; j++)
        {
            if (i == 0 && j == 0)
                tmp_table.free_block_table[j] = 0;
            else
            {
                tmp_table.free_block_table[j] = 100 * i + j + FileSystem::DATA_ZONE_START_SECTOR - 1;
            }
        }
        memcpy(&data[99 * BufferManager::BUFFER_SIZE + i *  SuperBlock::GROUP_BLOCK_NUMBER * 512], (void *)&tmp_table, sizeof(tmp_table));
        if (last_datablk_num == 0)
            break;
    }
}

void DiskDriver::disk_formatting(int fd)
{
    SuperBlock spb;
    write_super_block(spb);
    DiskInode *di_table = new DiskInode[FileSystem::INODE_ZONE_SIZE*FileSystem::INODE_NUMBER_PER_SECTOR];

    di_table[0].d_mode = Inode::IFDIR;  
    di_table[0].d_mode |= Inode::IEXEC;

    char* datablock = new char[FileSystem::DATA_ZONE_SIZE * 512];
    memset(datablock, 0, FileSystem::DATA_ZONE_SIZE * 512);
    write_disk_block(datablock);

    write(fd, &spb, sizeof(SuperBlock));
    write(fd, di_table, FileSystem::INODE_ZONE_SIZE * FileSystem::INODE_NUMBER_PER_SECTOR * sizeof(DiskInode));
    write(fd, datablock, FileSystem::DATA_ZONE_SIZE * 512);

    printf("diskdriver: Format Disk Done...\n");
}

void DiskDriver::disk_mapping(int fd)
{
    struct stat st; //upload file info
    int r = fstat(fd, &st);
    if (r == -1)
    {
        printf("[error] fail to get img file info, file system exit\n");
        close(fd);
        exit(-1);
    }
    // get the size of the file
    int len = st.st_size;
    // use mmap to map the file
    char *addr = (char*)mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    this->m_BufferManager->SetP(addr);
}

