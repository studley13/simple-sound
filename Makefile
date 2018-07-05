CC=clang
C_ARGS="-O"
SOX=sox
RATE=22100
SOX_ARGS=-e signed-integer -c 1 -r ${RATE} -b 8 -t raw -

EVERYTHING=sound outfile.wav spectrogram.png outfile.mp3

all: ${EVERYTHING}

.PHONY:clean
clean:
	rm ${EVERYTHING}

sound: sound.c

outfile.wav: sound
	./sound | ${SOX} ${SOX_ARGS} $@

%.mp3: %.wav
	ffmpeg -v quiet -y -i $^ $@

spectrogram.png: sound
	./sound | ${SOX} ${SOX_ARGS} -n spectrogram

