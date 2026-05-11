//elementi statici del gioco
//elementi.c
#include "elementi.h"
#include "definizioni.h"

//funzione stampa riva
void riva() {
    wattron(gamewin, COLOR_PAIR(COL_RIVA));
    for(int i = 0; i < RIVA_Y; i++) {
        for(int j = 0; j < RIVA_X; j++) {
            mvwprintw(gamewin, RIVASY + i, RIVASX + j, " ");
        }
    }
    wattroff(gamewin, COLOR_PAIR(COL_RIVA));
}
//funzione stampa fiume
void fiume() {
    wattron(gamewin, COLOR_PAIR(COL_FIUME));
    for(int i = 0; i < FIUME_Y; i++) {
        for(int j = 0; j < FIUME_X; j++) {
            mvwprintw(gamewin, FIUMESY + i, FIUMESX + j, " ");
        }
    }
    wattroff(gamewin, COLOR_PAIR(COL_FIUME));
}
//funzione stampa marciapiede
void marciapiede(){
    wattron(gamewin, COLOR_PAIR(COL_MARCIAPIEDE));
    for(int i = 0; i < MARCIAPIEDE_Y; i++) {
        for(int j = 0; j < MARCIAPIEDE_X; j++) {
            mvwprintw(gamewin, MARCIAPIEDESY + i, MARCIAPIEDESX + j, " ");
        }
    }
    wattroff(gamewin, COLOR_PAIR(COL_MARCIAPIEDE));
}




//funzione che disegna una tana
void draw_tana(int x) {
    wattron(gamewin, COLOR_PAIR(COL_TANE));
    for(int i = 0; i < TANEY; i++) {
        for(int j = 0; j < TANE_LARGE; j++) {
            mvwprintw(gamewin, TANESY + i, x + j, " ");
        }
    }
    wattroff(gamewin, COLOR_PAIR(COL_TANE));
}

//funzione stampa tane
void tane(){

    wattron(gamewin, COLOR_PAIR(COL_MARCIAPIEDE));
    for(int i = 0; i < TANEY; i++) {
        for(int j = 0; j < TANEX; j++) {
            mvwprintw(gamewin, TANESY + i, TANESX + j, " ");
        }
    }
    wattroff(gamewin, COLOR_PAIR(COL_MARCIAPIEDE));
    for (size_t i = 0; i < 5; i++) {
        if (tanen[i] == 0) {
            draw_tana(TANESX+TANE_LARGE*i+12*(i+1));
        }
    }

}

//funzione che disegna lo sfondo
void background() {
    tane();
    riva();
    fiume();
    marciapiede();
}


