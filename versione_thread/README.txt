Progetto di SOPR (versione thread) a cura di:
-Contu Mauro (60/79/00115)
-Caruso Marco (60/79/00107)

Installare:
-ncurses -> sudo apt install libncurses5-dev
-SDL2 -> sudo apt install libsdl2-dev

Compilare con make

Avviare con ./output

Una volta eseguito il programma, comparirà la schermata iniziale. Dopo aver dato un qualsiasi input, verrà aperto il menu.


Menu:
    Presenta 4 opzioni cliccabili con INVIO:
    1-Scegli difficoltà -> da qua possiamo iniziare una run con la difficoltà che si preferisce.
    2-Comandi -> qua è presente una breve descrizione dei comandi per muovere la rana.
    3-Regole -> qua è presente una descrizione di come funziona il gioco.
    4-Crediti -> qua sono presenti i crediti del progetto.

Musica:
    Sono presenti le musiche per la schemata iniziale, per il menù, per la schermata di vittoria e per la schermata di sconfitta.

Difficoltà:
    La variazione di difficoltà consiste nella velocità dei coccodrilli e dei loro proiettili.
    Più aumenta la difficoltà, più questi sono veloci.
    In base alla difficoltà, avremo incrementi e decrementi di punteggi differenti.

Gestione del punteggio:
Si parte da una base di 200 punti.
-Modalità facile:
    Perdita di una vita: -10 (-15, -20)
    Sconfitta (si perdono tutte e 5 le vite): -100
    Vittoria (si chiudono tutte e 5 le tane): +1000
    Chiusura di una tana (manche vincente): +100
    Neutralizzazione di un proiettile del coccodrillo (collisione proiettile rana/proiettile coccodrillo): +5
    -1 ogni 6 secondi passati (circa)

-Modalità media:
    Perdita di una vita: -15
    Sconfitta (si perdono tutte e 5 le vite): -250
    Vittoria (si chiudono tutte e 5 le tane): +1500
    Chiusura di una tana (manche vincente): +250
    Neutralizzazione di un proiettile del coccodrillo (collisione proiettile rana/proiettile coccodrillo): +5
    -1 ogni 6 secondi passati (circa)

-Modalità difficile:
    Perdita di una vita: -20
    Sconfitta (si perdono tutte e 5 le vite): -500
    Vittoria (si chiudono tutte e 5 le tane): +2000
    Chiusura di una tana (manche vincente): +500
    Neutralizzazione di un proiettile del coccodrillo (collisione proiettile rana/proiettile coccodrillo): +5
    -1 ogni 6 secondi passati (circa)

