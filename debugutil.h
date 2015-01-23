/*
 *
 * Debug utilities for C
 * Author: Yigal Alexander
 * Based on Macros from Rachel Cohen Yeshurun
 *
 */
#include <stdlib.h>
#include <stdio.h>


#define DBG_MSG printf("\n[%d]: %s: ", __LINE__, __FUNCTION__);printf
#define DBG_ENTRY printf("\n[%d]: --> %s", __LINE__,__FUNCTION__);
#define DBG_EXIT printf("\n[%d]: <-- %s", __LINE__,__FUNCTION__);
