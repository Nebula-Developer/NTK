SRCS = $(wildcard src/*.c)

PROGS = $(patsubst src/%.c,bin/%,$(SRCS))

all: $(PROGS)

bin/%: src/%.c
	$(CC) $(CFLAGS)  -o $@ $<

.PHONY: clean cp

clean:
	rm ./bin/*

cp:
	mkdir -p /usr/local/bin/ntk
	cp ./bin/* /usr/local/bin/ntk/