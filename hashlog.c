/*
 * hashlog
 *
 * Input generally should be a log file. Each line may begin with a time
 * stamp. Month and day-of-week abbreviations are unique-ified out.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"
#include "htab.h"

int
main(int argc, char **argv)
{
    int i;

    if (argc < 2) {
	fprintf(stderr, "Usage: hashlog <logfile> [<logfile> ...]\n");
	exit(1);
    }

    for (i = 1; i < argc; i++)
	process_file(argv[i]);

    htab_dump();

    return 0;
}
