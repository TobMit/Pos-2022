#include <cstdio>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef enum huba {
    BEDLA, DUBAK, KOZAK, MUCHOTRAVKA
} HUBA ;

typedef struct SpolData {
    HUBA * pult;
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
    int celkovyPocetHub;
    SP * data;
} SUSIC;

#define POCET_HUB 15;

void * hubarF(void * arg) {
    HUBAR * dataH = static_cast<HUBAR *> (arg);
    printf("Hubar %d zacian fungovat\n", dataH->id);
    HUBA huba;
    int tmp;
    for (int i = 0; i < dataH->pocetHub; ++i) {
        tmp = 1*rand() % 100;
        if (tmp <= 25) {
            huba = BEDLA;
        } else if (tmp <= 35) {
            huba = DUBAK;
        } else if (tmp <= 60) {
            huba = KOZAK;
        } else {
            huba = MUCHOTRAVKA;
        }
        printf("Hubar %d zobral som %d hubu %d z auta!\n", dataH->id, (i+1), huba);
        sleep(dataH->casPresu);
        printf("Hubar %d zobral som %d hubu %d a som pred pultom\n", dataH->id, (i+1), huba);
        pthread_mutex_lock(dataH->data->mut);
        while (dataH->data->aktualPult >=dataH->data->maxPult) {
            printf("Hubar %d zobral somje na pulte %d! Cakam!\n", dataH->id, dataH->data->aktualPult);
            pthread_cond_wait(dataH->data->pridaj, dataH->data->mut);
        }
        printf("Hubar %d je na pulte %d! Vkladam hubu %d\n", dataH->id, dataH->data->aktualPult, huba);
        dataH->data->pult[dataH->data->aktualPult++] = huba;
        pthread_cond_signal(dataH->data->odober);
        pthread_mutex_unlock(dataH->data->mut);
        printf("Hubar %d odlosil som hubu %d na pult! Idem do auta!\n", dataH->id, (i+1));
        sleep(dataH->casPresu);
        printf("Hubar %d som pri aute!\n", dataH->id);
    }
    printf("Hubar %d konci fungovat\n", dataH->id);
    return NULL;
}

void * susicF(void * arg) {
    SUSIC * dataS = static_cast<SUSIC *> (arg);
    printf("Susic Mada Sad zacina fungovat\n");
    int pocetSpracovanychHub = 0;
    while ( pocetSpracovanychHub < dataS->celkovyPocetHub) {
        printf("Susic Mada Sad idem pozreit pult!\n");
        pthread_mutex_lock(dataS->data->mut);
        while (dataS->data->aktualPult <= 0) {
            printf("Susic Mada Sad ma %d hub, cakam!\n", dataS->celkovyPocetHub);
            pthread_cond_wait(dataS->data->odober, dataS->data->mut);
        }
        for (int i = 0; i < dataS->data->aktualPult; ++i) {
            printf("Susic Mada Sad napulte %d na pozicii %d!", dataS->data->pult[i],i);
        }
        pocetSpracovanychHub += dataS->data->aktualPult;
        pthread_cond_broadcast(dataS->data->pridaj);
        dataS->data->aktualPult = 0;
        pthread_mutex_unlock(dataS->data->mut);
    }
    printf("Susic Mada Sad konci fungovat\n");
    pthread_exit(NULL);
}


int main (int argc, char ** argv) {
    srand(time(NULL));
    int n = 5;
    if (argc > 1 ) {
        n = atoi(argv[1]);
    }
    int velkostPultu = 5;
    HUBA pult[velkostPultu];
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
