/*
 * my_rm.c - Custom implementation of ls on ext2fs
 *
 *   Created on: Jan 23, 2015
 *      Student: Yigal Alexander
 *      	 ID: 306914565
 */

#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <string.h>
#include <ext2_fs.h>
#include <fs.h>

#include "debugutil.h"


void print_usage() {
	printf("Usage: my_dir\n");
}


int main(int argc, char * argv[]) {

	/*
	 * 1. Check args
	 * 2. parse the directory name
	 * 2. open the  /dev/fd0
	 * 3. list
	 */

	if (argc==2) { /* check args*/

	}
	print_usage();
	exit (1);


}
