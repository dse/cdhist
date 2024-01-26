APPEND_BIN = bin/cdhist-append
SRCS = src/cdhist-append.c src/filename.c src/getline.c src/main.c
HDRS = $(patsubst %.c,%.h,$(SRCS))
OBJS = $(patsubst %.c,%.o,$(SRCS))

$(APPEND_BIN): $(OBJS) Makefile
	cc $(LDFLAGS) -o "$@" $(OBJS)

src/%.o: src/%.c $(HDRS) Makefile
	cc -c $(CFLAGS) $(CPPFLAGS) -o "$@" "$<"

clean: FORCE
	/bin/rm $(OBJS) $(APPEND_BIN) || true

.PHONY: FORCE
