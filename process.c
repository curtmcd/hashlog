#include <assert.h>
#include <string.h>

#include "tok.h"
#include "gc.h"
#include "htab.h"

/*
 * Maximum length of signature string (truncated thereafter)
 */
#define SIG_SIZE	256

int sig_size = SIG_SIZE;

/*
 * Determine which words should be elided as varying data
 */

static int
elide(char *s)
{
    static char *dstr = "JanFebMarAprMayJunJulAugSepOctNovDecSunMonTueWedThuFriSat";

    /*
     * Strings beginning with a digit, assumed to be a number, including
     * hex strings beginning with '0x' or '0X'
     */
    if (ISDIGIT(s[0]))
	return 1;

    /*
     * Two-character strings of valid hex digits, to help ignore MAC addresses
     */
    if (ISHEX(s[0]) && ISHEX(s[1]) && s[2] == '\0')
	return 1;

    /*
     * Three-letter month and day-of-week abbreviations
     */
    if (strlen(s) == 3) {
	int i;
	for (i = 0; i < 57; i += 3)
	    if (s[0] == dstr[i + 0] &&
		s[1] == dstr[i + 1] &&
		s[2] == dstr[i + 2])
		return 1;
    }

    return 0;
}

void
process_file(char *fname)
{
    int t;
    char tok[TOK_SIZE], *tokptr;
    char sig[SIG_SIZE], *sigend, *sigptr;
    int eof;

    tok_open(fname);

    for (eof = 0; !eof; ) {
	/*
	 * Take words and punctuation until EOL/EOF,
	 * generating unique-ified signature.
	 */
	sigend = &sig[sig_size - 1];
	sigptr = sig;

	for (;;) {
	    char ch;

	    t = tok_get(tok);

	    if (t == TOK_EOF) {
		eof = 1;
		break;
	    }

	    if (t == TOK_EOL)
		break;

	    /* xxx - don't need SPACE or PUNCT to be separate from WORD? */
	    if (t == TOK_WORD) {
		if (elide(tok)) {
		    tok[0] = '#';
		    tok[1] = '\0';
		}
	    } else
		assert(t == TOK_SPACE || t == TOK_PUNCT);

	    for (tokptr = tok; (ch = *tokptr) != '\0'; tokptr++)
		if (sigptr < sigend)
		    *sigptr++ = ch;
	}

	if (sigptr > sig) {
	    *sigptr = '\0';
	    htab_insert(sig, sigptr - sig);
	}
    }

    tok_close();
}
