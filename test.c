#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gc.h"
#include "tok.h"
#include "htab.h"
#include "process.h"

#define FAIL()		fail(__LINE__)

void
fail(int lineno)
{
    fprintf(stderr, "Failed: line %d\n", lineno);
    exit(1);
}

void
test_gc()
{
    int i;

    gc_bufsize = 4;

    gc_open("test1.log");

    if (gc_get() != 'F')
	fail(__LINE__);

    if (gc_get() != 'e')
	fail(__LINE__);

    gc_unget('e');
    if (gc_get() != 'e')
	fail(__LINE__);

    if (gc_get() != 'b')
	fail(__LINE__);

    gc_unget('b');
    if (gc_get() != 'b')
	fail(__LINE__);

    if (gc_get() != ' ')
	fail(__LINE__);

    gc_unget(' ');
    if (gc_get() != ' ')
	fail(__LINE__);

    if (gc_get() != '1')
	fail(__LINE__);

    if (gc_get() != '7')
	fail(__LINE__);

    if (gc_get() != ' ')
	fail(__LINE__);

    if (gc_get() != '0')
	fail(__LINE__);

    if (gc_get() != '0')
	fail(__LINE__);

    for (i = 0; i < 84; i++)
	(void)gc_get();

    if (gc_get() != '2')
	fail(__LINE__);

    if (gc_get() != '\n')
	fail(__LINE__);

    if (gc_get() != -1)
	fail(__LINE__);

    if (gc_get() != -1)
	fail(__LINE__);

    gc_close();

    gc_bufsize = GC_BUFSIZE;
}

void
test_tok(void)
{
    int i;
    char tok[TOK_SIZE];

    tok_open("test1.log");

    if (tok_get(tok) != TOK_WORD)
	FAIL();
    if (strcmp(tok, "Feb") != 0)
	FAIL();

    if (tok_get(tok) != TOK_SPACE)
	FAIL();
    if (tok[0] != ' ')
	FAIL();

    if (tok_get(tok) != TOK_WORD)
	FAIL();
    if (strcmp(tok, "17") != 0)
	FAIL();

    if (tok_get(tok) != TOK_SPACE)
	FAIL();

    if (tok_get(tok) != TOK_WORD)
	FAIL();
    if (strcmp(tok, "00") != 0)
	FAIL();

    if (tok_get(tok) != TOK_WORD)
	FAIL();
    if (strcmp(tok, ":") != 0)
	FAIL();

    if (tok_get(tok) != TOK_WORD)
	FAIL();
    if (strcmp(tok, "13") != 0)
	FAIL();

    if (tok_get(tok) != TOK_WORD)
	FAIL();
    if (strcmp(tok, ":") != 0)
	FAIL();

    if (tok_get(tok) != TOK_WORD)
	FAIL();
    if (strcmp(tok, "44") != 0)
	FAIL();

    if (tok_get(tok) != TOK_SPACE)
	FAIL();

    if (tok_get(tok) != TOK_WORD)
	FAIL();
    if (strcmp(tok, "host") != 0)
	FAIL();

    for (i = 0; i < 12; i++)
	tok_get(tok);

    if (tok_get(tok) != TOK_SPACE)	/* double space after MSG: */
	FAIL();

    for (i = 0; i < 24; i++)
	tok_get(tok);

    if (tok_get(tok) != TOK_WORD)
	FAIL();
    if (strcmp(tok, "1432") != 0)
	FAIL();

    if (tok_get(tok) != TOK_EOL)
	FAIL();
    if (tok[0] != '\n')
	FAIL();

    if (tok_get(tok) != TOK_EOF)
	FAIL();

    if (tok_get(tok) != TOK_EOF)
	FAIL();

    tok_close();
}

void
test_htab(void)
{
    htab_insert("hello", 5);
    htab_insert("abc", 3);
    //htab_dump();
    htab_clear();
}

void
test_process(void)
{
    sig_size = 60;

    process_file("test1.log");
    process_file("test2.log");

    //htab_dump();
    htab_clear();
}

int
main(int argc, char **argv)
{
    test_gc();
    test_tok();
    test_htab();
    test_process();

    return 0;
}
