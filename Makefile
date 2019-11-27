BIN=
BIN+=resample

SRC=$(patsubst %,src/%.c,$(BIN))

# CC=gcc
# CC=clang
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

all: audiofiles $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ src/$@.c

data: resample
	rm -f data
	mkdir data
	ffmpeg -i hoff\ -\ innocence.flac -f s16be -acodec pcm_s16be -ac 1 data/hoff\ -\ innocence.44k1.s16be.pcm
	./resample -i 44100 -o 38000 < data/hoff\ -\ innocence.44k1.s16be.pcm > data/hoff\ -\ innocence.38k.s16be.pcm
	./resample -i 44100 -o 22050 < data/hoff\ -\ innocence.44k1.s16be.pcm > data/hoff\ -\ innocence.22k.s16be.pcm
	./resample -i 44100 -o 16000 < data/hoff\ -\ innocence.44k1.s16be.pcm > data/hoff\ -\ innocence.16k.s16be.pcm
	./resample -i 44100 -o 11050 < data/hoff\ -\ innocence.44k1.s16be.pcm > data/hoff\ -\ innocence.11k.s16be.pcm
	./resample -i 44100 -o  8000 < data/hoff\ -\ innocence.44k1.s16be.pcm > data/hoff\ -\ innocence.8k.s16be.pcm

original-sound: audiofiles
	pacat --raw --format

.PHONY: clean
clean:
	rm -rf $(BIN)
