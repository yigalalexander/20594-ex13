/*
 * disk_util.c
 *
 *  Created on: Jan 24, 2015
 *      Author: yigal
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fs.h>
#include <ext2_fs.h>

#include "disk_util.h"


int fid; /* global variable set by the open() function */
int block_size; /* TODO bytes per sector from disk geometry */

struct ext2_super_block sb;
struct ext2_group_desc group1;


/* Reads into the buffer a given block*/
int fd_read(int block_number, char *buffer){
	int dest, len;
	dest = lseek(fid, block_number * block_size, SEEK_SET);
	if (dest != block_number * block_size){
		/* Error handling */
	}
	len = read(fid, buffer, block_size);
	if (len != block_size){
		/* error handling here */
	}
	return len;
}


int open_disk(char * path) {

}

/* Getting the superblock from the disk*/
int get_superblock_from_disk () {
	char * buffer;

	fd_read(0,buffer);
	struct ext2_super_block sb;
	memcpy(sb, buffer, sizeof(struct ext2_super_block));

	return 0;
}

void print_dir(struct ext2_dir_entry_2) {

}


