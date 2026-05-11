
FILE_O=File_O

#mettiamo i file oggetto dentro la cartella File_O
DIR_O=$(addprefix $(FILE_O)/, main.o utils.o grafica.o collision.o elementi.o rana.o coccodrillo.o proiettili.o menu.o musica.o)

#File.c utilizzati
MAIN=main.c definizioni.h grafica.h menu.h
GRAFICA=grafica.c grafica.h definizioni.h utils.h menu.h musica.h
UTILS=utils.c definizioni.h utils.h collision.h rana.h coccodrillo.h elementi.h menu.h grafica.h
COLLISION=collision.c collision.h definizioni.h
ELEMENTI=elementi.c elementi.h definizioni.h
RANA=rana.c rana.h definizioni.h menu.h grafica.h
COCCODRILLO=coccodrillo.c coccodrillo.h definizioni.h
PROIETTILI=proiettili.c proiettili.h definizioni.h
MENU=menu.c menu.h definizioni.h grafica.h utils.h musica.h
MUSICA=musica.c musica.h


#librerie usate
NCT=-lncursesw -lm -lpthread
SDL2=$(shell sdl2-config --cflags --libs)
CF=-w $(shell sdl2-config --cflags)

#Colleghiamo i file

output: $(DIR_O)
	gcc $(CF) $(DIR_O) -o output $(NCT) $(SDL2)

$(FILE_O)/main.o: $(MAIN)
	gcc $(CF) -c -o $(FILE_O)/main.o main.c

$(FILE_O)/grafica.o: $(GRAFICA)
	gcc $(CF) -c -o $(FILE_O)/grafica.o grafica.c

$(FILE_O)/utils.o: $(UTILS)
	gcc $(CF) -c -o $(FILE_O)/utils.o utils.c

$(FILE_O)/collision.o: $(COLLISION)
	gcc $(CF) -c -o $(FILE_O)/collision.o collision.c

$(FILE_O)/elementi.o: $(ELEMENTI)
	gcc $(CF) -c -o $(FILE_O)/elementi.o elementi.c

$(FILE_O)/rana.o: $(RANA)
	gcc $(CF) -c -o $(FILE_O)/rana.o rana.c

$(FILE_O)/coccodrillo.o: $(COCCODRILLO)
	gcc $(CF) -c -o $(FILE_O)/coccodrillo.o coccodrillo.c

$(FILE_O)/proiettili.o: $(PROIETTILI)
	gcc $(CF) -c -o $(FILE_O)/proiettili.o proiettili.c

$(FILE_O)/menu.o: $(MENU)
	gcc $(CF) -c -o $(FILE_O)/menu.o menu.c

$(FILE_O)/musica.o: $(MUSICA)
	gcc $(CF) -c -o $(FILE_O)/musica.o musica.c

$(DIR_O): | $(FILE_O)

#Creazione della cartella per i file oggetto
$(FILE_O):
	mkdir -p $(FILE_O)

#Pulizia
clean:
	rm -r $(FILE_O)
	rm output