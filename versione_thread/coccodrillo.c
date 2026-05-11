//coccodrillo.c
#include "coccodrillo.h"
#include "definizioni.h"
#include "utils.h"



int flussi[N_FLUSSI];
//funzione che inizializza i flussi
void init_flussi() {
    srand(time(NULL));
    int r = rand() % 2;
    //alterniamo i flussi
    for (size_t i = 0; i < N_FLUSSI; i++) {
        if (r == 0) {
            flussi[i] = (i % 2 == 0) ? (0) : (1);
        }
        else {flussi[i] = (i % 2 == 0) ? (1) : (0);}
    }
}

//funzione che crea i coccodrilli in modo casuale negli 8 flussi (genera lui il processo CROC_ID).
//Genera quindi i coccodrilli un un flusso casuale
//funzione che genera i coccodrilli
void* croc_creator() {
    //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    srand(time(NULL)); struct msg m;int cont=0;
    int rand_flux; int prev_rand_flux = -1;
    m.id = FCROC_ID;
    m.pid = pthread_self();
    sem_wait(&sem_liberi);
    buffer[scrivi] = m;
    //sem_post(&sem_occupati);
    pthread_mutex_lock(&mutex);
    scrivi = (scrivi + 1) % DIM_BUFFER;
    pthread_mutex_unlock(&mutex);
    sem_post(&sem_occupati);
    while(true) {
      cont++;
        //Aumentiamo la probabilità di spawn di un coccodrillo nel primo flusso
        if (cont==8 && rand_flux!=1){rand_flux=1;cont=0;}
        pthread_testcancel();
        rand_flux = rand() % 8 + 1;
        if (rand_flux != prev_rand_flux) {
            generate_thread(CROC_ID, rand_flux, -1, -1, 0);
        }
        prev_rand_flux = rand_flux;
        usleep(random_sleep);

    }
}

const int SLEEP_FLUSSO_FACILE[] = {0, SLEEP_FLUSSO_1F,SLEEP_FLUSSO_2F,SLEEP_FLUSSO_3F,SLEEP_FLUSSO_4F,SLEEP_FLUSSO_5F,SLEEP_FLUSSO_6F,SLEEP_FLUSSO_7F,SLEEP_FLUSSO_8F};
const int SLEEP_FLUSSO_MEDIO[] = {0, SLEEP_FLUSSO_1F,SLEEP_FLUSSO_2F,SLEEP_FLUSSO_3F,SLEEP_FLUSSO_4F,SLEEP_FLUSSO_5F,SLEEP_FLUSSO_6F,SLEEP_FLUSSO_7F,SLEEP_FLUSSO_8F};
const int SLEEP_FLUSSO_DIFFICILE[] = {0, SLEEP_FLUSSO_1D,SLEEP_FLUSSO_2D,SLEEP_FLUSSO_3D,SLEEP_FLUSSO_4D,SLEEP_FLUSSO_5D,SLEEP_FLUSSO_6D,SLEEP_FLUSSO_7D,SLEEP_FLUSSO_8D};

//funzione che gestisce il comportamento di un singolo coccodrillo nel gioco
//funzione che crea il coccodrillo
void* croc(void* params) {
    //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    struct msg m;
    struct croc_params* par = (struct croc_params*)params;
    int flusso = par->flusso;
    m.y = MARCIAPIEDESY-flusso*FLUX_Y;
    m.x = flussi[flusso-1]*(DIM_X+CROC_X);
    m.id = CROC_ID;
    m.pid = pthread_self();
    m.x_speed = (flussi[flusso-1] == 0) ? (CROC_SPEED) : (-CROC_SPEED);
    srand(time(NULL));
    while(true) {
        pthread_testcancel();
        sem_wait(&sem_liberi);
        buffer[scrivi] = m;
        //sem_post(&sem_occupati);
        pthread_mutex_lock(&mutex);
        scrivi = (scrivi + 1) % DIM_BUFFER;
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_occupati);
        //generiamo un proiettile con una certa probabilità
        //m.x+((m.x_speed/abs(m.x_speed))*3) --> facciamo spawnare il proiettile un po' più lontano dal coccodrillo per evitare collisioni indesiderate
        //nel caso in cui la rana risieda sulla bocca del coccodrillo
        if (rand() % BULL_CHANCE == 0) {
            generate_thread(BULL_CROC_ID, -1, m.x+((m.x_speed/abs(m.x_speed))*3), m.y, m.x_speed);
        }
        m.x += m.x_speed;
        //Ogni flusso ha una velocità differente, definita staticamente.
        //in base alle difficoltà, avremo degli usleep diversi
        if (flusso >= 1 && flusso <= 8) {
            if(flag_difficolta==MEDIO){usleep(SLEEP_FLUSSO_MEDIO[flusso]);}//medio
            else if(flag_difficolta==DIFFICILE){usleep(SLEEP_FLUSSO_DIFFICILE[flusso]);}//difficile
            else {usleep(SLEEP_FLUSSO_FACILE[flusso]);}//facile
        }
    }
}



