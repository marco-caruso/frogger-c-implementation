#ifndef MUSICA_H
#define MUSICA_H
#include <SDL2/SDL.h>
//posso includere SDL direttamente qui: dipende da dove utilizziamo la musica
void init_sdl_audio();

void playAudio(const char *filename);

void stop_audio();

void cleanup_sdl_audio();


extern SDL_AudioSpec wavSpec;
extern Uint32 wavLength;
extern Uint8 *wavBuffer;
extern SDL_AudioDeviceID deviceId;
extern char *startMelody;
extern char *loseMelody;
extern char *winMelody;
extern char *menuMelody;
extern char *restartMelody;
extern int stopAudio;

#endif //MUSICA_H
