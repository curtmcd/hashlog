#define TOK_SIZE	256

#define ISSPACE(c)	((c) == ' ' || (c) == '\t')
#define ISDIGIT(c)	((c) >= '0' && (c) <= '9')
#define ISUPPER(c)	((c) >= 'A' && (c) <= 'Z')
#define ISLOWER(c)	((c) >= 'a' && (c) <= 'z')
#define ISXUPPER(c)	((c) >= 'A' && (c) <= 'F')
#define ISXLOWER(c)	((c) >= 'a' && (c) <= 'f')
#define ISWORD(c)	(ISDIGIT(c) || ISUPPER(c) || ISLOWER(c))
#define ISHEX(c)	(ISDIGIT(c) || ISXUPPER(c) || ISXLOWER(c))

#define TOK_NONE	-1
#define TOK_EOF		0
#define TOK_EOL		1
#define TOK_SPACE	2
#define TOK_PUNCT	3
#define TOK_WORD	4

void tok_open(char *fname);
void tok_close(void);

int tok_get(char *tok);
void tok_unget(int t, char *tok);
