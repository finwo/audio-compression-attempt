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
CFLAGS+= -W -Wall

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $@.c

.PHONY: clean
clean:
	rm -rf $(BIN)

.PHONY: sound
sound: $(BIN)
	./resample -o 96000 < ../data/hoff\ -\ innocence.44k1.s16be.pcm | ./encode -r 600 | ./decode -r 600 | pacat --raw --format=s16be --rate=96000 --channels=1
