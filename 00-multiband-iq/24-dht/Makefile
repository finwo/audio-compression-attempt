SRC=$(wildcard *.c)
BIN=$(patsubst %.c,%,$(SRC))

CC=clang
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

NF=20
W=dbC
C=2
SL=$(shell echo "4096 * $(C)" | bc)

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $@.c

.PHONY: clean
clean:
	rm -rf $(BIN)

.PHONY: sound
sound: $(BIN)
	# 96000 / 120 = 800hz modulation
	cat ../data/hoff\ -\ innocence.44k1.s16be.pcm | ./encode --$(W) -c 1 -r $(SL) -n $(NF) | pv -B 1 | ./decode -r $(SL) | pacat --raw --format=s16be --rate=44100 --channels=1

.PHONY: jinjer
jinjer: $(BIN)
	# 96000 / 120 = 800hz modulation
	yes | ffmpeg -i ~/Videos/Music\ Clips/Jinjer/JINJER\ -\ Pisces\ \(Live\ Session\)\ _\ Napalm\ Records-SQNtGoM3FVU.webm -f s16be -acodec pcm_s16be -ac $(C) -filter:a "volume=-3dB" ../data/jinjer-pisces.44k1.s16be.pcm
	cat ../data/jinjer-pisces.44k1.s16be.pcm | ./encode --$(W) -c $(C) -r $(SL) -n $(NF) | pv | ./decode -r $(SL) | pacat --raw --format=s16be --rate=44100 --channels=$(C)

.PHONY: ola
ola: $(BIN)
	# 96000 / 120 = 800hz modulation
	yes | ffmpeg -i ~/Videos/Music\ Clips/OLA\ ENGLUND\ -\ Cerberus\ -\ \(NEW\ SINGLE\ 2019\)-SvYVSJteqF4.mkv -f s16be -acodec pcm_s16be -ac $(C) -filter:a "volume=-3dB" ../data/ola-englund-cerberus.44k1.s16be.pcm
	cat ../data/ola-englund-cerberus.44k1.s16be.pcm | ./encode --$(W) -c $(C) -r $(SL) -n $(NF) | pv | ./decode -r $(SL) | pacat --raw --format=s16be --rate=44100 --channels=$(C)

.PHONY: atmos
atmos: $(BIN)
	# 96000 / 120 = 800hz modulation
	yes | ffmpeg -i ../data/dolby-atmos-3VZFpwlXKpg.mkv -f s16be -acodec pcm_s16be -ar 44100 -ac $(C) -filter:a "volume=-3dB" ../data/dolby-atmos-3VZFpwlXKpg.44k1.s16be.pcm
	cat ../data/dolby-atmos-3VZFpwlXKpg.44k1.s16be.pcm | ./encode --$(W) -c $(C) -r $(SL) -n $(NF) | pv | ./decode -r $(SL) | pacat --raw --format=s16be --rate=44100 --channels=$(C)

.PHONY: ola14
ola14: $(BIN)
	# 96000 / 120 = 800hz modulation
	yes | ffmpeg -i ~/Videos/Ola\ Challenges/Riff\ Challenge/14/ola\ englund\ challenge\ 14.wav -f s16be -acodec pcm_s16be -ac 1 -filter:a "volume=-3dB" ../data/ola-challenge-14.44k1.s16be.pcm
	cat ../data/ola-challenge-14.44k1.s16be.pcm | ./encode --$(W) -c 1 -r $(SL) -n $(NF) | pv | ./decode -r $(SL) | pacat --raw --format=s16be --rate=44100 --channels=1
