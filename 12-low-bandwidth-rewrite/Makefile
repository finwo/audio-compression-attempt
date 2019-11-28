SRC=$(wildcard *.c)
BIN=$(patsubst %.c,%,$(SRC))

CC=gcc
CFLAGS?=
CFLAGS+= -O3 -s
CFLAGS+= -lm
CFLAGS+= -fno-stack-protector
CFLAGS+= -fomit-frame-pointer
CFLAGS+= -fno-math-errno
CFLAGS+= -fmerge-all-constants
CFLAGS+= -fno-ident
CFLAGS+= -ffast-math
CFLAGS+= -W

all: $(BIN)

$(BIN): $(SRC) smooth.h
	$(CC) $(CFLAGS) -o $@ $@.c

.PHONY: clean
clean:
	rm -rf $(BIN)

.PHONY: sound
sound: $(BIN)
	# 96000 / 120 = 800hz modulation
	./resample -o 96000 < ../data/hoff\ -\ innocence.44k1.s16be.pcm | ./encode -r 120 | pv | ./decode -r 120 | pacat --raw --format=s16be --rate=96000 --channels=1
	# ./resample -o 96000 < ../data/hoff\ -\ innocence.44k1.s16be.pcm | ./encode -r 120 | hexdump -C | less
