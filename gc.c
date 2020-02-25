#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "gc.h"

int gc_bufsize = GC_BUFSIZE;

int rfd = -1;
char rbuf[GC_BUFSIZE];
int rcnt;
int rpos;
int gc_line;

void
gc_open(char *fname)
{
    if (strcmp(fname, "-") == 0)
	rfd = 0;
    else if ((rfd = open(fname, O_RDONLY)) < 0) {
	fprintf(stderr,
		"Could not open %s: %s\n",
		fname, strerror(errno));
	exit(1);
    }

    rcnt = 0;
    rpos = 0;
    gc_line = 1;
}

void
gc_close(void)
{
    if (rfd > 0)
	close(rfd);
}

/*
 * gc returns character, or -1 after EOF
 */
int
gc_get(void)
{
    int c;

    if (rpos == rcnt) {
	size_t n;

	if ((n = read(rfd, rbuf, BUFSIZ)) < 0) {
	    fprintf(stderr,
		    "Read error: %s\n",
		    strerror(errno));
	    exit(1);
	}

	if (n == 0)
	    return -1;

	rpos = 0;
	rcnt = (int)n;
    }

    c = rbuf[rpos++];

    if (c == '\n')
	gc_line++;

    return c;
}

/*
 * Can only unget one character. Can't unget EOF.
 */

void
gc_unget(int c)
{
    if (rpos > 0)
	rbuf[--rpos] = c;
    else {
	rpos = 1;
	rcnt = 1;
	rbuf[0] = c;
    }

    if (c == '\n')
	gc_line--;
}
