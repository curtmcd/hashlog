#include <string.h>

#include "gc.h"
#include "tok.h"

void
tok_open(char *fname)
{
    gc_open(fname);
}

void
tok_close(void)
{
    gc_close();
}

/*
 * Tokens larger than TOK_SIZE are truncated
 */

int
tok_get(char *tok)
{
    int c;
    int pos;

    c = gc_get();

    if (c < 0)
	return TOK_EOF;

    if (c == '\n') {
	tok[0] = '\n';
	tok[1] = 0;
	return TOK_EOL;
    }

    if (ISSPACE(c)) {
	/* collapse space */
	for (;;) {
	    c = gc_get();
	    if (c < 0)
		break;
	    if (!ISSPACE(c)) {
		gc_unget(c);
		break;
	    }
	}
	tok[0] = ' ';
	tok[1] = '\0';
	return TOK_SPACE;
    }

    tok[0] = c;
    pos = 1;

    /* Not EOF, EOL or SPACE */

    if (!ISWORD(c)) {
	/* Left with single-character punctuation */
	tok[1] = '\0';
	return TOK_WORD;
    }

    /* Word */

    for (;;) {
	c = gc_get();
	if (c < 0)
	    break;
	if (!ISWORD(c)) {
	    gc_unget(c);
	    break;
	}
	/* store but truncate */
	if (pos < TOK_SIZE - 1)
	    tok[pos++] = c;
    }

    tok[pos] = '\0';
    return TOK_WORD;
}
