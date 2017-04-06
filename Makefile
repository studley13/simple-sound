CC=clang
C_ARGS="-O"
SOX=sox
RATE=22100
SOX_ARGS=-e signed-integer -c 1 -r $(RATE) -b 8 -t raw -

all: sound outfile.wav spectrogram.png

.PHONY:clean
clean:
	rm outfile.wav sound spectrogram.png

sound: sound.c
	$(CC) $(C_ARGS) -o $@ $^

outfile.wav: sound
	./sound | $(SOX) $(SOX_ARGS) $@

spectrogram.png: sound
	./sound | $(SOX) $(SOX_ARGS) -n spectrogram

