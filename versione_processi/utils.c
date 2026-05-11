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
int pipe_inverse[2];
int pipe_inverse1[2];
int pipe_inverse2[2];

int tanen[5] = {0};
int score=0;

pid_t fpid;

extern int *croc_pids = NULL;    // Dynamic array to store crocodile PIDs
extern size_t croc_count = 0;    // Number of elements in the array
extern int *bull_pids = NULL;    // Dynamic array to store crocodile PIDs
extern size_t bull_count = 0;    // Number of elements in the array

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
   long int start_time = 0;
   long int tempo_passato = 0;
   score=200;//Durante ogni run, il punteggio inizierà da 200
   clock_gettime(CLOCK_REALTIME, (struct timespec[]){{0}});
   start_time = time(NULL); // Alternativa più semplice per ottenere il tempo iniziale
	while(true){

    //disegno barra del tempo, vite ecc...
      ready_field();
      wrefresh(gamewin);
	//inzia la manche
      manche();
	   tempo_passato = gettime(start_time);

	   // Sottrai punti in base ai secondi trascorsi
	   //wrefresh(gamewin);
	   score -= (tempo_passato/10); //decrementa di 1 ogni 6 secondi
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
	//ridisegno il box
      box(gamewin, 0, 0);
    }
}


//funzione che si occupa del ciclo di manche
void manche() {
   partita.manche_on = true; partita.loss = false; struct timespec start;
//misuro il tempo di inizio
   //graphics();
    //wrefresh(gamewin);
    clock_gettime(CLOCK_REALTIME, &start);
//disegno lo sfondo
    background();
//inizializziamo i flussi
    init_flussi();
//inizializzo la pipe
    init_pipe();
//inizializzo i processi
    init_processes();
//sono in lettura
    close(pipe_fds[1]);
//sono in scrittura
    close(pipe_inverse[0]);
//preparo la rana
    ready_frog();
//preapro la matrice settandola a -1
    ready_matrix();
    graphics();
    wrefresh(gamewin);
//disegnamo le vite adesso e non in graphics perché graphics si occupa di grafiche d
//continua finché non capita un evento di fine manche (es. la rana viene assassinata)

   //Rendiamo casuale la velocità di generazione dei coccodrilli per ogni manche
   srand(time(NULL));
   random_sleep = MIN_SLEEP + rand() % (MAX_SLEEP - MIN_SLEEP + 1);
   //
   //FARE UNA FUNZIONE PER GESIRLOOOO
   while(partita.manche_on) {
      time_bar_progress(start.tv_sec);

      //graphics();
      //process_data();
      wrefresh(gamewin);
//ricevo i dati dalle pipes
      get_data();
//processo i dati
      graphics();
      fflush(stdout);
      //controllo se il tempo è finito

      if (gettime(start.tv_sec) >= TIMEL) {
         partita.manche_on = false;

         partita.loss = true;
         kill(frogxy.pid, SIGKILL); waitpid(frogxy.pid, 0, 0);

      }

      usleep(MAIN_SLEEP);

   }

   kill_all();
}


//funzione che inizializza i processi
void init_processes() {
   generate_frog();
   generate_croc_father();
}


//funzione che riceve e processa i dati
void get_data() {
   struct msg temp;
   read(pipe_fds[0], &temp, sizeof(temp));
   update(temp);
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
         if (temp.first) {
         fpid = temp.pid;
         }
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
   if (temp.first) {
      add_pid(temp.pid, temp.id);
   }
//controllo se il coccodrillo è uscito dal campo di gioco
   if (croc_is_out_of_bounds(temp)) {kill_process(temp.pid); remove_pid(temp.pid, temp.id);}
   //if ((frog_on_water()) || frog_is_out_of_bounds()) {kill_process(frogxy.pid); partita.manche_on = false; partita.loss = true;}
   else {
      for(size_t i = 0; i < CROC_X; i++) {
         for(size_t j = 0; j < CROC_Y; j++) {
           if (/*game_matrix[temp.x+i][temp.y+j].id != FROG_ID &&*/ game_matrix[temp.x+i][temp.y+j].id != BULL_ID) {
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
   kill_process(frogxy.pid); partita.manche_on = false; partita.loss = true;}
   is_frog_on_croc();
   if (temp.pid == frogxy.croc_pid) {update_frog(temp.x_speed, 0, frogxy.pid); update_matrix_frog(true);}
   if (!is_frog_on_croc()) {frogxy.on_croc = false; frogxy.croc_pid = -1;}

}

//funzione che aggiorna la matrice relativamente ai proiettili
void update_matrix_bull(struct msg temp) {
   if (temp.first) {
      //for (int i = frogxy.x+abs(temp.x); i < frogxy.x + abs(temp.x) + BULL_X; i++) {
      /*
         for (int j = temp.y-1; j < temp.y + BULL_Y; j++) {
            game_matrix[i][j].id = BULL_ID;
            game_matrix[i][j].pid = temp.pid;
            strcpy(game_matrix[i][j].ch, SYMB_BULL);

         }
         */
         add_pid(temp.pid, temp.id);
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
      //}
   }

   else {

      for (int i = 0; i < CROC_X*2+DIM_X; i++) {
          if (game_matrix[i][temp.y].pid == temp.pid) {
             //beep();
             delete_old(temp.pid, false);
             if (bullet_is_out_of_bounds(i+temp.x_speed)) {kill_process(temp.pid); remove_pid(temp.pid, temp.id);}
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
   if (temp.first) {
      add_pid(temp.pid, temp.id);
   }
   //se i proiettili vanno fuori dalla schermata, vengono uccisi
   if (bullet_is_out_of_bounds(temp.x)) {
     kill_process(temp.pid); remove_pid(temp.pid, temp.id);}
   else {
   if (game_matrix[temp.x][temp.y].id == BULL_ID && game_matrix[temp.x][temp.y].pid > 0) {
     //caso in cui i proiettili della rana intersecano quelli del coccodrillo
      score+=5; //viene inserito solo alla fine della manche
      wrefresh(gamewin);
      kill_process(temp.pid); remove_pid(temp.pid, temp.id);
      kill_process(game_matrix[temp.x][temp.y].pid); remove_pid(temp.pid, temp.id);
      delete_old(game_matrix[temp.x][temp.y].pid, false);
   }
   else if (frog_collision(temp.x, temp.y)) {
      kill_process(frogxy.pid);
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
void update_frog(int x, int y, pid_t pid) {
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
   kill_process(frogxy.pid);delete_old(frogxy.pid, true); partita.manche_on = false; partita.loss = true;}

}

//funzione che cancella le vecchie entità
void delete_old(pid_t pid, bool frog) {
   for(size_t i = 0; i < DIM_X + CROC_X*2; i++) {
      for(size_t j = 0; j < DIM_Y; j++) {
         if (pid == game_matrix[i][j].pid) {
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
   generate_process(FROG_ID, 1, -1, -1, 0);
}

//funzione che genera il processo padre dei coccodrilli
void generate_croc_father() {
   generate_process(FCROC_ID, 1, -1, -1, 0);
}


//funzione che genera i processi
void generate_process(int id, int flusso, int x, int y, int speed) {
   pid_t pid;

   pid = fork();
   if (pid < 0) {perror("fork call"); _exit(2);}
   if (pid == 0) {
//sono in scrittura
// 🔹 Processo figlio
        //printf("FIGLIO creato: ID = %d, PID = %d, PPID = %d\n", id, getpid(), getppid());
      
      close(pipe_fds[0]);
      close(pipe_inverse[1]);
      close(pipe_inverse1[1]);
      close(pipe_inverse2[1]);
      switch(id) {

         case FROG_ID: //Gestione della rana
            int fd = open("/dev/null", O_RDONLY);
            dup2(fd, STDOUT_FILENO);
            frog();
            break;

         case FCROC_ID: //Creazione e inizializzazione dei coccodrilli (padre croc)
            croc_creator();
            break;

         case CROC_ID: //Gestione dei coccodrilli in gioco
            croc(flusso);
            /*
            void print_croc_pids() {
    		printf("Processi coccodrilli attivi:\n");
    		for (int i = 0; i < croc_count; i++) {
        		printf("Coccodrillo PID: %d\n", croc_pids[i]);
   	 			}
			}*/
            break;

         case BULL_ID: //Proiettili della rana
            bullet(x, y);
            break;

         case BULL_CROC_ID: //Proiettili del coccodrillo
            bullet_croc(x, y, speed);
            break;

         default:
            break;
      }
      _exit(2);
   }
   //else { } // non che serva, era solo comodo "concettualmente"
	else {
        // Processo padre
        //croc_pids[croc_count++] = pid; // Salvo il PID del figlio
        //printf("PADRE: ho creato un coccodrillo con PID = %d\n", pid);
    }
}

void add_pid(pid_t pid, int id) {
    if (id == CROC_ID) {
       croc_pids = realloc(croc_pids, (croc_count + 1) * sizeof(int));
       if (croc_pids == NULL) {
          perror("realloc failed");
          exit(EXIT_FAILURE);
      }
      croc_pids[croc_count] = pid;
      croc_count++;
     }
    else {
       bull_pids = realloc(bull_pids, (bull_count + 1) * sizeof(int));
       if (bull_pids == NULL) {
          perror("realloc failed");
          exit(EXIT_FAILURE);
        }
       bull_pids[bull_count] = pid;
       bull_count++;
    
    }
}

void remove_pid(pid_t pid, int id) {
    struct msg m;
    m.pid = pid; m.id = id;
    //write(pipe_inverse[1], &m, sizeof(m));
    if (id == CROC_ID) {
     write(pipe_inverse[1], &m, sizeof(m));
     for (size_t i = 0; i < croc_count; i++) {
        if (croc_pids[i] == pid) {
            memmove(&croc_pids[i], &croc_pids[i + 1], (croc_count - i - 1) * sizeof(int));
            croc_count--;
            croc_pids = realloc(croc_pids, croc_count * sizeof(int));
            if (croc_count == 0) {
                free(croc_pids);
                croc_pids = NULL;
            }
            break;
        }
      }
    }
    else {
       for (size_t i = 0; i < bull_count; i++) {
        if (id == BULL_ID) {write(pipe_inverse1[1], &m, sizeof(m));}
        else {write(pipe_inverse2[1], &m, sizeof(m));}
        if (bull_pids[i] == pid) {
            memmove(&bull_pids[i], &bull_pids[i + 1], (bull_count - i - 1) * sizeof(int));
            bull_count--;
            bull_pids = realloc(bull_pids, bull_count * sizeof(int));
            if (bull_count == 0) {
                free(bull_pids);
                bull_pids = NULL;
            }
            break;
        }
      }
    
    }
}



//Uccide ogni processo presente in gioco (tranne se stesso)
void kill_all() {
   /*
   for (size_t i = 0; i < CROC_X*2 + DIM_X; i++) {
      for (size_t j = 0; j < DIM_Y; j++) {
         kill_process(game_matrix[i][j].pid);
      }
   }*/
   int bull_temp_count = bull_count;
   for (size_t i = 0; i < bull_temp_count; i++) {
      kill_process(bull_pids[0]);
      remove_pid(bull_pids[0], BULL_ID);
   }
   usleep(500000);
   int temp_croc_count = croc_count;
   for (size_t i = 0; i < temp_croc_count; i++) {
      kill_process(croc_pids[0]);
      remove_pid(croc_pids[0], CROC_ID);
   }
   usleep(1200000);
   kill(fpid, SIGKILL);
   waitpid(fpid, 0, 0);
}

//Uccide il processo
void kill_process(pid_t pid)  {
   if (pid > 0) {
      kill(pid, SIGKILL);
      waitpid(pid, 0, WNOHANG);
   }
}


//funzione che inizializza la pipe
void init_pipe() {
   if (pipe(pipe_fds) == -1) {
        perror("Pipe call");
        exit(1);
    }
   if (pipe(pipe_inverse) == -1) {
        perror("Pipe call");
        exit(1);
    }
    int flags = fcntl(pipe_inverse[0], F_GETFL, 0);
    
    flags |= O_NONBLOCK;
    if (fcntl(pipe_inverse[0], F_SETFL, flags) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe_inverse1) == -1) {
        perror("Pipe call");
        exit(1);
    }
    int flags1 = fcntl(pipe_inverse1[0], F_GETFL, 0);
    flags1 |= O_NONBLOCK;
    if (fcntl(pipe_inverse1[0], F_SETFL, flags1) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
    
    
    if (pipe(pipe_inverse2) == -1) {
        perror("Pipe call");
        exit(1);
    }
    int flags2 = fcntl(pipe_inverse2[0], F_GETFL, 0);
    flags2 |= O_NONBLOCK;
    if (fcntl(pipe_inverse2[0], F_SETFL, flags2) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
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
                  if (game_matrix[i][j].pid > -1) {
                  wattron(gamewin, COLOR_PAIR(COL_COCCODRILLI));
                  mvwprintw(gamewin, j, i-CROC_X-1, "%s", game_matrix[i][j].ch);
                  wattroff(gamewin, COLOR_PAIR(COL_COCCODRILLI));
                  }
                  break;
               case BULL_ID:
                 if(j >= FROG_START_Y){
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

   if ((frog_on_water()) || frog_is_out_of_bounds()) {
   kill_process(frogxy.pid); partita.manche_on = false; partita.loss = true;}
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
               //game_polling();
            }

            else if (decision == 113) {
              //delwin(lose_win);
			//delwin(gamewin);
               endwin();
               exit(0);
            }

         }

}

