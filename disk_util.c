/*
 * disk_util.c
 *
 *  Created on: Jan 24, 2015
 *      Author: yigal
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

int get_dir_entry( struct ext2_dir_entry_2 *dir_pointer, char* entry_name, int inode_number, int inode_table,int inode_size,int block_size)
{
	DBG_ENTRY
	struct ext2_inode inode;
	int i, j, db_length;

	if(get_inode(&inode, inode_number,inode_table,inode_size,block_size)==-1)
	{
		printf("[ERROR] get_inode failed\n");
		return -1;

	}
	char dir_data[block_size];
	DBG_MSG("dir name is %s",dir_pointer->name);
	for ( i = 0; i < 12 && (inode.i_block[i]!=0); i++ )
	{
		db_length = 0;
		db_length = read_block(inode.i_block[i], dir_data,block_size); /*first data block*/
		if(db_length == -1)
		{
			printf("[ERROR] get_inode failed\n");
			DBG_EXIT
			return -1;

		}
		j = 0;
		while( j < db_length )
		{
			if( memcpy( dir_pointer, dir_data + j, sizeof( struct ext2_dir_entry_2))!= NULL)
			{
				DBG_MSG("dir name is %s at inode %d entry is %s",dir_pointer->name,dir_pointer->inode,entry_name);
				if(!strcmp(dir_pointer->name,entry_name) && dir_pointer->file_type == 2)
				{
					DBG_EXIT
					return 0;
				}
				j += dir_pointer->rec_len;
			} else return -1; /* To prevent a potential infinite loop in case of a copy error and */
		}
	}
	DBG_EXIT
	return -1; /*the entry wasn't found*/
}

int split_path(char* path, char*** result)
{
	DBG_ENTRY
	char** sub_directories;
	char* entry_name= strtok(path, "/");
	int entries_number = 0;
	sub_directories = (char**)malloc(sizeof(char*) * 1);
	while(entry_name)
	{
		sub_directories = realloc(sub_directories, sizeof(char*) * (++entries_number + 1));
		if (sub_directories == NULL)
		{
			printf("[ERROR] memory allocation (using realloc) failed\n");
			return -1;
		}
		sub_directories[entries_number -1] = entry_name;
		entry_name= strtok(NULL, "/");
	}
	/*realloc one more entry for the NULL*/
	sub_directories = realloc(sub_directories, sizeof(char*) * (entries_number+1));
	sub_directories[entries_number] = NULL;

	*result = sub_directories;
	DBG_EXIT
	return 0;
}

int valid_path(char path[], int inode_table,int inode_size,int block_size, struct ext2_dir_entry_2 * target_dir)
{
	DBG_ENTRY
	char** sub_directories;
	char tmp_path[BUF_SIZE];
	strcpy(tmp_path , path);
	int i, inode_number = 1;

	if(split_path(tmp_path, &sub_directories) == -1)
	{
		printf("[ERROR] split_path failed\n");
		return -1;
	}

	struct ext2_dir_entry_2 current_entry;
	for(i=0; sub_directories[i] != NULL; i++)
	{
		DBG_MSG("Getting inode_number:%d",inode_number);
		if (get_dir_entry(&current_entry, sub_directories[i], inode_number,inode_table,inode_size,block_size) == -1)
		{
			return -1;
		}
		inode_number = current_entry.inode - 1;
		DBG_MSG("Next inode_number:%d",inode_number);
	}

	/*the last one is the most inner one - copy if for further use*/
	if( memcpy( target_dir, &current_entry, sizeof( struct ext2_dir_entry_2 ) ) == NULL)
	{
		printf("[ERROR] memcpy failed\n");
		return -1;
	}
	DBG_EXIT
	return 0;
}

int open_disk()
{
	DBG_ENTRY
	return open(BASE_DISK_PATH, O_RDWR);
}

int get_disk_properties(int * block_size, struct ext2_super_block * sb, struct ext2_group_desc * gd)
{
	DBG_ENTRY
	char buffer[*block_size];

	/* ---- SuperBlock ---- */
	int fd = read_block(1, buffer,*block_size);
	if (fd == -1)
	{
		printf("[ERROR] read super block failed\n");
		return -1;
	}
	if (memcpy((void*)sb, buffer, sizeof(struct ext2_super_block)) == NULL)
	{
		printf("[ERROR] memcpy super block failed\n");
		return -1;
	}
	*block_size = (int)pow(2, sb->s_log_block_size) * 1024; /*  Update block size*/
	/* ---- GroupDescriptor ---- */
	fd = read_block(2, buffer,*block_size);
	if (fd == -1)
	{
		printf("[ERROR] read group descriptor block failed\n");
		return -1;
	}


	if(memcpy((void*)gd, buffer, sizeof(struct ext2_group_desc)) == NULL)
	{
		printf("[ERROR] memcpy group descriptor block failed\n");
		return -1;
	}
	DBG_EXIT
	return 0;
}

int read_block(int block_number, char *buffer, int block_size)
{
	DBG_ENTRY
	int fid;
	fid = open_disk();
	if (fid == -1)
	{
		printf("[ERROR] open failed\n");
		return -1;
	}

	int dest, len;
	dest = lseek(fid, block_number * block_size, SEEK_SET);
	if (dest != block_number * block_size)
	{
		printf("[ERROR] lseek failed\n");
		close(fid);
		return -1;
	}

	len = read(fid, buffer, block_size);
	if (len != block_size)
	{
		printf("[ERROR] read block failed\n");
		close(fid);
		return -1;
	}
	close(fid);
	buffer[1023]= '\0';
	DBG_EXIT
	return len;
}

int read_inode(int inode_number, int inode_table, int inode_size, char *buffer,int block_size)
{
	DBG_ENTRY
	int fid;
	fid = open_disk();
	if (fid == -1)
	{
		printf("[ERROR] open failed\n");
		return -1;
	}

	int dest, len;
	dest = lseek(fid, inode_table * block_size + inode_number * inode_size, SEEK_SET);
	if (dest != inode_table * block_size + inode_number * inode_size)
	{
		printf("[ERROR] lseek failed\n");
		close(fid);
		return -1;
	}

	len = read(fid, buffer, inode_size);
	if (len != inode_size)
	{
		printf("[ERROR] read block failed\n");
		close(fid);
		return -1;
	}
	close(fid);
	DBG_EXIT
	return len;
}

int get_inode(struct ext2_inode* inode_pointer, int inode_number,int inode_table, int inode_size,int block_size)
{
	DBG_ENTRY
	DBG_MSG("inode_number: %d",inode_number);
	char inode[inode_size];
	int res = read_inode(inode_number, inode_table,inode_size, inode,block_size);
	if (res == -1)
	{
		printf("[ERROR] read inode %d failed\n", inode_number);
		return -1;
	}
	if( memcpy((void*)inode_pointer, inode, sizeof(struct ext2_inode))== NULL)
	{
		printf("[ERROR] memcpy to inode struct failed\n");
		return -1;
	}
	DBG_EXIT
	return 0;
}


int print_dir_contents(struct ext2_dir_entry_2 *dir_pointer, int inode_table,int inode_size,int block_size)
{

	DBG_ENTRY
	struct ext2_inode inode;
	struct ext2_dir_entry_2 * de_pos;
	int i, j, db_length;

	DBG_MSG("dir name is %s at inode %d",dir_pointer->name,dir_pointer->inode);
	if(get_inode(&inode, (dir_pointer->inode),inode_table,inode_size,block_size)==-1)
	{
		printf("[ERROR] get_inode failed\n");
		return -1;
	}
	char dir_data[block_size];
	de_pos=dir_pointer;
	for ( i = 0; i < 12 && (inode.i_block[i]!=0); i++ ) /* Read the data blocks of the directory  */
	{

		DBG_MSG("i: %d iblock: %d",i,inode.i_block[i]);
		db_length = 0;
		db_length = read_block(inode.i_block[i], dir_data,block_size); /*read a block*/
		DBG_MSG("i: %d db_length: %d",i,db_length);
		if(db_length == -1)
		{
			printf("[ERROR] get_inode failed\n");
			return -1;

		}
		j = 0;
		while( j < db_length )
		{
			if( memcpy( de_pos, dir_data + j, sizeof(struct ext2_dir_entry_2))!= NULL)
			{
				if(de_pos->inode!=0)
				{
					DBG_MSG("found dir named %s:",de_pos->name);
					if (print_dir_entry(de_pos,inode_table,inode_size,block_size)==-1)
					{
						printf("[ERROR] printing file entry failed\n");
						return -1;
					}
				}
				j += de_pos->rec_len;
			}
		}
	}
	DBG_EXIT
	return -1; /*the entry wasn't found*/
}

int print_dir_entry(struct ext2_dir_entry_2 * entry,int inode_table, int inode_size, int block_size)
{
	struct ext2_inode inode;
	char date_string[100];
	struct tm *converted_time;
	if(get_inode(&inode, (entry->inode),inode_table,inode_size,block_size)==-1)
	{
		printf("[ERROR] get_inode failed\n");
		return -1;
	}
	converted_time=localtime((time_t *) &inode.i_mtime);
	strftime(date_string, sizeof(date_string), "%d-%b-%Y %H:%M:%S ",converted_time);
	printf(" %s %s\n",date_string,entry->name);

	// take the date from and convert it to a string
	return 0;
}


