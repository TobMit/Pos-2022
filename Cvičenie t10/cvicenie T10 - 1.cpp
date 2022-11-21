#include <cstdio>
#include <stdlib.h>
#include <pthread.h>

typedef enum huba {
    BEDLA = 1, DUBAK = 2, KOZAK = 3, MUCHOTRAVKA = 4
} HUBA ;

typedef struct SpolData {
    int * pult;
    int maxPult;
    int aktualPult;
    pthread_mutex_t * mut;
    pthread_cond_t * odober;
    pthread_cond_t * pridaj;
} SP;

typedef struct hubar {
    int id;
    int casPresu;
    int pocetHub;
    SP * data;
} HUBAR;

typedef struct susic {
    int celkovy;
    SP * data;
} SUSIC;

#define POCET_HUB 15;

void * hubarF(void * arg) {
    HUBAR * dataH = static_cast<HUBAR *> (arg);
    printf("Hubar %d zacian fungovat\n", dataH->id);
    HUBA huba;
    for (int i = 0; i < dataH->pocetHub; ++i) {
        //huba = (HUBA)rand() % 4;
        //printf("Hubar %d zobral som %d hubu %d z auta!\n", dataH->id, (i+1), huba);
        printf("Hubar %d zobral som %d hubu z auta!\n", dataH->id, (i+1));
    }
    printf("Hubar %d konci fungovat\n", dataH->id);
    return NULL;
}

void * susicF(void * arg) {
    HUBAR * dataH = static_cast<HUBAR *> (arg);
    printf("Susic Mada Sad zacina fungovat\n");
    for (int i = 0; i < dataH->pocetHub; ++i) {
        //
    }
    printf("Susic Mada Sad konci fungovat\n", dataH->id);
    pthread_exit(NULL);
}


int main (int argc, char ** argv) {
    srand(time(NULL));
    int n = 5;
    if (argc > 1 ) {
        n = atoi(argv[1]);
    }
    int velkostPultu = 5;
    int pult[velkostPultu];
    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t pridaj = PTHREAD_COND_INITIALIZER, odeber = PTHREAD_COND_INITIALIZER;
    SP spolData = { pult, velkostPultu,0,&mut, &odeber, &pridaj};
    int pocetHub = 15;
    pthread_t hubari[n];
    HUBAR hubarD[n];
    pthread_t susic;
    SUSIC susicD = {pocetHub*n, &spolData};
    for (int i = 0; i < n; ++i) {
        hubarD[i].id=i+1;
        hubarD[i].casPresu = i + rand()%2;
        hubarD[i].pocetHub = pocetHub;
        pthread_create(&hubari[i], NULL, hubarF, &hubarD[i]);
    }
    pthread_create(&susic, NULL, susicF, &susicD);
    printf("Cas Susenia nasta, hor sa na to\n");
    for (int i = 0; i < n; ++i) {
        pthread_join(hubari[i], NULL);
    }
    pthread_join(susic, NULL);
    printf("Je nasusene!");
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&pridaj);
    pthread_cond_destroy(&odeber);
    return 0;
}
