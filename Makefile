#GRR20197153 Arthur Henrique Canello Vilar

CFLAGS = -Wall -g

EXECUTAVEIS = wavinfo wavvol wavnorm wavrev wavecho wavwide wavcat wavmix

# regra default 
all: $(EXECUTAVEIS)

# regras de ligação
wavinfo: wavinfo.o wav.o
wavvol: wavvol.o wav.o
wavnorm: wavnorm.o wav.o
wavrev: wavrev.o wav.o
wavecho: wavecho.o wav.o
wavwide: wavwide.o wav.o
wavcat: wavcat.o wav.o
wavmix: wavmix.o wav.o

# regras de compilação
wavinfo.o: wavinfo.c wav.h
wavvol.o: wavvol.c wav.h
wavnorm.o: wavnorm.c wav.h
wavrev.o: wavrev.c wav.h
wavecho.o: wavecho.c wav.h
wavwide.o: wavwide.c wav.h
wavcat.o: wavcat.c wav.h
wavmix.o: wavmix.c wav.h
wav.o: wav.c wav.h 

# remove arquivos temporários
clean:
	-rm -f *~ *.o

# remove tudo que não é o código-fonte original
purge: clean
	-rm -f $(EXECUTAVEIS)