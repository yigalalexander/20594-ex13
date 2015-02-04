/*
 *
 * Debug utilities for C
 * Author: Yigal Alexander
 * Based on Macros from Rachel Cohen Yeshurun
 *
 */
#include <stdlib.h>
#include <stdio.h>

#define DBG_MSG //printf("\n[%s]\t[%d]: %s: ", __FILE__,__LINE__, __FUNCTION__);printf
#define DBG_ENTRY //printf("\n[%s]\t[%d]: --> %s", __FILE__,__LINE__, __FUNCTION__);
#define DBG_EXIT //printf("\n[%s]\t[%d]: <-- %s", __FILE__,__LINE__, __FUNCTION__);
