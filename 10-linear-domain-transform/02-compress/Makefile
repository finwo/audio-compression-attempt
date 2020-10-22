SRC=$(wildcard *.c)
BIN=$(patsubst %.c,%,$(SRC))

CC=clang
CFLAGS?=
CFLAGS+= -O3 -s
CFLAGS+= -I.
CFLAGS+= -lm
CFLAGS+= -fno-stack-protector
CFLAGS+= -fomit-frame-pointer
CFLAGS+= -fno-math-errno
CFLAGS+= -fmerge-all-constants
CFLAGS+= -fno-ident
CFLAGS+= -ffast-math
CFLAGS+= -W

# SL=1
# SL=2
# SL=4
# SL=8
# SL=16
# SL=32
# SL=64
# SL=128
# SL=256
# SL=512
# SL=1024
# SL=2048
# SL=4096
# SL=8192
# SL=16384
SL=32768
CH=1
B=320000

all: $(BIN)

$(BIN): $(SRC) helpers.h
	$(CC) $(CFLAGS) -o $@ $@.c

.PHONY: clean
clean:
	rm -rf $(BIN)

.PHONY: sound
sound: $(BIN)
	cat ../../data/hoff\ -\ innocence.44k1.s16be.pcm | ./encode -r $(SL) -b $(B) | pv -B 1 | ./decode -r $(SL) | pacat --raw --format=s16be --rate=44100 --channels=1

.PHONY: jinjer
jinjer: $(BIN)
	yes | ffmpeg -i ~/Videos/Music\ Clips/Jinjer/JINJER\ -\ Pisces\ \(Live\ Session\)\ _\ Napalm\ Records-SQNtGoM3FVU.webm -f s16be -acodec pcm_s16be -ac $(CH) -filter:a "volume=-3dB" ../../data/jinjer-pisces.44k1.s16be.pcm
	cat ../../data/jinjer-pisces.44k1.s16be.pcm | ./encode -r $(SL) -b $(B) | pv | ./decode -r $(SL) | pacat --raw --format=s16be --rate=44100 --channels=$(CH)

.PHONY: ola
ola: $(BIN)
	yes | ffmpeg -i ~/Videos/Music\ Clips/OLA\ ENGLUND\ -\ Cerberus\ -\ \(NEW\ SINGLE\ 2019\)-SvYVSJteqF4.mkv -f s16be -acodec pcm_s16be -ac $(CH) -filter:a "volume=-3dB" ../../data/ola-englund-cerberus.44k1.s16be.pcm
	cat ../../data/ola-englund-cerberus.44k1.s16be.pcm | ./encode -r $(SL) -b $(B) | pv | ./decode -r $(SL) | pacat --raw --format=s16be --rate=44100 --channels=$(CH)

.PHONY: atmos
atmos: $(BIN)
	yes | ffmpeg -i ../../data/dolby-atmos-3VZFpwlXKpg.mkv -f s16be -acodec pcm_s16be -ar 44100 -ac $(CH) -filter:a "volume=-3dB" ../../data/dolby-atmos-3VZFpwlXKpg.44k1.s16be.pcm
	cat ../../data/dolby-atmos-3VZFpwlXKpg.44k1.s16be.pcm | ./encode -r $(SL) -b $(B) | pv | ./decode -r $(SL) | pacat --raw --format=s16be --rate=44100 --channels=$(CH)

# .PHONY: ola14
# ola14: $(BIN)
# 	# 96000 / 120 = 800hz modulation
# 	yes | ffmpeg -i ~/Videos/Ola\ Challenges/Riff\ Challenge/14/ola\ englund\ challenge\ 14.wav -f s16be -acodec pcm_s16be -ac 1 -filter:a "volume=-3dB" ../data/ola-challenge-14.44k1.s16be.pcm
# 	cat ../data/ola-challenge-14.44k1.s16be.pcm | ./encode --$(W) -c 1 -r $(SL) -n $(NF) | pv | ./decode -r $(SL) | pacat --raw --format=s16be --rate=44100 --channels=1
