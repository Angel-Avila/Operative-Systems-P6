all:	main.c
		gcc -o main main.c
		gcc -o mensajes mensajes.c

clean:
		rm main *.o
		rm mensajes *.o
