
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <wait.h>

#define CICLOS 10

typedef struct
{					   // Estructura de los mensajes enviados
	long msg_type;	 // Tipo de mensaje
	char mensaje[100]; // Contenido
} MSGTYPE;

int msgqid;

char *pais[3] = {"Peru", "Bolvia", "Colombia"};
int *g;

void proceso(int i)
{

	int k;
	int l;
	int retval;
	MSGTYPE m;

	for (k = 0; k < CICLOS; k++)
	{
		retval = msgrcv(msgqid, &m, sizeof(MSGTYPE) - sizeof(long), 1, 0);
		printf("+ Entra %s ", pais[i]);
		fflush(stdout);
		sleep(rand() % 3);
		printf("- %s Sale\n", pais[i]);

		m.msg_type = 1;
		sprintf(m.mensaje, "+ Mensaje %d\n", i);
		msgsnd(msgqid, &m, sizeof(MSGTYPE) - sizeof(long), 0); // Mandamos el mensaje de que ya salió el país

		// Salida de la sección crítica
		// Espera aleatoria fuera de la sección crítica
		sleep(rand() % 3);
	}

	exit(0);
}

int main()
{
	int pid;
	int i;
	int status;
	int args[3];
	srand(getpid());

	msgqid = msgget(0x1234, 0666 | IPC_CREAT); // Creamos la cola de mensajes
	MSGTYPE m;
	m.msg_type = 1;
	sprintf(m.mensaje, "Iniciando proceso de paso de trenes %d", i);
	msgsnd(msgqid, &m, sizeof(MSGTYPE) - sizeof(long), 0);

	for (i = 0; i < 3; i++)
	{
		pid = fork();
		if (pid == 0)
			proceso(i);
	}
	for (i = 0; i < 3; i++)
	{
		pid = wait(&status);
	}

	msgctl(msgqid, IPC_RMID, NULL); // Terminamos la cola de mensajes
	return (0);
}
