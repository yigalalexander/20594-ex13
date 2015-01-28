/*
 * disk_util.h
 *
 *  Created on: Jan 24, 2015
 *      Author: yigal
 */

#ifndef DISK_UTIL_H_
#define DISK_UTIL_H_
#include <linux/fs.h>
#include <linux/ext2_fs.h>

#define BUF_SIZE 2048
#define BASE_DISK_PATH "/dev/fd0"
#define BASE_BLOCK_SIZE 1024

int open_disk();

int get_dir_entry( struct ext2_dir_entry_2 *dir_pointer, char* entry_name, int inode_number, int inode_table,int inode_size,int block_size);

int split_path(char* path, char*** result);

int valid_path(char path[], int inode_table,int inode_size,int block_size);

int read_block(int block_number, char *buffer, int block_size);

int read_inode(int inode_number, int inode_table, int inode_size, char *buffer,int block_size);

int get_inode(struct ext2_inode* inode_pointer, int inode_number,int inode_table, int inode_size,int block_size);


/* print contents of a directory*/
void print_dir(struct ext2_dir_entry_2);

#endif
