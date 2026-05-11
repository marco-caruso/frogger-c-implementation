//definizioni.h
#ifndef DEFINIZIONI_H
#define DEFINIZIONI_H

//Librerie
#include <locale.h>
#include <ncurses.h>
#include <sys/prctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <math.h>
//#include <SDL2/SDL.h>


//MACRO DI INIZIALIZZAZIONE
#define CURS 0 //visibilità cursore
#define ND 0 //valore di nodelay
#define KP 1 //keypad

//DIMENSIONI SCHERMO & FRIENDS
#define DIM_X 155 //larghezza campo di gioco
#define DIM_Y 42 //altezza campo di gioco
#define VITE_X 1 //coord x delle vite
#define VITE_Y DIM_Y-2 //coord y delle vite
#define START_TIME_X 2
#define END_TIME_X DIM_X-3
#define TIME_Y 1
#define TANEX DIM_X-2
#define TANEY 3
#define RIVA_X DIM_X-2
#define RIVA_Y 4
#define FIUME_X DIM_X-2
#define FIUME_Y 24
#define MARCIAPIEDE_X DIM_X-2
#define MARCIAPIEDE_Y 4
#define TANESX 1
#define TANESY 3
#define RIVASX 1
#define RIVASY TANESY+TANEY
#define FIUMESX 1
#define FIUMESY RIVASY+RIVA_Y
#define MARCIAPIEDESX 1
#define MARCIAPIEDESY FIUMESY+FIUME_Y
#define FLUX_Y FIUME_Y/8
#define TANE_LARGE 16

//COLORI
#define COL_VITE 1
#define COL_TIME 2
#define COL_RIVA 3
#define COL_MARCIAPIEDE 4
#define COL_FIUME 5
#define COL_RANA 6
#define COL_RANA_RIVA 7
#define COL_RANA_FIUME 8
#define COL_RANA_MARC 9
#define COL_COCCODRILLI 10
#define COL_TANE 11
#define COL_RANA_CROC 12
#define COL_BULL_CROC 13
#define COL_BULL_RANA 14
#define COL_START 15
#define COL_LOSE 16
#define COL_WIN 17
#define COL_BULL_RANA_MARC 18

//COLORI PERSONALIZZATI
#define VERDE_RANA 100
#define ROSSO_ARGINE 101
#define NERO 102

//ID
#define FROG_ID 0
#define FCROC_ID 1
#define CROC_ID 2
#define BULL_ID 3
#define BULL_CROC_ID 4

//macro testo
#define AVVISO_DIM_SCHERMO "INGRANDIRE LO SCHERMO" //stampato quando la dimensione dello schermo è inferiore al minimo

//dimensioni entità
#define FROG_X 4
#define FROG_Y 3
#define CROC_X 16
#define CROC_Y FROG_Y
#define BULL_X 3
#define BULL_Y 2
#define TANA_X 6
#define TANA_Y 4

//MISCELLANOUS
#define VITE 5
#define SYMB_VITE "♥ "
#define SYMB_BULL "☣"
#define RIGHT_BULL "⁍"
#define LEFT_BULL "⁌"
#define TIMEL 60
#define TAB 32
#define FROG_START_X DIM_X/2
#define FROG_START_Y MARCIAPIEDESY
#define N_FLUSSI 8
#define CROC_SPEED 1 //Direzione coccodrilli
#define BULL_SPEED 1
#define BULL_CHANCE 100 //PROB massima di spawn di un proiettile di un coccodrillo

//sleep vari
#define FROG_SLEEPS_TONIGHT 30000
#define MAIN_SLEEP 1
#define CREATOR_SLEEP 100000 //VELOCITÀ CREAZIONE COCCODRILLI
//#define CROC_SLEEP 90000
#define FROG_BULL_SLEEP 500000 //VELOCITÀ DI GENERAZIONE PROIETTILI DELLA RANA (evita le raffiche)
#define MIN_SLEEP 760000
#define MAX_SLEEP 830000

//#define BULL_SLEEP 60000 //VELOCITÀ DEI PROIETTILI DELLA RANA
#define BULL_SLEEP_F 60000 //VELOCITÀ DEI PROIETTILI DEI COCCODRILLI in modalità facile
#define BULL_SLEEP_M 50000 //VELOCITÀ DEI PROIETTILI DEI COCCODRILLI in modalità media
#define BULL_SLEEP_D 30000 //VELOCITÀ DEI PROIETTILI DEI COCCODRILLI in modalità difficile

//velocità per ogni flusso in modalità facile
#define SLEEP_FLUSSO_1F 85000
#define SLEEP_FLUSSO_2F 87500
#define SLEEP_FLUSSO_3F 91500
#define SLEEP_FLUSSO_4F 88000
#define SLEEP_FLUSSO_5F 93000
#define SLEEP_FLUSSO_6F 86000
#define SLEEP_FLUSSO_7F 89000
#define SLEEP_FLUSSO_8F 84000

//velocità per ogni flusso in modalità media
#define SLEEP_FLUSSO_1D 62000
#define SLEEP_FLUSSO_2D 72500
#define SLEEP_FLUSSO_3D 75000
#define SLEEP_FLUSSO_4D 65000
#define SLEEP_FLUSSO_5D 69000
#define SLEEP_FLUSSO_6D 71000
#define SLEEP_FLUSSO_7D 69500
#define SLEEP_FLUSSO_8D 68000

//velocità per ogni flusso in modalità difficile
#define SLEEP_FLUSSO_1D 40000
#define SLEEP_FLUSSO_2D 42000
#define SLEEP_FLUSSO_3D 44000
#define SLEEP_FLUSSO_4D 45000
#define SLEEP_FLUSSO_5D 43000
#define SLEEP_FLUSSO_6D 41000
#define SLEEP_FLUSSO_7D 39000
#define SLEEP_FLUSSO_8D 35000


#define PAUSA 112

//menu
#define MENU_X 40
#define MENU_Y 15
#define REGOLE_X 80
#define REGOLE_Y 30
#define MENU 50 //colore
#define EVIDENZIATORE 51

//rappresenta la posizione di un elemento (es. rana) e include informazioni aggiuntive come il PID del processo e lo stato
struct point {
    int x;
    int y;
    pid_t pid;
    bool change;
    bool on_croc;
    pid_t croc_pid;
};

//struttura dei messaggi che passo alla pipe
struct msg {
    int id;
    int x;
    int y;
    char ch[4];
    pid_t pid;
    int x_speed;
    int y_speed;
    bool first;
};

//struttura della partita (cioè ha i due campi che servono per valutare una manche)
struct game {
    bool manche_on;
    bool loss;
};

extern WINDOW *gamewin;
extern WINDOW *start_win;
extern WINDOW *menu;
extern WINDOW *lose_win;
extern WINDOW *win_win;

//struttura rana (il processo manda solo posizioni relative)
extern struct point frogxy;
//matrice di gioco
extern struct msg game_matrix[DIM_X+2*CROC_X][DIM_Y];
extern struct game partita;

extern int pipe_fds[2];
extern int pipe_inverse[2];
extern int pipe_inverse1[2];
extern int pipe_inverse2[2];

extern int tanen[5];

//gestione pausa
extern bool inpausa;

extern int score;

extern const int SLEEP_FLUSSO[];

extern int random_sleep;

extern int flag_difficolta;
#define FACILE 1000
#define MEDIO 1001
#define DIFFICILE 1002
#endif //DEFINIZIONI_H
