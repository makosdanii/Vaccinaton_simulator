#include "pipe.h"

void handler(sig_atomic_t n){
    static int oltobusz = 0;
    if (oltobusz == 0){

        printf("Oltóbusz 1 - HARCRA_FEL\n");
        oltobusz++;
    }
}

void handler2(sig_atomic_t n){
    static int oltobusz2 = 0;
    if (oltobusz2 == 0){

        printf("Oltóbusz 2 - HARCRA_FEL\n");
        oltobusz2++;
    }

}

void testInit(Patients* p){
    Person x, y;
    x.name=(char*)malloc((strlen("juci") + 1)*sizeof(char));
    strcpy(x.name,"juci");
    x.date=(char*)malloc((strlen("20000801") + 1)*sizeof(char));
    strcpy(x.date,"20000801");
    x.phone=(char*)malloc((strlen("06305540075") + 1)*sizeof(char));
    strcpy(x.phone,"06305540075");
    x.paid = true;

    y.name=(char*)malloc((strlen("ceci") + 1)*sizeof(char));
    strcpy(y.name,"ceci");
    y.date=(char*)malloc((strlen("20000801") + 1)*sizeof(char));
    strcpy(y.date,"20000801");
    y.phone=(char*)malloc((strlen("06703151528") + 1)*sizeof(char));
    strcpy(y.phone,"06703151528");
    y.paid = true;


    _add(p, x);
    _add(p, y);
    _add(p, x);
    _add(p, y);
    _add(p, x);
    _add(p, y);
    _add(p, x);
    _add(p, y);
    _add(p, x);
    _add(p, y);
    _add(p, x);
    _add(p, y);
}

int enor(Patients* p){
    int cnt = 0;
    for (int i = 0; i < p->arr_size; i++){
        if(p->head[i].paid) cnt++;
    }

    return cnt > 4 ? (cnt > 9 ? 10 : 5) : 0; //trunc the patients
}

void simulate(Patients* p){
    // Patients p;
    // init(&p, 1);
    // testInit(&p);
    
//PIPEOLAS
    int pipefd[2];
    int pipe2fd[2];
    if(pipe(pipefd) == -1) {perror("pipe creating"); exit(EXIT_FAILURE);}
    if(pipe(pipe2fd) == -1) {perror("pipe creating"); exit(EXIT_FAILURE);}

//SLEEP
    struct timespec ts; 
    ts.tv_sec = 0;
    ts.tv_nsec = 500000000;

//SIGNALOZAS
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler2);

//SEMABEPHOROZAS
    sem_unlink("/consumer");
    sem_unlink("/producer");


    sem_t* prod;
    if (SEM_FAILED == (prod = sem_open("/producer",O_CREAT,S_IRUSR|S_IWUSR,0))){
        perror("semaphore creating");  exit(EXIT_FAILURE);
    } 

    sem_t* cons;
    if (SEM_FAILED == (cons = sem_open("/consumer",O_CREAT,S_IRUSR|S_IWUSR,1))){
        perror("semaphore creating"); exit(EXIT_FAILURE);
    } 

//SHMEMORIZGATAS
    key_t key;
    if ((key = ftok("./shmem.c", 0)) == -1) {perror("key krealas"); exit(EXIT_FAILURE);}
    int shmid;
    if ((shmid = shmget(key, 16, IPC_CREAT|S_IRUSR|S_IWUSR)) == -1) {perror("shmem creating"); exit(EXIT_FAILURE);}
    char* s;
    if ((s = shmat(shmid, NULL, 0)) == NULL) {perror("memoria attach"); exit(EXIT_FAILURE);}


//FORKOLAS KEZDODIK
    pid_t ppid = getpid();
    pid_t pid;
    if ((pid = fork()) == -1){perror("forking"); exit(EXIT_FAILURE);}
    
if (pid == 0){
//GYEREK
        close(pipe2fd[1]);

        sem_wait(prod);
        printf("Operációs törzs ennyi embert oltat be ma: %s\n", s);
        int n = atoi(s);
        shmdt(s);
        sem_post(cons);

        sleep(1);

        //OLTOBUSZ 1
        if (n != 0) {kill(ppid, SIGUSR1);
        }else{
            close(pipefd[0]);
            close(pipefd[1]);

            close(pipe2fd[0]);
            exit(0);
        }

        // sem_wait(cons);
        // sem_post(prod);

        //OLTOBUSZ 2
        if (n == VACC_CAP) {
            if ((pid = fork()) == -1){perror("forking");  exit(EXIT_FAILURE);}
            if ((pid == 0)){
                kill(ppid, SIGUSR2);
            }
        }else{
            close(pipe2fd[0]);
            close(pipe2fd[1]);
        }

        int num;

        struct timespec ts2;
        timespec_get(&ts2, TIME_UTC);
        srand(ts2.tv_nsec);

        char msg[MSG_LENGTH];
        int m;
        char vaccinated[m = (n == VACC_CAP) ? (int)round(n/2) : n][MSG_LENGTH];
        for(int i = 0; (n == VACC_CAP) ? i<round(n/2) : i < n; i++) strcpy(vaccinated[i], "nincs oltva");
        sem_wait(prod);
        //OLTOBUSZ 1
        if (pid == 0){
            sem_post(cons);
            for (int i=0; (n == VACC_CAP) ? i<round(n/2) : i < n; i++){
                if (-1 == read(pipefd[0], msg, MSG_LENGTH)) {perror("pipe reading"); exit(EXIT_FAILURE);}
                printf("Oltóbusz 1: %s\n", msg);

                if((num = rand() % RATIO) != 0) strcpy(vaccinated[i], msg);
            }
            close(pipefd[0]);
            close(pipe2fd[0]);
        //OLTOBUSZ 2
        }else{
            sem_post(cons);
            for (int i=0; i<floor(n/2); i++){
                if (-1 == read(pipe2fd[0], msg, MSG_LENGTH)) {perror("pipe reading"); exit(EXIT_FAILURE);}
                printf("Oltóbusz 2: %s\n", msg);

                if((num = rand() % RATIO) != 0) strcpy(vaccinated[i], msg);
            }

            close(pipefd[0]);
            close(pipe2fd[0]);
        }

        if (pid != 0) sem_post(cons);
        sem_wait(prod);
        printf("\nOltóbusz %i olt...\n", pid==0 ? 1 : 2);

        for (int i=0; i < n; i++){
            nanosleep(&ts, NULL);
            if (-1 == write(pipefd[1], vaccinated[i], MSG_LENGTH)) {perror("pipe writing");  exit(EXIT_FAILURE);}
        }
        close(pipefd[1]);
        sem_close(cons);
        sem_close(prod);
    exit(0);
}else{
//SZULO
        close(pipe2fd[0]);

        sem_wait(cons);
        int n = enor(p);
        sprintf(s, "%d", n);
        shmdt(s);
        shmctl(shmid,IPC_RMID,NULL);
        sem_post(prod);

        nanosleep(&ts, NULL);

        if (n != 0) { 
            pause();
            printf("ELső oltóbusz elindult\n");
        }else{
            printf("Ceci ma nem olt\n");
            close(pipefd[0]);
            close(pipefd[1]);

            close(pipe2fd[1]);
            return;
        }

        // sem_wait(prod);

        if (n == VACC_CAP) { 
            pause();
            printf("Második oltóbusz elindult\n");
        }
        // sem_post(cons);

        sem_wait(cons);
        sleep(1);
        printf("\nBeosztás készítése folyamatban...:\n");

        //elso busz keszenlet
        sem_post(prod);
        

        //masodik busz keszenlet
        if (n == VACC_CAP){
            sem_wait(cons); 
            sem_post(prod);
        }
        
        for(int i = 0; i < n; i++){
            if (p->head[i].paid) { 
                printf("%s - %i\n", p->head[i].name, n == VACC_CAP ? (i%2)+1 : 1); 
                if (-1 == write((n == VACC_CAP && i%2 != 0) ? pipe2fd[1] : pipefd[1],
                    p->head[i].phone, (strlen(p->head[i].phone)+1)*sizeof(char))) { perror("pipe writing"); exit(EXIT_FAILURE);}

            }
            nanosleep(&ts, NULL);
        }
        
        close(pipefd[1]);
        close(pipe2fd[1]);

        //elso, masodik oltashoz kesz
        sem_wait(cons);
        if (n == VACC_CAP) sem_wait(cons);
        sem_post(prod);
        if (n == VACC_CAP)sem_post(prod);

        char msg[MSG_LENGTH];
        
        for(int i = 0; i<n; i++){
            if (-1 == read(pipefd[0], msg, MSG_LENGTH)){perror("pipe reading"); exit(EXIT_FAILURE);}
            printf("Oltott bejegyezve: %s\n", msg);
            if (0 != strcmp(msg, "nincs oltva")) _remove(p, msg);
        }

        close(pipefd[0]);

        sem_close(cons);
        sem_close(prod);
        wait(NULL);
        return;
    }
    perror("zombie apocalypse"); exit(EXIT_FAILURE);
}