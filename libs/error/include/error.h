//
// Created by jakub on 11/22/15.
//

#ifndef AC_ERROR_H
#define AC_ERROR_H

//#define _GNU_SOURCE
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define ERR(source) (fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
                     perror(source),kill(0,SIGKILL),\
		     		     exit(EXIT_FAILURE))
#define HERR(source) (fprintf(stderr,"%s(%d) at %s:%d\n",\
						source,h_errno,__FILE__,__LINE__),\
						perror(source),kill(0,SIGKILL),\
						exit(EXIT_FAILURE))


#endif //AC_ERROR_H
