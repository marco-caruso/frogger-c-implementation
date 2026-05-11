//rana.c
#include "rana.h"
#include "definizioni.h"
#include "utils.h"
#include "menu.h"
#include "grafica.h"

void draw_frog(int x, int y) {
    int color;
    if (y >= MARCIAPIEDESY && y < MARCIAPIEDESY+MARCIAPIEDE_Y){color = COL_RANA_MARC;}
    else if (y >= FIUMESY && y < FIUMESY + FIUME_Y){color = COL_RANA_FIUME;}
    else if (y >= RIVASY && y < RIVASY + RIVA_Y){color = COL_RANA_RIVA;}
    if (frogxy.on_croc) {color = COL_RANA_CROC;}
    wattron(gamewin, COLOR_PAIR(color));
    mvwprintw(gamewin, y, x-CROC_X-1, "%s", game_matrix[x][y].ch);
    wattroff(gamewin, COLOR_PAIR(color));

}

void ready_frog() {
    frogxy.x = FROG_START_X + CROC_X;
    frogxy.y = FROG_START_Y;
    frogxy.on_croc = false;
    frogxy.croc_pid = -1;
}



//funzione che controlla la rana
void* frog() {

    struct msg m; pid_t pid; int input; bool new_input = false; struct point f;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    flushinp();
    m.id = FROG_ID;
    m.pid = pthread_self();
    //IMPORTANTE!!! Chiudo lo stream di output di questo processo; la ragione per cui è necessario è che chiamando getch() si chiama anche refresh()
    //questo da problemi di coerenza (o quantomeno non garantisce una esecuzione corretta) in alcuni casi.
    //int fd = open("/dev/null", O_RDONLY);
    //dup2(fd, STDOUT_FILENO);
    //sfrog.x = FROG_STARTX;
    f.y = FROG_START_Y;
    m.x = 0;
    m.y = 0;
    while(true) {
       //pthread_mutex_lock(&mutex_refresh);
       input = (int)getch();
       //pthread_mutex_unlock(&mutex_refresh);

       //Controllo se c'è dell'input nello stream
       if (input != ERR) {new_input = true; }
       else {input = false; new_input = false;}

        switch (input) {

            case KEY_UP:
                m.x = 0;
                m.y = -FROG_Y; // Moving up
                f.y += m.y;
                break;

            case KEY_DOWN:
                m.x = 0;
            m.y = (frogxy.y < MARCIAPIEDESY) ? FROG_Y : 0;//se la rana si trova sul marciapiede inferiore, non può andare in basso
                f.y += m.y;
                break;

            case KEY_LEFT:
                m.x = -FROG_X;
                m.y = 0; // Moving left
                break;

            case KEY_RIGHT:
                m.x = FROG_X;
                m.y = 0; // Moving right
                break;

            case TAB:
                generate_thread(BULL_ID, -1, frogxy.y, -1, 0);
                generate_thread(BULL_ID, -1, frogxy.y, 1, 0);
                usleep(FROG_BULL_SLEEP);
                break;

            default:
                break;
        }

        pthread_testcancel();
        if ((new_input) && (m.x != 0 || m.y != 0) && (input != TAB)) {
           sem_wait(&sem_liberi);
           buffer[scrivi] = m;
           pthread_mutex_lock(&mutex);
           scrivi = (scrivi + 1) % DIM_BUFFER;
           pthread_mutex_unlock(&mutex);
           sem_post(&sem_occupati);
           m.x = 0; m.y = 0;
        }

        else {
           m.x = 0; m.y = 0;
           sem_wait(&sem_liberi);
           buffer[scrivi] = m;
           pthread_mutex_lock(&mutex);
           scrivi = (scrivi + 1) % DIM_BUFFER;
           pthread_mutex_unlock(&mutex);
           sem_post(&sem_occupati); }


        usleep(FROG_SLEEPS_TONIGHT);
    }
}

