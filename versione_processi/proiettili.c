//proiettili.c
#include "proiettili.h"
#include "definizioni.h"


//funzione che genera i proiettili coccodrillo
void bullet_croc(int x, int y, int speed) {
    struct msg m;
    if (speed > 0) {m.x_speed = 1; m.x = x+CROC_X;} //Proiettili sparati verso sinistra
    else {m.x_speed = -1; m.x = x;} //Proiettili sparati verso sinistra
    m.pid = getpid();
    m.y = y+1; //Proiettile che parte dal centro del coccodrillo
    m.id = BULL_CROC_ID;
    m.first = true;
    while (true) {
        write(pipe_fds[1], &m, sizeof(m)); //Invia la posizione del proiettile
        m.first = false;
        m.x += m.x_speed; //Aggiorna la posizione del proiettile

        if(flag_difficolta==MEDIO){usleep(BULL_SLEEP_M/2);}//medio
        else if(flag_difficolta==DIFFICILE){usleep(BULL_SLEEP_D/2);}//difficile
        else {usleep(BULL_SLEEP_F/2);}//facile
    }
}


//funzione che genera i proiettili della rana
void bullet(int y, int speed) {
    struct msg m;
    //struct msg m;
    m.y = y+1; //Proiettili che partono dal centro della rana
    m.id = BULL_ID;
    m.pid = getpid();
    m.x_speed = (speed >= 0) ? (BULL_SPEED) : (-BULL_SPEED); //Velocità orizzontale del proiettile (negativa per quelli che vanno a sinistra)
    m.x = m.x_speed;
    m.first = true; //Indica se è il primo ciclo del proiettile

        while(true) {
            write(pipe_fds[1], &m, sizeof(m));
            m.first = false;
            if(flag_difficolta==MEDIO){usleep(BULL_SLEEP_M);}//medio
            else if(flag_difficolta==DIFFICILE){usleep(BULL_SLEEP_D);}//difficile
            else {usleep(BULL_SLEEP_F);}//facile

        }


}
