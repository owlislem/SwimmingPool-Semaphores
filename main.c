#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#define nc 1
#define scab 0
#define Mutex 1
#define span 2

int main (){
    // insial key
    printf("1\n");
    key_t key = ftok("main.c", 1);
    if (key == -1){
        perror("ftok");
        return 1;
    }
    // init semaphores
    int semid = semget(key, 3, IPC_CREAT | 0666);
    if (semid == -1){
        perror("semget");
        return 1;
    }
    // set value of scab & Mutex & span
    if(semctl(semid,scab,SETVAL,nc) == -1){
        perror("semctl SETVAL scab a 1");
        return 1;
    }
    if(semctl(semid,Mutex,SETVAL,1) == -1){
        perror("semctl SETVAL Mutex a 1");
        return 1;
    }
    if(semctl(semid,span,SETVAL,0) == -1){
        perror("semctl SETVAL span a 0");
        return 1;
    }
    // init ndp
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
    // init ndo
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


    // init ndp and ndo
    *ndp = 0;
    *ndo = 0;

    //detach ndp and ndo    
    shmdt(ndp);
    shmdt(ndo);

    // creation du processus fils 
    int i, err,p;
    char *pid;
    for (i = 0; i < 3;i++)
    {
        p = fork();
        
        if (p == -1) {
            perror("fork");
            return 1;
        }

        if (p == 0) {
            sprintf(pid,"%d",i);
            err = execlp("./nag", "nag",pid, NULL);
            printf("err = %d\n",err);

            return 1;
        }
    }
    // wait for all fils
    p = wait(NULL);
    while (p != -1){
        p = wait(NULL);
    }
    // destruction du ndp and ndo
    if (shmctl(shmid_ndp, IPC_RMID, NULL) == -1) {
        perror("shmctl IPC_RMID");
        return 1;
    }
    if (shmctl(shmid_ndo, IPC_RMID, NULL) == -1) {
        perror("shmctl IPC_RMID");
        return 1;
    }

    // destruction du semaphores
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID");
        return 1;
    }


     




    
}
