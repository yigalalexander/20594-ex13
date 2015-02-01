/*
 * my_cd.c - Custom implementation of cd on ext2fs
 *	Provided by the staff of Operating Systems course at the Open University of Israel
 *	Modified to re use the code here - most of the disk handling functions are moved to a separate header
 *
 *   Created on: Jan 26, 2015
 *      Student: Yigal Alexander
 *      	 ID: 306914565
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
	DBG_ENTRY
	if (argc != 2)
	{
		printf("Usage: %s <directory name>\n", argv[0]);
		exit(1);
	}

	char dir_name[BUF_SIZE];	
	strcpy(dir_name, argv[1]);

	struct ext2_super_block sb;
	struct ext2_group_desc gd;
	if (get_disk_properties(&block_size,&sb,&gd)==-1)
	{
		printf("[ERROR] failed to get the superblock and group descriptor\n");
		exit (1);
	}

	inode_size = sb.s_inode_size; /*Size of on-disk inode structure*/
	inode_table = gd.bg_inode_table; /*Block number of first inode table block*/

	if ( valid_path(dir_name,inode_table,inode_size,block_size,&inner_dir) == 0 )
	{
		path_fid = open(PWD_PATH, O_RDWR | O_TRUNC) ;
		if (path_fid == -1)
		{
			if(errno == EACCES)
			{
				printf("[ERROR] open 'myext2' failed, permission denied\n");
			}
			else if(errno == ENONET)
			{
				printf("[ERROR] open 'myext2' failed, the file does not exist\n");
			}
			else
			{
				printf("[ERROR] open 'myext2' failed\n");
			}
			exit(1);
		}
		int res = write(path_fid, dir_name, strlen(dir_name));
		if (res == -1)
		{
			printf("[ERROR] write directory to 'myext2' failed\n");
			exit(1);
		}
	}
	else
	{
		printf("[ERROR] directory doesn't exist\n");
		exit(1);
	}

	return 0;
}

