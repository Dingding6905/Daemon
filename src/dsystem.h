#ifndef __DSYSTEM_H__
#define __DSYSTEM_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#define CORE_SIZE 1024 * 1024 * 500

int Dsysteam_CoreDump();

#endif /* __DSYSTEM_H__ */

