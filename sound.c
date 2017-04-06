
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define SAMPLE_RATE 22100
#define DURATION 5
#define VOICES 3
#define VOLUME 48

typedef struct voice {
    uint32_t frequency;
    uint8_t volume;
    size_t duration;
    size_t position;
    int8_t *sample;
} voice_t;

voice_t *createVoice(uint32_t frequency, uint8_t volume);
void destroyVoice(voice_t *voice);
int8_t voiceNext(voice_t* voice);
int8_t *sample(uint8_t volume, int duration);
uint32_t duration(uint32_t frequency);

int main() {
    uint32_t frequencies[VOICES] = {440, 660, 550};
    voice_t *voices[VOICES] = {NULL};

    for (int v = 0; v < VOICES; v++) {
        voices[v] = createVoice(frequencies[v], VOLUME);
    }

    for (int i = 0; i < SAMPLE_RATE * DURATION; i++) {
        int8_t voice = 0;
        for (size_t v = 0; v < VOICES; v++) {
            voice += voiceNext(voices[v]);
        }
        int bytes = write(1, &voice, 1);
    }

    return 0;
}

// Create a voice
voice_t *createVoice(uint32_t frequency, uint8_t volume) {
    voice_t *new_voice = malloc(sizeof (voice_t));

    new_voice->frequency = frequency;
    new_voice->volume = volume;
    new_voice->duration = duration(frequency);
    new_voice->position = 0;
    new_voice->sample = sample(volume, new_voice->duration);

    return new_voice;
}

// Destroy a voice
void destroyVoice(voice_t *voice) {
    free(voice->sample);
    free(voice);
}

// Get the next value for the voice
int8_t voiceNext(voice_t* voice) {
    int8_t value = voice->sample[voice->position];
    voice->position++;
    voice->position %= voice->duration;
    return value;
}

// Generats a sawtooth wave at duration/RATE Hz
int8_t *sample(uint8_t volume, int duration) {
    int8_t *sample_space = malloc(volume*2);
    uint8_t rise = volume;
    int8_t floor = - volume / 2;
    uint32_t run = duration / 2;
    
    if (rise >= run) {
        // steep slope
        uint8_t slope = rise / run;

        // Draw upward slope
        for (int s = 0; s < run; s++) {
            sample_space[s] = (s * slope) + floor; 
        }

        // Drawdownward slope
        for (int s = 0; s < run; s++) {
            sample_space[s + run] = volume - (s * slope) + floor;
        }
    } else {
        // shallow slope
        uint32_t step = run / rise;

        // Draw upward slope
        for (int s = 0; s < run; s++) {
            sample_space[s] = (s / step) + floor;
        }

        // Draw downward slope
        for (int s = 0; s < run; s++) {
            sample_space[s + run] = volume - (s / step) + floor;
        }
    }

    return sample_space;
}

// Get the sample duration for a period of a given frequency
uint32_t duration(uint32_t frequency) {
    uint32_t dur = SAMPLE_RATE / frequency;

    return (dur / 4) * 4; // Closest multiple of 4
}
