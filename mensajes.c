
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <wait.h>

#define CICLOS 10

typedef struct {    // Estructura de los mensajes enviados
	long msg_type;		// Tipo de mensaje
	char mensaje[100];	// Contenido
} MSGTYPE;

int msgqid;

char *pais[3]={"Peru","Bolvia","Colombia"};
int *g;

void emisor()
{
      int i; int l; int k;
      MSGTYPE m;
      for(k=0;k<CICLOS;k++) // Se pone este loop aquí para que sea más fácil alternar entre los países
      {
        for(i=0;i<3;i++)
        {
            printf("Entra %s\n",pais[i]);
            sleep(rand()%3);
            m.msg_type = 1;
            sprintf(m.mensaje, "- %s Sale\n", pais[i]);
            msgsnd(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),0); // Mandamos el mensaje de que ya salió el país
            fflush(stdout);
            // Salida de la sección crítica
            // Espera aleatoria fuera de la sección crítica
            sleep(rand()%3);
        }
      }

      m.msg_type=1;
     	strcpy(m.mensaje,"Fin");
     	msgsnd(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),0);

      exit(0); // Termina el proceso
 }

 void receptor()
 {
       MSGTYPE m;
     	 int retval;
       do
     	{
     		retval=msgrcv(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),1,0); // Recibimos el mensaje
     		printf("\t\tRecibido: %s",m.mensaje);
     	}
     	while(strcmp(m.mensaje,"Fin")!=0); // Hasta que el mensaje sea "Fin"
      printf("\n");
     	return;
}

int main() {
     int pid;
     int status;
     int args[3];
     srand(getpid());

     msgqid=msgget(0x1234,0666|IPC_CREAT); // Creamos la cola de mensajes

    // Crea un nuevo proceso hijo que ejecuta la función emisor()
    pid=fork();
    if(pid==0)
      emisor();

    receptor();

    pid = wait(&status);

    msgctl(msgqid,IPC_RMID,NULL); // Terminamos la cola de mensajes
    return(0);
}
