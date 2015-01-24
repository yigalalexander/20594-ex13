/*
 * disk_util.h
 *
 *  Created on: Jan 24, 2015
 *      Author: yigal
 */

#ifndef DISK_UTIL_H_
#define DISK_UTIL_H_
#include <fs.h>
#include <ext2_fs.h>
#define EXT2_N_BLOCKS 15


/* Reads into the buffer a given block*/
int fd_read(int block_number, char *buffer);

/* */
int get_superblock_from_disk ();

/* */
int open_disk(char * path);

/* print contents of a directory*/
void print_dir(struct ext2_dir_entry_2);

#endif
