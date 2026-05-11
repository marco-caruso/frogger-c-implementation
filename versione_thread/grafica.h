//grafica.h
#ifndef GRAFICA_H
#define GRAFICA_H

struct point get_screen_size();
struct point generate_win_start();
bool is_screen_size_ok();
void screen_size_loop();
void create_game_win();
void create_init_win();
void game_init();
void game_polling();
void mostraSchermataIniziale();
int mostra_schermata_lose();
int mostra_schermata_win();
void init_sdl_audio();
void playAudio(const char *filename);
void stop_audio();
void cleanup_sdl_audio();


#endif //GRAFICA_H
