extern int gc_bufsize;
extern int gc_line;

#define GC_BUFSIZE	0x100000

void gc_open(char *fname);
void gc_close(void);
int gc_get(void);
void gc_unget(int c);
