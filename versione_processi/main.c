//main.c
#include "definizioni.h"
#include "grafica.h"
#include "menu.h"

int main() {

   //funzione che si occupa di mostrare la schermata iniziale
   mostraSchermataIniziale();

   //richiamiamo il menu,
   //richiama game_init con l'inizializzazione di ncurses e dei colori
   // che nelle difficoltà richiama game_polling
   init_menu();

   //funzione che si occupa di inizializzare la schermata e i colori
   //game_init();

   //funzione che contiene la logica di gioco
   //game_polling();
}



