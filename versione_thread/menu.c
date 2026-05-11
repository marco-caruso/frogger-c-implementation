
#include "definizioni.h"
#include "menu.h"
#include "grafica.h"
#include "utils.h"
#include "musica.h"
WINDOW *menu;  // Puntatore alla finestra
int flag_difficolta=0;
int init_menu() {

    colori_menu();
    game_init();
    init_sdl_audio();
      playAudio(menuMelody);
    //keypad(stdscr, TRUE);   // Abilita i tasti speciali
    // Creazione della finestra del menu
    menu = newwin(MENU_Y, MENU_X, (LINES / 2 - MENU_Y / 2), (COLS / 2 - MENU_X / 2));

    keypad(menu, TRUE);   // Abilita i tasti speciali

    int highlight = 1;  // Indice dell'opzione attualmente evidenziata
    int scelta = 0;     // Scelta dell'utente
    int input;
    bool running = true;

    while (1) {
        // Stampa il menu solo se necessario
        print_menu(highlight);
        wrefresh(menu);

        input = wgetch(menu);

        // Controlla se il tasto premuto è tra quelli validi
        if (input == KEY_UP || input == KEY_DOWN || input == 10 || input == 27) {
            switch (input) {
                case KEY_UP:
                    highlight = (highlight == 1) ? 4 : highlight - 1; // Torna all'ultima opzione o passa a quella precedente
                    break;
                case KEY_DOWN:
                    highlight = (highlight == 4) ? 1 : highlight + 1; // Torna all'ultima opzione o passa a quella successiva
                    break;
                case 10:  // INVIO
                    scelta = highlight;
                    switch (scelta) {
                        case 1: apri_difficolta(); break;
                        case 2: apri_comandi(); break;
                        case 3: apri_regole(); break;
                        case 4: apri_crediti(); break;
                    }
                    break;
                default:
                  break;
            }
        }

    }
    //pulizia
    delwin(menu);
    endwin();
    return 0;
}

void colori_menu() {
    init_color(NERO,0,0,0);
    init_pair(EVIDENZIATORE, COLOR_YELLOW, NERO); // Testo evidenziato
    init_pair(MENU,COLOR_WHITE, NERO);// colore del menu
}

void print_menu(int highlight) {
      clean();

    int x = 6, y = 2;

    // Imposta il colore di sfondo della finestra
    wbkgd(menu, COLOR_PAIR(MENU));

    // Disegna il bordo della finestra
    wattron(menu, COLOR_PAIR(MENU));
    box(menu, ACS_VLINE, ACS_HLINE);
    wattroff(menu, COLOR_PAIR(MENU));

    mvwprintw(menu, 0, MENU_X/2-2, "MENU");
    mvwprintw(menu, MENU_Y-1, MENU_X/2-4, "INVIO/ESC");

    char *opzioni[] = {
        "1. Scegli difficolta'",
        "2. Comandi",
        "3. Regole",
        "4. Crediti"
    };

    // Stampa ogni opzione, evidenziando quella selezionata
    for (int i = 0; i < 4; ++i) {
        if (highlight == i + 1) {
            wattron(menu, COLOR_PAIR(EVIDENZIATORE));  // Evidenziazione
            mvwprintw(menu, y, x, "%s", opzioni[i]);
            wattroff(menu, COLOR_PAIR(EVIDENZIATORE));
        } else {
            mvwprintw(menu, y, x, "%s", opzioni[i]);
        }
        y += 2;  // Sposta verso il basso
    }
}

void apri_difficolta() {
    clean();
    WINDOW *difficolta = newwin(MENU_Y, MENU_X, (LINES / 2 - MENU_Y / 2), (COLS / 2 - MENU_X / 2));
    wbkgd(difficolta, COLOR_PAIR(MENU));
    box(difficolta, ACS_VLINE, ACS_HLINE);
    mvwprintw(difficolta, MENU_Y-1, MENU_X/2-4, "INVIO/ESC");
    keypad(difficolta, TRUE);
    int y = 1, x = 2;
    int highlight = 1; // Indice dell'opzione attualmente evidenziata
    int input;

    char *opzioni[] = {
        "1. Facile",
        "2. Media",
        "3. Difficile"
    };
    bool running = true;
    while (running) {
        // Stampa le opzioni con l'evidenziamento dell'opzione selezionata
        for (int i = 0; i < 3; ++i) {
            if (highlight == i + 1) {
                wattron(difficolta, COLOR_PAIR(EVIDENZIATORE)); // Evidenziazione
                mvwprintw(difficolta, y, x, "%s", opzioni[i]);
                wattroff(difficolta, COLOR_PAIR(EVIDENZIATORE));
            } else {
                mvwprintw(difficolta, y, x, "%s", opzioni[i]);
            }
            y += 2; // Sposta verso il basso
        }

        wrefresh(difficolta);
        input = wgetch(difficolta); // Ottieni l'input dell'utente

        switch (input) {
            case KEY_UP:
                highlight = (highlight == 1) ? 3 : highlight-1;break;// Torna all'ultima opzione o passa a quella precedente
            case KEY_DOWN:
                highlight = (highlight == 3) ? 1 : highlight+1;break;// Torna all'ultima opzione o passa a quella successiva
            case 10:  // INVIO
                switch (highlight) {
                    case 1:
                        stop_audio();stopAudio=1;flag_difficolta=FACILE;game_polling();
                        break;
                    case 2:
                        stop_audio();stopAudio=0;flag_difficolta=MEDIO;game_polling();
                        break;
                    case 3:
                        stop_audio();stopAudio=0;flag_difficolta=DIFFICILE;game_polling();
                        break;
                }
                delwin(difficolta);
                return;  // Esci dalla funzione dopo aver selezionato un'opzione
                break;
            case 27:  // ESC per uscire
                running = false;
                break;
            default:
                break;
        }

        // Resetta la posizione per stampare le opzioni
        y = 1;
    }

    // Pulizia
    delwin(difficolta);
    clean();
}



// Funzione per aprire la finestra di "Comandi"
void apri_comandi() {
    clean();
    WINDOW *comandi = newwin(MENU_Y, MENU_X, (LINES / 2 - MENU_Y / 2), (COLS / 2 - MENU_X / 2));
    wbkgd(comandi, COLOR_PAIR(MENU));
    box(comandi, ACS_VLINE, ACS_HLINE);
    int y=1,x=2;
    mvwprintw(comandi, y, x, "Comandi:");y+=2;
    mvwprintw(comandi, y, x, "-FRECCIE DIREZIONALI: muovi rana");y+=1;
    mvwprintw(comandi, y, x, "-TAB: spara proiettili");
    mvwprintw(comandi, MENU_Y-1, MENU_X/2-2, "ESC");
    wrefresh(comandi);
    //------
    int input;
    bool running = true;
    while (running) {
        input = wgetch(comandi);
        if (input == 27) { // ESC
            running = false;
        }
    }
    delwin(comandi);
}

// Funzione per aprire la finestra di "Regole"
void apri_regole() {
    clean();
    int y=1,x=2;
    WINDOW *regole = newwin(REGOLE_Y, REGOLE_X, (LINES / 2 - REGOLE_Y / 2), (COLS / 2 - REGOLE_X / 2));
    wbkgd(regole, COLOR_PAIR(MENU));
    box(regole, ACS_VLINE, ACS_HLINE);
    mvwprintw(regole, y, x, "Regole");y+=2;
    mvwprintw(regole, y, x, "-L'obiettivo e' far arrivare la rana in tutte e 5 le tane.");y+=2;
    mvwprintw(regole, y, x, "-Si parte da un totale di 5 vite nella modalita' facile, 4 in medio e");y+=1;
    mvwprintw(regole, y, x, " 3 in difficile");y+=2;
    mvwprintw(regole, y, x, "-Si hanno 60 secondi di tempo per ogni manche.");y+=2;
    mvwprintw(regole, y, x, "-La Rana parte dal centro dell'argine inferiore. Puo' muoversi in alto,");y+=1;
    mvwprintw(regole, y, x, " in basso, a destra e a sinistra con l'utilizzo delle frecce direzionali.");y+=2;
    mvwprintw(regole, y, x, "-La Rana dovrà attraversare il fiume salendo sopra i coccodrilli.");y+=1;
    mvwprintw(regole, y, x, " Questi sparano orizzontalmente dei proiettili, e se si viene colpiti ");y+=1;
    mvwprintw(regole, y, x, " si perde 1 vita. La rana a sua volta, con TAB puo' sparare 2 proiettili " );y+=1;
    mvwprintw(regole, y, x, " orizzontalmente (1 a dx e 1 a sx) e questi sono in grado di distruggere " );y+=1;
    mvwprintw(regole, y, x, " i proiettili dei coccodrilli, se collidono." );y+=2;
    mvwprintw(regole, y, x, "-Una volta che la rana raggiungerà una tana, quest'ultima " );y+=1;
    mvwprintw(regole, y, x, " si chiuderà e iniziera' un'altra manche." );y+=2;
    mvwprintw(regole, y, x, "-Se non si riesce a raggiungere una tana nel tempo prefissato," );y+=1;
    mvwprintw(regole, y, x, " si perde una vita e riniziera' un'altra manche." );y+=2;
    mvwprintw(regole, y, x, "-Il gioco termina con la vittoria se si chiudono tutte e 5 le tane," );y+=1;
    mvwprintw(regole, y, x, " mentre termina con la sconfitta se si perdono tutte le vite." );y+=1;
    mvwprintw(regole, REGOLE_Y-1, REGOLE_X/2-2, "ESC");
    wrefresh(regole);

    int input;
    bool running = true;
    while (running) {
        input = wgetch(regole);
        if (input == 27) { // ESC
            running = false;
        }
    }
    delwin(regole);
}

// Funzione per aprire la finestra di "Crediti"
void apri_crediti() {
    clean();
    int y=1,x=2;
    WINDOW *crediti = newwin(MENU_Y, MENU_X, (LINES / 2 - MENU_Y / 2), (COLS / 2 - MENU_X / 2));
    wbkgd(crediti, COLOR_PAIR(MENU));
    box(crediti, ACS_VLINE, ACS_HLINE);
    mvwprintw(crediti, y, x, "Crediti:");y+=2;
    mvwprintw(crediti, y, x, "Progetto sviluppato da:");y+=1;
    mvwprintw(crediti, y, x, "Mauro Contu e Marco Caruso");
    mvwprintw(crediti, MENU_Y-1, MENU_X/2-2, "ESC");
    wrefresh(crediti);
    int input;
    bool running = true;
    while (running) {
        input = wgetch(crediti);
        if (input == 27) { // ESC
            running = false;
        }
    }
    delwin(crediti);
}

void clean(){
    werase(menu);
    wrefresh(menu);
    clear();
    refresh();
}

