//utils.h
#ifndef ALTRO_H
#define ALTRO_H
#include "definizioni.h"

//firme delle funzioni

void lifes();
void time_bar();

void draw_matrix();
void graphics();
long int gettime(long int);
void time_bar_progress(long int);
void ready_field();
void ready_matrix();

void kill_thread(pthread_t);
void kill_all();
void init_sem();
void destroy_sems();



bool check_frog_win();


void update_frog(int, int, pthread_t);
void delete_old(pthread_t, bool);
void update_matrix_croc_bull(struct msg);
void update_matrix_bull(struct msg);
void update_matrix_croc(struct msg);
void update_matrix_frog(bool);
void update(struct msg);

void get_data();
//void croc_creator();
void generate_process(int, int, int, int, int);
void init_threades();
void generate_frog();
void generate_croc_father();
void init_pipe();
void manche();
void run();
void clean_buffer();


void endgame(bool);
bool all_tane_full();

void scores();
void send_signal(pthread_t);

//extern bool is_paused;  // Variabile globale per lo stato della pausa
//extern pid_t *process_ids;  // Array dei PID dei processi di gioco (coccodrilli, proiettili, ecc.)
//extern int num_processes;  // Numero di processi attivi






#endif //ALTRO_H
