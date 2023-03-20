#ifndef _GLOBA_H_
#define _GLOBA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>

#include "data_shm.h"
#include "data_types.h"

#include "sdk_ipc.h"
#include "sdk_file.h"
#include "sdk_misc.h"


#define	SDK_PRINT_H  1

#if	SDK_PRINT_H
	#include "sdk_print_debug.h"
	#include "sdk_print_error.h"
#else
    #define DEBUG(fmt, ...)
	#include "sdk_print_error.h"
#endif

#endif /* _GLOBA_H_ */