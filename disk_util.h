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
int block_size=1024;  /* bytes per sector from disk geometry */

int open_disk();

int get_dir_entry( struct ext2_dir_entry_2 *dir_pointer, char* entry_name, int inode_number);

int split_path(char* path, char*** result);

int valid_path(char path[]);

int read_block(int block_number, char *buffer);

int read_inode(int inode_number, char *buffer);

int get_inode(struct ext2_inode* inode_pointer, int inode_number);


/* print contents of a directory*/
void print_dir(struct ext2_dir_entry_2);

#endif
