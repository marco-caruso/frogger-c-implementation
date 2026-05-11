//proiettili.c
#include "proiettili.h"
#include "definizioni.h"


//funzione che genera i proiettili coccodrillo
void* bullet_croc(void* params) {
    //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    struct msg m;
    struct bull_params* par = (struct bull_params*)params;
    int speed = par->type;
    int y = par->y;
    int x = par->x;
    if (speed > 0) {m.x_speed = 1; m.x = x+CROC_X;}
    else {m.x_speed = -1; m.x = x;}
    m.pid = pthread_self();
    m.y = y+1;
    m.id = BULL_CROC_ID;
    while (true) {
        pthread_testcancel();
        sem_wait(&sem_liberi);
        buffer[scrivi] = m;
        //sem_post(&sem_occupati);
        pthread_mutex_lock(&mutex);
        scrivi = (scrivi + 1) % DIM_BUFFER;
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_occupati);
        m.x += m.x_speed;
        if(flag_difficolta==MEDIO){usleep(BULL_SLEEP_M/2);}//medio
        else if(flag_difficolta==DIFFICILE){usleep(BULL_SLEEP_D/2);}//difficile
        else {usleep(BULL_SLEEP_F/2);}//facile
    }
}


//funzione che genera i proiettili
void* bullet(void* params) {
    struct msg m;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    struct bull_params* par = (struct bull_params*)params;
    int y = par->x;
    int speed = par->y;
    m.y = y+1;
    m.id = BULL_ID;
    m.pid = pthread_self();
    m.x_speed = (speed >= 0) ? (BULL_SPEED) : (-BULL_SPEED);
    m.x = m.x_speed;
    m.first = true;
    while(true) {
        pthread_testcancel();
        sem_wait(&sem_liberi);
        buffer[scrivi] = m;
        pthread_mutex_lock(&mutex);
        scrivi = (scrivi + 1) % DIM_BUFFER;
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_occupati);
        m.first = false;
        if(flag_difficolta==MEDIO){usleep(BULL_SLEEP_M);}//medio
        else if(flag_difficolta==DIFFICILE){usleep(BULL_SLEEP_D);}//difficile
        else {usleep(BULL_SLEEP_F);}//facile
    }
}
