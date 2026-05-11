#include "musica.h"
#include "definizioni.h"
//Musiche prese da Pixabay
SDL_AudioSpec wavSpec;
Uint32 wavLength;
Uint8 *wavBuffer;
SDL_AudioDeviceID deviceId;
char *startMelody="start_melody.wav";
char *loseMelody="end_melody.wav";
char *winMelody="win.wav";
char *menuMelody="menu.wav";
char *restartMelody="restart.wav";
int stopAudio=0;

void init_sdl_audio(){
    SDL_Init(SDL_INIT_AUDIO);
    SDL_LoadWAV(startMelody,&wavSpec,&wavBuffer,&wavLength);
    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
}

void playAudio(const char *filename)
{
    char path[256];
    snprintf(path,sizeof(path),"music/%s",filename);
    SDL_LoadWAV(path,&wavSpec,&wavBuffer,&wavLength);
    SDL_QueueAudio(deviceId,wavBuffer,wavLength);
    SDL_PauseAudioDevice(deviceId,0);
}

void stop_audio()
{
    SDL_ClearQueuedAudio(deviceId);
}

void cleanup_sdl_audio()
{
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();
}


