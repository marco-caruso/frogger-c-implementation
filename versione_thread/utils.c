#include "definizioni.h"
#include "utils.h"
#include "collision.h"
#include "rana.h"
#include "coccodrillo.h"
#include "elementi.h"
#include "proiettili.h"
#include "menu.h"
#include "grafica.h"


//finestra di gioco
WINDOW *gamewin;

//variabili globali
int vite;
int pipe_fds[2];

int tanen[5] = {0};
int score=0;
int leggi=0, scrivi=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_refresh = PTHREAD_MUTEX_INITIALIZER;
struct msg buffer[DIM_BUFFER];
sem_t sem_liberi;
sem_t sem_occupati;
pthread_t fcroc_tid;

//La prima velocità di generazione di coccodrilli sarà questa, per le successive manche questa sarà casuale
int random_sleep=850000;

char frog_sprite[FROG_Y*FROG_X][FROG_X] = {"▙", "█", "█" ,"▟", " ", "█", "█", " ", "█","▀", "▀", "█"};
char croc_sprite[CROC_Y*CROC_X][CROC_X] = {"▄", "▄", "▄", "▄", "▮", "▃", "▃", "▃", "▃", "▁", " "," "," "," "," "," ",
   "▚", "▚", "▚", "▚", "▚", "█", "█", "█", "█", "█", "█", "█", "▅", "▄", "▃", "▁",
   "▀", "▀", "▀", "▀", "▀", "▐", "▛", "▀", "▀", "▐", "▛", "▀", "▀", "▀", "█", "◤"};
char croc_sprite1[CROC_Y*CROC_X][CROC_X]={" "," "," "," "," "," ", "▁","▃","▃","▃","▃","▮","▄","▄","▄","▄",
   "▁","▃","▄","▅","█","█","█","█","█","█","█","▞","▞","▞","▞","▞",
   "◥","█","▀","▀","▀","▜","▋","▀","▀","▜","▋","▀","▀","▀","▀","▀"};

//-----------------------------------------------

//-----------------------------------------------

//funzione che si occupa del loop secondario (manche) del gioco
void run() {
   vite = VITE;tanen[0]=0; tanen[1] = 0; tanen[2] = 0;tanen[3] = 0;tanen[4] = 0;
   score=200;//Durante ogni run, il punteggio inizierà da 200

	while(true){

    //disegno barra del tempo, vite ecc...
      ready_field();
      wrefresh(gamewin);
	//inzia la manche
      manche();

	//pulisco lo schermo
      wclear(gamewin);

	//se la manche è stata persa decremento le vite
      if (partita.loss) {
        if(flag_difficolta==MEDIO){score-=15;}
        else if(flag_difficolta==DIFFICILE){score-=20;}
        else{score-=10;}
         vite -= 1;
      }
      if (all_tane_full()) {
        if(flag_difficolta==MEDIO){score+=1500;}
        else if(flag_difficolta==DIFFICILE){score+=2000;}
        else{score+=1000;}

         endgame(true);

      }
      else if (vite == 0) {
        if(flag_difficolta==MEDIO){score-=250;}
        else if(flag_difficolta==DIFFICILE){score-=500;}
        else{score-=100;}
         endgame(false);
      }
      //wrefresh(gamewin);
	//ridisegno il box
      box(gamewin, 0, 0);
    }
}



//funzione che si occupa del ciclo di manche
void manche() {
   partita.manche_on = true; partita.loss = false; struct timespec start;
   //misuro il tempo di inizio
   clock_gettime(CLOCK_REALTIME, &start);
   //disegno lo sfondo
   background();
   //inizializziamo i flussi
   init_flussi();
   //inizializzo la pipe
   init_sem();
   //sono in lettura
   //preparo la rana
   ready_frog();
   //preapro la matrice settandola a -1
   ready_matrix();
   graphics();
   pthread_mutex_lock(&mutex_refresh);
   wrefresh(gamewin);
   pthread_mutex_unlock(&mutex_refresh);
   //inizializzo i processi
   init_threades();
   //Rendiamo casuale la velocità di generazione dei coccodrilli per ogni manche
   srand(time(NULL));
   random_sleep = MIN_SLEEP + rand() % (MAX_SLEEP - MIN_SLEEP + 1);
   //disegnamo le vite adesso e non in graphics perché graphics si occupa di grafiche d
   //continua finché non capita un evento di fine manche (es. la rana viene assassinata)
   while(partita.manche_on) {
      time_bar_progress(start.tv_sec);
      wrefresh(gamewin);
      //ricevo i dati dalle pipes
      get_data();
      //processo i dati
      graphics();

      if (gettime(start.tv_sec) >= TIMEL) {
         partita.manche_on = false;
         partita.loss = true;

      }

      usleep(MAIN_SLEEP);

   }

   kill_all();
   usleep(10000);
   clean_buffer();

   //distruggo i semafori
   destroy_sems();
}



//funzione che inizializza i processi
void init_threades() {
   generate_frog();
   generate_croc_father();
}


//funzione che riceve e processa i dati
void get_data() {
   struct msg temp;
   //aspetto ci sia qualcosa da leggere
   sem_wait(&sem_occupati);
   //ricevo i dati sulle mie entità dal buffer
   temp = buffer[leggi];
   leggi = (leggi + 1) % DIM_BUFFER;
   //incremento il contatore
   sem_post(&sem_liberi);
   update(temp);
}
//funzione che pulisce il buffer
void clean_buffer() {
   struct msg temp;
   for (size_t i = 0; i < DIM_BUFFER; i++) {
      buffer[i] = temp;
   }
}
//funzione che aggiorna i dati
void update(struct msg temp) {
   switch(temp.id) {

      //aggiorno i dati relativi all'oggetto rana
      case FROG_ID:
         update_frog(temp.x, temp.y, temp.pid);
      update_matrix_frog(true);
      break;

      case CROC_ID:
         update_matrix_croc(temp);
      break;

      case FCROC_ID:
         fcroc_tid = temp.pid;
      break;

      case BULL_ID:
         update_matrix_bull(temp);
      break;

      case BULL_CROC_ID:
         update_matrix_croc_bull(temp);
      break;

      default:
         break;
   }

}

//funzione che aggiorna la matrice relativamente ai coccodrilli
void update_matrix_croc(struct msg temp) {
   delete_old(temp.pid, false);
   //controllo se il coccodrillo è uscito dal campo di gioco
   if (croc_is_out_of_bounds(temp)) {kill_thread(temp.pid);}
   //if ((frog_on_water()) || frog_is_out_of_bounds()) {kill_process(frogxy.pid); partita.manche_on = false; partita.loss = true;}
   else {
      for(size_t i = 0; i < CROC_X; i++) {
         for(size_t j = 0; j < CROC_Y; j++) {
            if (/*game_matrix[temp.x+i][temp.y+j].id != FROG_ID &&*/ game_matrix[temp.x+i][temp.y+j].id != BULL_ID && game_matrix[temp.x+i][temp.y+j].id != BULL_CROC_ID) {
               //if (game_matrix[i][j].id == BULL_ID) {beep();}
               game_matrix[temp.x+i][temp.y+j].id = CROC_ID;
               game_matrix[temp.x+i][temp.y+j].pid = temp.pid;
               if (temp.x_speed > 0) {
                  strcpy(game_matrix[temp.x+i][temp.y+j].ch, croc_sprite1[i+CROC_X*j]);
               }
               else {strcpy(game_matrix[temp.x+i][temp.y+j].ch, croc_sprite[i+CROC_X*j]);}
            }
         }
      }

   }
   if ((frog_on_water()) || frog_is_out_of_bounds()) {
      kill_thread(frogxy.pid); delete_old(frogxy.pid, false); partita.manche_on = false; partita.loss = true;}
   is_frog_on_croc();
   if (temp.pid == frogxy.croc_pid) {update_frog(temp.x_speed, 0, frogxy.pid); update_matrix_frog(true); }
   if (!is_frog_on_croc()) {frogxy.on_croc = false; frogxy.croc_pid = -1;}

}


//funzione che aggiorna la matrice relativamente ai proiettili
void update_matrix_bull(struct msg temp) {
   if (temp.first) {
      if (temp.x_speed > 0) {
         game_matrix[frogxy.x+FROG_X][temp.y].id = BULL_ID;
         game_matrix[frogxy.x+FROG_X][temp.y].pid = temp.pid;
         strcpy(game_matrix[frogxy.x+FROG_X][temp.y].ch, SYMB_BULL);
      }
      else {
         game_matrix[frogxy.x-1][temp.y].id = BULL_ID;
         game_matrix[frogxy.x-1][temp.y].pid = temp.pid;
         strcpy(game_matrix[frogxy.x-1][temp.y].ch, SYMB_BULL);

      }

   }

   else {

      for (int i = 0; i < CROC_X*2+DIM_X; i++) {
         if (pthread_equal(game_matrix[i][temp.y].pid, temp.pid))
        {
            delete_old(temp.pid, false);
            if (bullet_is_out_of_bounds(i+temp.x_speed)) {kill_thread(temp.pid);}
            else {
               game_matrix[i+temp.x_speed][temp.y].id = BULL_ID;
               game_matrix[i+temp.x_speed][temp.y].pid = temp.pid;
               strcpy(game_matrix[i+temp.x_speed][temp.y].ch, SYMB_BULL);
               i = CROC_X*2+DIM_X;
            }
        }
      }
   }

}


//funzione che controlla se la rana ha chiuso una tana
bool check_frog_win() {
   for (size_t i = 0; i < 5; i++) {
      if (frogxy.x >= TANESX+12*(i+1)+TANE_LARGE*i+CROC_X && frogxy.x < TANESX+12*(i+1)+TANE_LARGE*(i+1)+CROC_X-FROG_X && frogxy.y < RIVASY)
      {

         if (tanen[i] == 0) {
            partita.manche_on = false; partita.loss = false; tanen[i] = 1;score+=100; break;}
         else {partita.manche_on = false; partita.loss = true; kill(frogxy.pid, SIGKILL); waitpid(frogxy.pid, 0, 0); break;}
      }
   }
   if (frogxy.y < RIVASY && partita.manche_on) {
      kill(frogxy.pid, SIGKILL); waitpid(frogxy.pid, 0, 0);
      partita.manche_on = false; partita.loss = true;
   }
}

//funzione che aggiorna la matrice relativamente ai proiettili coccodrillo
void update_matrix_croc_bull(struct msg temp) {
   delete_old(temp.pid, false);
   if (bullet_is_out_of_bounds(temp.x)) {kill_thread(temp.pid);}
   else {
      if (game_matrix[temp.x][temp.y].id == BULL_ID && !(pthread_equal(game_matrix[temp.x][temp.y].pid, -1))) {
         score+=5; //viene inserito solo alla fine della manche
         kill_thread(temp.pid);
         kill_thread(game_matrix[temp.x][temp.y].pid);
         delete_old(game_matrix[temp.x][temp.y].pid, false);
      }
      else if (frog_collision(temp.x, temp.y)) {
         kill_thread(frogxy.pid);
         delete_old(frogxy.pid, false);
         partita.manche_on = false;
         partita.loss = true;

      }
      else{
         game_matrix[temp.x][temp.y].id = BULL_CROC_ID;
         game_matrix[temp.x][temp.y].pid = temp.pid;

         wattron(gamewin, COLOR_PAIR(COL_TANE));
         if (temp.x_speed > 0) {strcpy(game_matrix[temp.x][temp.y].ch, RIGHT_BULL);}
         else {strcpy(game_matrix[temp.x][temp.y].ch, LEFT_BULL);}
         wattroff(gamewin, COLOR_PAIR(COL_TANE));
      }
   }
}

//funzione che si occupa di aggiornare i dati relativi alla rana
void update_frog(int x, int y, pthread_t pid) {
   frogxy.x += x;
   frogxy.y += y;
   frogxy.pid = pid;

   //controlliamo se la rana ha cambiato posizione
   if (x != 0 || y != 0) {frogxy.change = true;}
   else {frogxy.change = false;}
}

//funzione che si occupa di aggiornare la matrice delle posizioni
//se need è false allora l'update sta arrivando da update_matrix_croc e non è necessario controllare se la nuova posizione della rana è lecita (DEPRECATED)
void update_matrix_frog(bool need) {
   delete_old(frogxy.pid, true);
   check_frog_win();
   for(size_t i = 0; i < FROG_X; i++) {
      for(size_t j = 0; j < FROG_Y; j++) {
         if (game_matrix[frogxy.x+i][frogxy.y+j].id != BULL_ID) {
            game_matrix[frogxy.x+i][frogxy.y+j].id = FROG_ID;
            game_matrix[frogxy.x+i][frogxy.y+j].pid = frogxy.pid;
            strcpy(game_matrix[frogxy.x+i][frogxy.y+j].ch, frog_sprite[i+FROG_X*j]);
         }
      }
   }
   if ((frog_on_water() && need) || frog_is_out_of_bounds()) {
      kill_thread(frogxy.pid);  delete_old(frogxy.pid, true); partita.manche_on = false; partita.loss = true;}

}

//funzione che cancella le vecchie entità
void delete_old(pthread_t pid, bool frog) {
   for(size_t i = 0; i < DIM_X + CROC_X*2; i++) {
      for(size_t j = 0; j < DIM_Y; j++) {
         if (pthread_equal(game_matrix[i][j].pid, pid)) {
            if (frog && frogxy.on_croc /*&& game_matrix[i][j].id != BULL_ID*/) {
               game_matrix[i][j].id = CROC_ID;
               game_matrix[i][j].pid = -1;
            }
            //else if (game_matrix[i][j].id == BULL_ID) {
            //}
            else if (game_matrix[i][j].id == BULL_ID) {game_matrix[i][j].id = CROC_ID;
               game_matrix[i][j].pid = -1; }
            else{
               game_matrix[i][j].pid = -1;
               game_matrix[i][j].id = -1;

            }

         }
      }
   }

}



//fuznione che setta tutte le celle della matrice a -1
void ready_matrix() {
   //preparo la matrice con i valori iniziali corretti
   for (size_t i = 0; i < DIM_X+CROC_X*2; i++) {
      for (size_t j = 0; j < DIM_Y; j++) {
         game_matrix[i][j].id = -1;
         game_matrix[i][j].pid = -1;
      }
   }

   //setto la posizione iniziale della rana nella matrice
   for (size_t i = frogxy.y; i < frogxy.y+FROG_Y; i++) {
      for(size_t j = frogxy.x; j < frogxy.x+FROG_X; j++) {
         game_matrix[j][i].pid = -1;
         game_matrix[j][i].id = FROG_ID;
         strcpy(game_matrix[j][i].ch, frog_sprite[j+FROG_X*i]);
      }
   }
}

//funzione che genera il processo della rana
void generate_frog() {
   generate_thread(FROG_ID, 1, -1, -1, 0);
}

//funzione che genera il processo padre dei coccodrilli
void generate_croc_father() {
   generate_thread(FCROC_ID, 1, -1, -1, 0);
}


//funzione che genera i processi
void generate_thread(int id, int flusso, int x, int y, int speed) {
   pthread_t tid; struct croc_params cparams; struct bull_params bparams;

   //sono in scrittura
   switch(id) {

      case FROG_ID:
         //sleep(5);
            pthread_create(&tid, NULL, &frog, NULL);
      break;

      case FCROC_ID:
         //beep();
            //sleep(7);
               pthread_create(&tid, NULL, &croc_creator, NULL);
      break;

      case CROC_ID:
         cparams.flusso = flusso;
      if(flusso < 0) {
         int a = 3;
      }
      pthread_create(&tid, NULL, &croc, &cparams);
      usleep(10000);
      break;

      case BULL_ID:
         bparams.x = x;
      bparams.y = y;
      pthread_create(&tid, NULL, &bullet, &bparams);
      usleep(1000);
      break;

      case BULL_CROC_ID:
         bparams.x = x;
      bparams.y = y;
      bparams.type = speed;
      pthread_create(&tid, NULL, &bullet_croc, &bparams);
      usleep(1000);
      break;

      default:
         break;
   }
}

//Mando un segnale: lo uso per avvisare le piante di non mandare messaggi per un certo intervallo di tempo
void send_signal(pthread_t tid) {
   if (pthread_equal(tid, -1)) {
      pthread_kill(tid, SIGUSR1);
      usleep(500);
   }
}

//Uccide ogni processo presente in gioco (tranne se stesso)
void kill_all() {
   pthread_t arr[500] = {0}; int count = 0; bool not_killed = true;
   kill_thread(fcroc_tid);
   sleep(2);
   for (size_t i = 0; i < CROC_X*2 + DIM_X; i++) {
      for (size_t j = 0; j < DIM_Y; j++) {
         if(!pthread_equal(-1, game_matrix[i][j].pid)) {
            kill_thread(game_matrix[i][j].pid);
            delete_old(game_matrix[i][j].pid, false);
         }

      }
   }
}

//Uccide il thread
void kill_thread(pthread_t pid)  {
   if (!(pthread_equal(pid, -1))) {
      pthread_cancel(pid);
      usleep(5000);
      pthread_join(pid, NULL);
      usleep(200);
   }
}


//Inizializzo i semafori
void init_sem() {
   sem_init(&sem_occupati, 0, 0);
   sem_init(&sem_liberi, 0, DIM_BUFFER);
   leggi = 0; scrivi = 0;
}
//distruggo i semafori
void destroy_sems() {
   sem_destroy(&sem_occupati);
   sem_destroy(&sem_liberi);
}


//funzione che disegna vite e barra del tempo
void ready_field() {
   lifes();
   time_bar();
   scores();

}

//funzione che disegna le vite
void lifes() {
   wattron(gamewin, COLOR_PAIR(COL_VITE));
   for (int i = 0; i < vite; i++) {
      mvwprintw(gamewin, VITE_Y, VITE_X + i*2, SYMB_VITE);
   }
   wattroff(gamewin, COLOR_PAIR(COL_VITE));
}

void scores(){
  mvwprintw(gamewin, VITE_Y, DIM_X/2-5 , "Punteggio: %d", score);
  wrefresh(gamewin);
}

//funzione che disegna la barra del tempo piena
void time_bar() {

   wattron(gamewin, COLOR_PAIR(COL_TIME));
   for (int i = 0; i < END_TIME_X-2; i++) {
      mvwprintw(gamewin, TIME_Y, START_TIME_X+i, " ");
   }
   wattroff(gamewin, COLOR_PAIR(COL_TIME));


}

//funzione che mostra il progresso della barra del tempo, cambiando colore da verde a rosso
void time_bar_progress(long int start) {
   long int time_passed = gettime(start);//1....2....3....4....5

   // Calcoliamo la percentuale del tempo passato in relazione a TIMEL
   float time_percent = (float)time_passed / TIMEL;

   // Calcoliamo il valore RGB per il colore (verde a rosso)
   int R = (int)(time_percent * 1000); // Il rosso aumenta
   int G = (int)((1 - time_percent) * 1000); // Il verde diminuisce
   int B = 0; // Non usiamo il blu in questo caso

   int ADAPT = 40;
   init_color(ADAPT, R, G, B);
   init_pair(40, COLOR_WHITE, ADAPT);

   int barra_mancante=round(time_passed * 2.5);//aggiunge la parte mancante allo start della barra

   // Stampa la barra
   wattron(gamewin, COLOR_PAIR(40));
   for (int i = START_TIME_X; i < END_TIME_X + barra_mancante - round(time_passed * 2.5); i++) {
      mvwprintw(gamewin, TIME_Y, i, " ");
   }
   wattroff(gamewin, COLOR_PAIR(40));

   // Cancella la parte restante della barra
   for (int i = END_TIME_X - round(time_passed * 2.5); i < END_TIME_X; i++) {
      mvwprintw(gamewin, TIME_Y, i, " ");
   }

}


//funzione che ritorna il tempo passato da quando è stato misurato start
long int gettime(long int start){
   struct timespec end; long int time_passed;
   clock_gettime(CLOCK_REALTIME, &end);
   time_passed = end.tv_sec - start;
   return time_passed;
}

//funzione che disegna le grafiche
void graphics() {
   background();
   draw_matrix();
   box(gamewin, 0, 0);
}


//funzione che disegna ciò che è salvato nella matrice di gioco
void draw_matrix() {

   for (size_t i = CROC_X; i < DIM_X+CROC_X; i++) {
      for (size_t j = 0; j < DIM_Y; j++) {
         switch(game_matrix[i][j].id) {
            case FROG_ID:
               draw_frog(i, j);
            break;
            case CROC_ID:
               if (!pthread_equal(game_matrix[i][j].pid, -1)) {
                  wattron(gamewin, COLOR_PAIR(COL_COCCODRILLI));
                  mvwprintw(gamewin, j, i-CROC_X-1, "%s", game_matrix[i][j].ch);
                  wattroff(gamewin, COLOR_PAIR(COL_COCCODRILLI));
               }
            break;
            case BULL_ID:
               if(j>=FROG_START_Y){
                  wattron(gamewin, COLOR_PAIR(COL_BULL_RANA_MARC));
                  mvwprintw(gamewin, j, i-CROC_X-1, "%s", game_matrix[i][j].ch);
                  wattroff(gamewin, COLOR_PAIR(COL_BULL_RANA_MARC));
               }
               else{
                  wattron(gamewin, COLOR_PAIR(COL_BULL_RANA));
                  mvwprintw(gamewin, j, i-CROC_X-1, "%s", game_matrix[i][j].ch);
                  wattroff(gamewin, COLOR_PAIR(COL_BULL_RANA));
               }
            break;
            case BULL_CROC_ID:
               wattron(gamewin, COLOR_PAIR(COL_BULL_CROC));
            mvwprintw(gamewin, j, i-CROC_X-1, "%s", game_matrix[i][j].ch);
            wattroff(gamewin, COLOR_PAIR(COL_BULL_CROC));
            break;

            default:
               break;
         }
      }
   }
   fflush(stdout);;
   //is_frog_on_croc();
   if ((frog_on_water()) || frog_is_out_of_bounds()) {
      kill_thread(frogxy.pid); delete_old(frogxy.pid, false); partita.manche_on = false; partita.loss = true;}
}



//--------------------------------------------
//controlla se tutte le tane sono occupate
bool all_tane_full() {
   for (size_t i = 0; i < 5; i++) {
      if (tanen[i] != 1) {
         return false;
      }
   }
   return true;
}

//processa la fine della partita
void endgame(bool win) {
  int decision;
    if (!win) {
      decision=mostra_schermata_lose();
    }
    else {decision=mostra_schermata_win();}
         wrefresh(gamewin);
         while (true) {
            if (decision == 115) {
              //endwin();
              init_menu();
            }

            else if (decision == 113) {
               endwin();
               exit(0);
            }

         }

}

