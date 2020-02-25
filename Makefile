TARGETS = test hashlog

OPT = -O2
#DEBUG = -g
#PROF = -pg

CFLAGS = -Wall -Werror $(OPT) $(DEBUG) $(PROF)
LDFLAGS = $(PROF)

all: $(TARGETS)

# xxx not using cflags
%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

SRCS_LIB = gc.c tok.c htab.c process.c
SRCS_TEST = test.c
SRCS_HASHLOG = hashlog.c
SRCS = $(SRCS_LIB) $(SRCS_TEST) $(SRCS_HASHLOG)

OBJS_LIB = $(SRCS_LIB:.c=.o)
OBJS_TEST = $(SRCS_TEST:.c=.o)
OBJS_HASHLOG = $(SRCS_HASHLOG:.c=.o)
OBJS = $(SRCS:.c=.o)

test: $(OBJS_TEST) $(OBJS_LIB)
	$(CC) -o test $(LDFLAGS) $^

hashlog: $(OBJS_HASHLOG) $(OBJS_LIB)
	$(CC) -o hashlog $(LDFLAGS) $^

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGETS) gmon.out

.PHONY: depend
depend:
	makedepend -Y *.c && $(RM) Makefile.bak

# DO NOT DELETE THIS LINE -- make depend depends on it.

gc.o: gc.h
hashlog.o: process.h htab.h
htab.o: htab.h
process.o: tok.h gc.h htab.h
test.o: gc.h tok.h htab.h process.h
tok.o: gc.h tok.h
