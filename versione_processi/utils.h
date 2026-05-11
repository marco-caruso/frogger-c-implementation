//utils.h
#ifndef ALTRO_H
#define ALTRO_H
#include "definizioni.h"

//firme delle funzioni

void lifes();
void time_bar();
//void tane();
//void tane();

//void draw_frog(int, int);
//void background();
void draw_matrix();
void graphics();
long int gettime(long int);
void time_bar_progress(long int);
void ready_field();
void ready_matrix();
void add_pid(pid_t, int);
void remove_pid(pid_t, int);
void kill_process(pid_t);
void kill_all();



bool check_frog_win();


void update_frog(int, int, pid_t);
void delete_old(pid_t, bool);
void update_matrix_croc_bull(struct msg);
void update_matrix_bull(struct msg);
void update_matrix_croc(struct msg);
void update_matrix_frog(bool);
void update(struct msg);

void get_data();
//void croc_creator();
void generate_process(int, int, int, int, int);
void init_processes();
void generate_frog();
void generate_croc_father();
void init_pipe();
void manche();
void run();


void endgame(bool);
bool all_tane_full();

void scores();


//extern bool is_paused;  // Variabile globale per lo stato della pausa
//extern pid_t *process_ids;  // Array dei PID dei processi di gioco (coccodrilli, proiettili, ecc.)
//extern int num_processes;  // Numero di processi attivi






#endif //ALTRO_H
