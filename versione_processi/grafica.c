//grafica.c
#include "definizioni.h"
#include "grafica.h"
#include "utils.h"
#include "musica.h"
//#include "menu.h"
WINDOW *start_win;
WINDOW *lose_win;
WINDOW *win_win;


struct point get_screen_size() {
    struct point maxxy;
    getmaxyx(stdscr, maxxy.y, maxxy.x);
    return maxxy;
}

//funzione che ritorna le coordinate da passare a newwin per inizializzare la finestra di gioco
struct point generate_win_start() {
    struct point upper_left_corner;

    upper_left_corner = get_screen_size();
    //Voglio che il campo di gioco sia centrato rispetto allo schermo
    upper_left_corner.y = (upper_left_corner.y - DIM_Y) / 2;
    upper_left_corner.x = (upper_left_corner.x - DIM_X) / 2;

    return upper_left_corner;
}

//controlla se le dimensioni schermo sono conformi
bool is_screen_size_ok() {
    bool flag = true;
    if (get_screen_size().x <= DIM_X-20 || get_screen_size().y <= DIM_Y-20) {
        flag = false;
    }
    return flag;
}

//funzione che si assicura la schermata abbia dimensione adatta
void screen_size_loop(){
    while (!is_screen_size_ok()) {
        mvprintw(get_screen_size().y/2, get_screen_size().x/2 - 10, AVVISO_DIM_SCHERMO);
        refresh();
    }
}

//inizializza la schermata di gioco
void create_game_win() {
    gamewin = newwin(DIM_Y, DIM_X, generate_win_start().y, generate_win_start().x);
    box(gamewin, 0, 0);
    wbkgd(gamewin,COLOR_PAIR(COL_START));
    bkgd(COLOR_PAIR(COL_START));
}

void create_init_win(){
  start_win = newwin(DIM_Y, DIM_X, generate_win_start().y, generate_win_start().x);
  box(gamewin, 0, 0);
}

//inizializzazione di schermo e colori
void game_init() {
    //inizializzo lo standard screen. Abbiamo scelto di settare nodelay a false ma non è necessario.
    setlocale(LC_ALL, ""); initscr(); noecho(); curs_set(CURS); nodelay(stdscr, ND); keypad(stdscr, KP); cbreak(); start_color();
    //inizializzo i colori
    init_color(VERDE_RANA, 100, 600,0); //tonalità di verde chiaro per la rana. Verde_rana
    init_color(ROSSO_ARGINE,320,67,141); //tonalità rosso scuro pe l'argine superiore e inferiore. Rosso_argine
    init_color(NERO,0,0,0);// nero
    init_pair(COL_VITE, COLOR_RED, NERO);//colore delle vite
    init_pair(COL_TIME, NERO, COLOR_GREEN);//colore della barra del tempo
    init_pair(COL_RIVA, COLOR_BLUE, ROSSO_ARGINE);//colore dell'argine superiore
    init_pair(COL_MARCIAPIEDE, NERO, ROSSO_ARGINE);//colore del marciapiede
    init_pair(COL_FIUME, COLOR_YELLOW, COLOR_BLUE);//colore del fiume
    init_pair(COL_RANA_FIUME, VERDE_RANA, COLOR_BLUE);//colore della rana quando annega
    init_pair(COL_RANA_RIVA, VERDE_RANA, ROSSO_ARGINE);//colore della rana sull'argine superiore
    init_pair(COL_RANA_MARC, VERDE_RANA, ROSSO_ARGINE);//colore della rana sul marciapiede
    init_pair(COL_COCCODRILLI,NERO, COLOR_BLUE);//colore dei coccodrilli
    init_pair(COL_TANE, NERO, COLOR_YELLOW);//colore della zona delle tane
    init_pair(COL_RANA_CROC, VERDE_RANA, NERO);//colore della rana quando sta su un coccodrillo
    init_pair(COL_BULL_CROC, COLOR_YELLOW,COLOR_BLUE);//colori proiettili dei coccodrilli
    init_pair(COL_BULL_RANA, COLOR_RED,COLOR_BLUE);//colori proiettili della rana
    init_pair(COL_BULL_RANA_MARC, COLOR_RED,ROSSO_ARGINE);//colori proiettili della rana
    init_pair(COL_START, COLOR_WHITE, NERO);  // Colore verde su sfondo nero
    init_pair(COL_LOSE, COLOR_RED, NERO);  // Colore verde su sfondo nero
    init_pair(COL_WIN, VERDE_RANA, NERO);  // Colore verde su sfondo nero
}


//funzione che contiene il loop principale del gioco
void game_polling() {
    bool wanna_play = true;
    //il loop continua finché il giocatore non si stanca
    while (wanna_play) {
        wanna_play = false;
        //controllo che le dimensioni schermo vadano bene
        screen_size_loop();
        //IMPORTANTE!!! Il motivo per cui non inizializzo la finestra di gioco in game_init() è che
        //adesso ho la sicurezza che le coordinate usate per generare la finestra con generate_win_start() sono adatte allo schermo
        //grazie al while precedente
        //creiamo la finestra di gioco
        create_game_win();

        //loop di gioco
        run();
    }
}




void mostraSchermataIniziale() {

    game_init();
    screen_size_loop();

    init_sdl_audio();
    playAudio(startMelody);
    //create_init_win();
    start_win = newwin(DIM_Y, DIM_X, generate_win_start().y, generate_win_start().x);
    box(gamewin, 0, 0);

    wbkgd(start_win,COLOR_PAIR(COL_START));


    // Testo "Frogger" in grande (ASCII art)
    const char *frogger_scritta[] = {
        "  ███████╗██████╗  ██████╗  ██████   ██████   ███████╗ ██████╗",
        "  ██╔════╝██╔══██╗██╔═══██╗██╔═══╗  ██╔═══╗   ██╔════╝ ██╔══██╗",
        "  █████╗  ██████╔╝██║   ██║██║ ████║██║ ████║ █████╗   ██████╔╝ ",
        "  ██╔══╝  ██╔══██╗██║   ██║██║   ██║██║   ██║ ██╔══╝   ██╔══██╗",
        "  ██║     ██║  ██║╚██████╔╝╚██████╔╝╚██████╔╝ ███████╗ ██║  ██║",
        "  ╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚═════╝  ╚═════╝  ╚══════╝ ╚═╝  ╚═╝"
    };

    // Testo "Premi un tasto per iniziare"
    const char *text = "Premi un tasto per iniziare";

    // Calcola la posizione per "Frogger"
    int y_frogger = (LINES-(LINES/2))/2;  // Posizionato più in alto nella finestra
    int x_frogger = (COLS-(COLS/2))/2;  // Allineato a sinistra

    // Disegna "Frogger" in grande


    wattron(start_win, A_BOLD | COLOR_PAIR(COL_START));  // Testo in grassetto e verde
    for (int i = 0; i < 6; i++) {
        mvwprintw(start_win, y_frogger + i, x_frogger, "%s", frogger_scritta[i]);
    }
    wattroff(start_win, A_BOLD | COLOR_PAIR(COL_START));

    // Posizione per il testo di avvio
    //int y_start = y_frogger + 7;
    //int x_start = (COLS-28) / 2;
    int y_start = y_frogger + 7;
    int x_start = (COLS-(COLS/4)-5) / 2;

    // Disegna il testo di avvio
    wattron(start_win, A_BOLD | COLOR_PAIR(COL_START));  // Testo in grassetto e verde
    mvwprintw(start_win, y_start, x_start, "%s", text);
    wattroff(start_win, A_BOLD | COLOR_PAIR(COL_START));

    // Aggiorna la finestra
    wrefresh(start_win);
   // int ch = getch();
	//if (ch!=ERR)
          //{
          //stop_audio();
         // stopAudio=1;
          //}

    flushinp();
    fflush(stdin);
    // Attende la pressione di un tasto
    wgetch(start_win);
	stop_audio();
    stopAudio=1;
    // Elimina la finestra e passa alla successiva
    delwin(start_win);
    clear();
    refresh();
    endwin();

}


int mostra_schermata_lose()
{
	game_init();//Richiamo i colori
    init_sdl_audio();
    playAudio(loseMelody);
    lose_win = newwin(DIM_Y, DIM_X, generate_win_start().y, generate_win_start().x);
    box(gamewin, 0, 0);
    wbkgd(lose_win,COLOR_PAIR(COL_LOSE));
    int decision;
    // Testo "Frogger" in grande (ASCII art)
    const char *lose_scritta[] = {
    "  ██╗  ██╗  █████╗  ██╗     ██████╗  ███████╗ ██████╗   ██████   ██████╗ ",
    "  ██║  ██║ ██╔══██╗ ██║     ██╔══██╗ ██╔════╝ ██╔══██╗ ██════╝  ██╔═══██╗",
    "  ███████║ ███████║ ██║     ██████╔╝ █████╗   ██████╔╝ ╔█████   ██║   ██║",
    "  ██╔══██║ ██╔══██║ ██║     ██╔═══╝  ██╔══╝   ██╔══██╗ ╚════██╗ ██║   ██║",
    "  ██║  ██║ ██║  ██║ ██║     ██║      ███████╗ ██║  ██║ ██████╔╝  ██████╔╝",
    "  ╚═╝  ╚═╝ ╚═╝  ╚═╝ ╚═╝     ╚═╝      ╚══════╝ ╚═╝  ╚═╝ ╚═════╝   ╚═════╝ "
    };

    // Testo "Premi un tasto per iniziare"
    const char *text = "SE VUOI RICOMINCIARE PREMI s SE NO PREMI q";

    // Calcola la posizione della scritta
    int y_lose = (LINES-(LINES/2))/2;  // Posizionato più in alto nella finestra
    int x_lose = (COLS-(COLS/2))/2;  // Allineato a sinistra

    // Disegna La scritta in grande
    wattron(lose_win, A_BOLD | COLOR_PAIR(COL_LOSE));  // Testo in grassetto e verde
    for (int i = 0; i < 6; i++) {
        mvwprintw(lose_win, y_lose + i, x_lose, "%s", lose_scritta[i]);
    }
    wattroff(lose_win, A_BOLD | COLOR_PAIR(COL_LOSE));


    int y_start = y_lose + 7;
    int x_start = (COLS-(COLS/4)-13) / 2;

    // Disegna il testo di avvio
    wattron(lose_win, A_BOLD | COLOR_PAIR(COL_LOSE));  // Testo in grassetto e verde
    mvwprintw(lose_win, y_start, x_start, "%s", text);
    mvwprintw(lose_win, y_start +5, x_start+11, "Punteggio finale: %d", score);
    wattroff(lose_win, A_BOLD | COLOR_PAIR(COL_LOSE));

    // Aggiorna la finestra
    wrefresh(lose_win);

    // Attende la pressione di un tasto
    decision=wgetch(lose_win);
	stop_audio();
    stopAudio=1;
    // Elimina la finestra e passa alla successiva
    delwin(lose_win);
    clear();
    refresh();
    endwin();
    return decision;
}

int mostra_schermata_win()
{
    game_init();//Richiamo i colori
    init_sdl_audio();
    playAudio(winMelody);
    win_win = newwin(DIM_Y, DIM_X, generate_win_start().y, generate_win_start().x);
    box(gamewin, 0, 0);
    wbkgd(win_win,COLOR_PAIR(COL_WIN));
    int decision;
    // Testo "Frogger" in grande (ASCII art)
    const char *lose_scritta[] = {
        "  ██╗  ██╗  █████╗  ██╗     ██    ██╗ ██╗ ███╗   ██╗ ████████╗  ██████╗ ",
        "  ██║  ██║ ██╔══██╗ ██║     ██    ██║ ██║ ████╗  ██║ ╚══██╔══╝ ██╔═══██╗",
        "  ███████║ ███████║ ██║     ██    ██║ ██║ ██╔██╗ ██║    ██║    ██║   ██║",
        "  ██╔══██║ ██╔══██║ ██║      ██  ██╔╝ ██║ ██║╚██╗██║    ██║    ██║   ██║",
        "  ██║  ██║ ██║  ██║ ██║       ████╔╝  ██║ ██║ ╚████║    ██║     ██████╔╝",
        "  ╚═╝  ╚═╝ ╚═╝  ╚═╝ ╚═╝        ╚══╝   ╚═╝ ╚═╝  ╚═══╝    ╚═╝     ╚═════╝ "
        };

    // Testo "Premi un tasto per iniziare"
    const char *text = "SE VUOI RICOMINCIARE PREMI s SE NO PREMI q";

    // Calcola la posizione della scritta
    int y_lose = (LINES-(LINES/2))/2;  // Posizionato più in alto nella finestra
    int x_lose = (COLS-(COLS/2))/2;  // Allineato a sinistra

    // Disegna La scritta in grande
    wattron(win_win, A_BOLD | COLOR_PAIR(COL_WIN));  // Testo in grassetto e verde
    for (int i = 0; i < 6; i++) {
        mvwprintw(win_win, y_lose + i, x_lose, "%s", lose_scritta[i]);
    }
    wattroff(win_win, A_BOLD | COLOR_PAIR(COL_WIN));


    int y_start = y_lose + 7;
    int x_start = (COLS-(COLS/4)-13) / 2;

    // Disegna il testo di avvio
    wattron(win_win, A_BOLD | COLOR_PAIR(COL_WIN));  // Testo in grassetto e verde
    mvwprintw(win_win, y_start, x_start, "%s", text);
    mvwprintw(lose_win, y_start +5, x_start+11, "Punteggio finale: %d", score);
    wattroff(win_win, A_BOLD | COLOR_PAIR(COL_WIN));

    // Aggiorna la finestra
    wrefresh(win_win);

    // Attende la pressione di un tasto
    decision=wgetch(win_win);
	stop_audio();
    stopAudio=1;
    // Elimina la finestra e passa alla successiva
    delwin(win_win);
    clear();
    refresh();
    endwin();
    return decision;
}
