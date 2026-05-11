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
void croc_creator() {
    int rand_flux; int prev_rand_flux = -1;int counter=0;int status; int cont=0;
    srand(time(NULL));
    struct msg m; struct msg t;
    m.id = FCROC_ID;
    m.pid = getpid();
    m.first = true;
    write(pipe_fds[1], &m, sizeof(m));
    m.first = false;
    while(true) {
      	cont++;
        rand_flux = rand() % 8 + 1;

        //Aumentiamo la probabilità di spawn di un coccodrillo nel primo flusso
        if (cont==8 && rand_flux!=1){rand_flux=1;cont=0;}

        //Verifica che il flusso attuale sia diverso da quello precedente, in modo da non creare coccodrilli sovrapposti
        if (rand_flux != prev_rand_flux) {
            generate_process(CROC_ID, rand_flux, -1, -1, 0);
            m.x_speed = rand_flux;
            write(pipe_fds[1], &m, sizeof(m));
            counter++;
        }
        prev_rand_flux = rand_flux;
        read(pipe_inverse[0], &t, sizeof(t));
        if (t.id == CROC_ID) {waitpid(t.pid, &status, 0);}
        usleep(random_sleep);   
    }
}

const int SLEEP_FLUSSO_FACILE[] = {0, SLEEP_FLUSSO_1F,SLEEP_FLUSSO_2F,SLEEP_FLUSSO_3F,SLEEP_FLUSSO_4F,SLEEP_FLUSSO_5F,SLEEP_FLUSSO_6F,SLEEP_FLUSSO_7F,SLEEP_FLUSSO_8F};
const int SLEEP_FLUSSO_MEDIO[] = {0, SLEEP_FLUSSO_1F,SLEEP_FLUSSO_2F,SLEEP_FLUSSO_3F,SLEEP_FLUSSO_4F,SLEEP_FLUSSO_5F,SLEEP_FLUSSO_6F,SLEEP_FLUSSO_7F,SLEEP_FLUSSO_8F};
const int SLEEP_FLUSSO_DIFFICILE[] = {0, SLEEP_FLUSSO_1D,SLEEP_FLUSSO_2D,SLEEP_FLUSSO_3D,SLEEP_FLUSSO_4D,SLEEP_FLUSSO_5D,SLEEP_FLUSSO_6D,SLEEP_FLUSSO_7D,SLEEP_FLUSSO_8D};

//funzione che gestisce il comportamento di un singolo coccodrillo nel gioco
void croc(int flusso) {
    struct msg m; struct msg t; int counter = 0;
    m.y = MARCIAPIEDESY-flusso*FLUX_Y;
    m.x = flussi[flusso-1]*(DIM_X+CROC_X);
    m.id = CROC_ID;
    m.pid = getpid();
    m.first = true;
    m.x_speed = (flussi[flusso-1] == 0) ? (CROC_SPEED) : (-CROC_SPEED);
    srand(m.pid);


    while(true) {
        write(pipe_fds[1], &m, sizeof(m));//scriviamo la posizione del coccodrillo nella pipe
        m.first = false;
        m.id = CROC_ID;
        if (rand() % BULL_CHANCE == 0) {
            generate_process(BULL_CROC_ID, -1, m.x, m.y, m.x_speed); //generiamo un proiettile con una certa probabilità
        }
        m.x += m.x_speed;
        //usleep(CROC_SLEEP);
        read(pipe_inverse2[0], &t, sizeof(t));
        if (t.id == BULL_CROC_ID) {waitpid(t.pid, 0, 0);}
        //Ogni flusso ha una velocità differente, definita staticamente.
        //in base alle difficoltà, avremo degli usleep diversi
        if (flusso >= 1 && flusso <= 8) {
          if(flag_difficolta==MEDIO){usleep(SLEEP_FLUSSO_MEDIO[flusso]);}//medio
          else if(flag_difficolta==DIFFICILE){usleep(SLEEP_FLUSSO_DIFFICILE[flusso]);}//difficile
          else {usleep(SLEEP_FLUSSO_FACILE[flusso]);}//facile
        }
        waitpid(-1, NULL, WNOHANG);
        
    }

}


