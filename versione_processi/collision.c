//Qua ci sono tutte le collisioni
//collision.c
#include "collision.h"
#include "definizioni.h"

//Richiamo le struct
//struttura rana (il processo manda solo posizioni relative)
struct point frogxy;
//matrice di gioco
struct msg game_matrix[DIM_X+2*CROC_X][DIM_Y];
struct game partita;

//Ci dice se la rana è sul coccodrillo (o marciapiede)
bool frog_is_drowning() {
    if (game_matrix[frogxy.x-1][frogxy.y].id != CROC_ID && game_matrix[frogxy.x+FROG_X][frogxy.y].id != CROC_ID) {
        return true;
    }
    return false;
}

//verifica la posizione rispetto al movimento del coccodrillo. Verifica che la rana sia al centro del coccodrillo
bool croc_on_both() {
    frogxy.croc_pid = game_matrix[frogxy.x-1][frogxy.y].pid;
    for (size_t i = frogxy.y; i < frogxy.y+FROG_Y; i++) {
        if ((game_matrix[frogxy.x-1][i].id != CROC_ID && game_matrix[frogxy.x-1][i].id != BULL_ID) || (game_matrix[frogxy.x+FROG_X][i].id != CROC_ID && game_matrix[frogxy.x+FROG_X][i].id != BULL_ID)) {
            return false;
        }
    }
    frogxy.croc_pid = game_matrix[frogxy.x-1][frogxy.y].pid;
    return true;
}

//Verifica se la rana non sborda a destra
bool croc_on_right() {
    for (size_t i = frogxy.y; i < frogxy.y+FROG_Y; i++) {
        if (game_matrix[frogxy.x-1][i].id != CROC_ID && game_matrix[frogxy.x-1][i].id != BULL_ID) {
            return false;
        }
    }
    if ((game_matrix[frogxy.x+FROG_X-CROC_X][frogxy.y].id == CROC_ID || game_matrix[frogxy.x+FROG_X-CROC_X][frogxy.y].id == BULL_ID) && (game_matrix[frogxy.x-CROC_X+1][frogxy.y].id != CROC_ID && game_matrix[frogxy.x-CROC_X+1][frogxy.y].id != BULL_ID)) {frogxy.croc_pid = game_matrix[frogxy.x-1][frogxy.y].pid; return true;}

    return false;
}

//Verifica se la rana non sborda a sinistra
bool croc_on_left() {
    for (size_t i = frogxy.y; i < frogxy.y+FROG_Y; i++) {
        if (game_matrix[frogxy.x+FROG_X][i].id != CROC_ID && game_matrix[frogxy.x+FROG_X][i].id != BULL_ID) {
            // beep();
            return false;
        }
    }
    if ((game_matrix[frogxy.x+CROC_X-1][frogxy.y].id == CROC_ID || game_matrix[frogxy.x+CROC_X-1][frogxy.y].id == BULL_ID ) && (game_matrix[frogxy.x+CROC_X+FROG_X-1][frogxy.y].id != CROC_ID && game_matrix[frogxy.x+CROC_X+FROG_X-1][frogxy.y].id != BULL_ID)) {frogxy.croc_pid = game_matrix[frogxy.x+FROG_X][frogxy.y].pid; return true;}

    return false;
}

//funzione che ritorna true se la rana è sull'acqua
bool frog_on_water() {
    //IMPORTANTE! è necessaria questa inzializzazione (o metterla come prima condizione nell'if). Se non attuata il compilatore svolge solo una condizione, vede che è falsa e ignora le altre. E noi vogliamo che la funziona venga eseguita.
    bool is_on = is_frog_on_croc();
    if (frogxy.y < MARCIAPIEDESY && frogxy.y > FIUMESY && !is_on) {
        return true;
    }

    return false;
}

//funzione che ritorna true se un coccodrillo è fuori dai limiti della finestra
bool croc_is_out_of_bounds(struct msg temp) {
    if ((temp.x > DIM_X-1+CROC_X && temp.x_speed > 0) || (temp.x <= 0 && temp.x_speed < 0)) {
        return true;
    }
    return false;
}

//funzione che controlla se la rana è out of bounds
bool frog_is_out_of_bounds() {
    if (frogxy.x < CROC_X+1 || frogxy.x > CROC_X+DIM_X-FROG_X-1) {
        return true;
    }
    return false;
}

//funzione che controlla se la rana è sul coccodrillo (controlla i bordi)
bool is_frog_on_croc() {
    if ((croc_on_both() || croc_on_right() || croc_on_left()) && (!frog_is_drowning()))
  	{frogxy.on_croc = true; return true;}
    else {return false;}
}

//funzione che controlla che il proiettile non sia uscito dallo schermo
bool bullet_is_out_of_bounds(int x) {
    if ((x > DIM_X+CROC_X) || (x < CROC_X)) {return true;}
    return false;
}

//funzione che controlla se il proiettile ha colpito la rana
bool frog_collision(int x, int y) {
    if (game_matrix[x][y].id == FROG_ID) {
        return true;
    }
    return false;
}