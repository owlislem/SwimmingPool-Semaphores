#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#define nc 1 // nombre de cabine
#define np 2 // nombre de panier
#define scab 0 // semaphore cabine
#define Mutex 1 // semaphore mutex
#define span 2 // semaphore span

struct sembuf Pscab = {scab,-1,0};
struct sembuf Vscab = {scab,1,0};
struct sembuf Pmutex = {Mutex,-1,0};
struct sembuf Vmutex = {Mutex,1,0};
struct sembuf Pspan = {span,-1,0};
struct sembuf Vspan = {span,1,0};

int main (int argc, char* pid[] ){
    key_t key = ftok("main.c", 1);
    if (key == -1){
        perror("ftok");
        return 1;
    }
    int semid = semget(key, 3, IPC_CREAT | 0666);
    if (semid == -1){
        perror("semget");
        return 1;
    }
    key_t key2 = ftok("main.c", 2);
    int shmid_ndp = shmget(key2, sizeof(int), IPC_CREAT | 0666);
    if (shmid_ndp == -1 ){
        perror("shmget");
        return 1;
    }
    // attach ndp
    int *ndp = (int*)shmat(shmid_ndp, NULL, 0);
    if (ndp == (void *) -1){
        perror("shmat");
        return 1;
    }
    key_t key3 = ftok("main.c", 3);
    int shmid_ndo = shmget(key3, sizeof(int), IPC_CREAT | 0666);
    if (shmid_ndo == -1 ){
        perror("shmget");
        return 1;
    }
    // attach ndo
    int *ndo = (int*)shmat(shmid_ndo, NULL, 0);
    if (ndo == (void *) -1){
        perror("shmat");
        return 1;
    }
    // initialize ndp and ndo
    *ndp = 0;
    *ndo = 0;
    /////////////////////// najeur program 
    int i = atoi(pid[1]);
    printf("je suis le nageur %d\n",i);
    //demander panier
    int Nump ; // numero de panier
    semop(semid,&Pmutex,1);
    *ndp = *ndp + 1;
    if (*ndo == np){
        semop(semid,&Vmutex,1);
        semop(semid,&Pspan,1);
    } 
    *ndp = *ndp - 1;
    *ndo = *ndo + 1;
    Nump = *ndo;
    semop(semid,&Vmutex,1);
    
    printf("nageur %d a pris la panier %d\n",i,Nump);

    // demander cabine
    semop(semid,&Pscab,1);
    // se change et se ranger son vetement dans le panier , wait 3 seconds
    printf("nageur %d: se change et se ranger son vetement dans le panier\n",i);
    sleep(3);
    printf("3 seconds \n");   
   
    // libérer cabine
    semop(semid,&Vscab,1);
    // nager
    // print am naguer Pid and i have panier Nump
    printf("nageur %d: j'onage avec le panier %d\n",i,Nump);
    // wait 5 seconds
    sleep(5);
    printf("5 seconds \n");
    // demander cabine
    semop(semid,&Pscab,1);
    // se change et se ranger son vetement dans le panier , wait 3 seconds
    printf("nageur %d: se change et se ranger son vetement dans le panier\n",i);
    sleep(3);
    printf("3 seconds \n");
    // libérer cabine
    semop(semid,&Vscab,1);
    // libérer panier
    semop(semid,&Pmutex,1);
    *ndo = *ndo - 1;
    Nump = *ndo;
    if  (*ndp > 0){
        printf("nageur %d a libéré le panier et il y %d demmande en attente \n",i,*ndp);
        semop(semid,&Vspan,1);
    }else{
        semop(semid,&Vmutex,1);
    }
    printf(" je suis le nageur %d j ai libéré un panier et il reste %d panier libres\n",i,np-Nump);
    // print am naguer Pid et jai libéré le panier et il reste Nump panier
    // fin nageur program
}
