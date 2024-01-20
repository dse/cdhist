BIN = bin/cdhist
SRCS = src/append.c src/filename.c src/getline.c src/main.c
HDRS = $(patsubst %.c,%.h,$(SRCS))
OBJS = $(patsubst %.c,%.o,$(SRCS))

$(BIN): $(OBJS) Makefile
	cc $(LDFLAGS) -o "$@" $(OBJS)

src/%.o: src/%.c $(HDRS) Makefile
	cc -c $(CFLAGS) $(CPPFLAGS) -o "$@" "$<"

clean: FORCE
	/bin/rm $(OBJS) $(BIN) || true

.PHONY: FORCE
