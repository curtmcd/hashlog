#include <string.h>

#include "gc.h"
#include "tok.h"

int tok_pushback_code;
char tok_pushback_buf[TOK_SIZE];

void
tok_open(char *fname)
{
    gc_open(fname);

    tok_pushback_code = TOK_NONE;
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

    if (tok_pushback_code != TOK_NONE) {
	int t = tok_pushback_code;
	tok_pushback_code = TOK_NONE;
	if (t != TOK_EOF && t != TOK_EOL && t != TOK_SPACE)
	    strcpy(tok, tok_pushback_buf);
	return t;
    }

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
	tok[1] = '\0';
	return TOK_PUNCT;
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

/*
 * Can only unget one token.
 */

void
tok_unget(int t, char *tok)
{
    tok_pushback_code = t;
    if (t != TOK_EOF && t != TOK_EOL && t != TOK_SPACE) {
	strncpy(tok_pushback_buf, tok, sizeof (tok_pushback_buf) - 1);
	tok_pushback_buf[sizeof (tok_pushback_buf) - 1] = '\0';
    }
}
