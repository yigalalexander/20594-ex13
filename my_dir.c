/*
 * my_dir.c - Custom implementation of ls on ext2fs
 *
 *   Created on: Jan 23, 2015
 *      Student: Yigal Alexander
 *      	 ID: 306914565
 *    valid_path searches each sub directory after breaking the path
 *    it uses get_dir_entry to search for it
 *    if the result of valid_path is true, that is the file type is 2,
 *    then we hold the pointer for that dir entry and we list the contents
 *    for each file we need to get the date, time and name - from its inode
 *    we need to call get_dir_entry for the directory we found and list the data for each folder
 *    TODO modify get_dir_entry -> print_dir_contents
 *    TODO function get_inode_metadata - for getting time stamp from the file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/ext2_fs.h>
#include <sys/stat.h>

#include "disk_util.h"
#include "debugutil.h"


int disk_fid,path_fid; /* global variable set by the open() function */

int inode_table;
int inode_size;

int main(int argc, char *argv[])
{
	DBG_ENTRY

	int block_size=BASE_BLOCK_SIZE;  /* bytes per sector from disk geometry */
	struct ext2_dir_entry_2 inner_dir;
	char dir_name[BUF_SIZE];

	if (argc != 1)
	{
		printf("Usage: %s \n", argv[0]);
		exit(1);
	}


	path_fid = open(PWD_PATH, O_RDONLY) ;
	if (path_fid == -1)
	{
		if(errno == ENONET)
		{
			printf("[ERROR] open 'myext2' failed, using the root directory, creating the file\n");
			path_fid=open(PWD_PATH,O_CREAT);
			if (path_fid==-1)
			{
				printf("[ERROR] failed to create the %s file\n",PWD_PATH); /* Attempt to create the file again*/
				exit(1);
			}
		}
		else
		{
			printf("[ERROR] open 'myext2' failed ");
			if(errno == EACCES) printf("permission denied");
			printf("\n");
			exit(1);
		}

	}

	int res = read(path_fid, dir_name, BUF_SIZE);
	if (res == -1)
	{
		printf("[ERROR] detecting present working directory failed\n");
		exit(1);
	}


	struct ext2_super_block sb;
	struct ext2_group_desc gd;
	if (get_disk_properties(&block_size,&sb,&gd)==-1)
	{
		printf("[ERROR] failed to get the superblock and group descriptor\n");
		exit (1);
	}

	inode_size = sb.s_inode_size; /*Size of on-disk inode structure*/
	inode_table = gd.bg_inode_table; /*Block number of first inode table block*/
	DBG_MSG("inode_size %d inode_table %d\n",inode_size,inode_table);
	if ( valid_path(dir_name,inode_table,inode_size,block_size,&inner_dir) == 0 ) // found the folder
	{
		//DBG_MSG("What a surprise, i found the folder");
		print_dir_contents(&inner_dir,inode_table,inode_size,block_size);
	}
	else
	{
		printf("[ERROR] directory doesn't exist, perhaps %s is corrupted\n",PWD_PATH);
		exit(1);
	}
	DBG_EXIT
	return 0;
}

