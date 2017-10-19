#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h> 

void semwait(int semid)
{
	struct sembuf s;
	s.sem_num=0; // Primero y único elemento del semáforo
	s.sem_op=-1;
	s.sem_flg=SEM_UNDO;
	
	semop(semid,&s,1);
	return;
}
	
void semsignal(int semid)
{
	struct sembuf s;
	s.sem_num=0; // Primero y único elemento del semáforo
	s.sem_op=1;
	s.sem_flg=SEM_UNDO;
	
	semop(semid,&s,1);
	return;
}

int createsem(int key, int value)
{
	int semid;
	semid=semget(key,1,0666|IPC_CREAT); // Solicitar un arreglo de semáforos al SO
	semctl(semid,0,SETVAL,value);	// Inicializar el semáforo en 1
	return(semid);
}

void erasesem(int semid)
{
	semctl(semid,0,IPC_RMID,0);	// Eliminar el arreglo de semaforos
	return;
}
