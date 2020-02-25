TARGETS = test hashlog

CFLAGS = -Wall -Werror -g

all: $(TARGETS)

# xxx not using cflags
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

OBJS = gc.o tok.o htab.o process.o
TEST_OBJS = test.o
HASHLOG_OBJS = hashlog.o

test: $(TEST_OBJS) $(OBJS)
	$(CC) -o test $^

hashlog: $(HASHLOG_OBJS) $(OBJS)
	$(CC) -o hashlog $^

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TEST_OBJS) $(HASHLOG_OBJS) $(TARGETS)
