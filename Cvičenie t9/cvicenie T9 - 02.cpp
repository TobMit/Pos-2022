#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

typedef struct spolData {
    int pocet;
    char * poleZnakov;
    int velkostBuffera;
    int obsadenie;
    pthread_mutex_t *mutexGS;
    pthread_cond_t *generuj;
    pthread_cond_t *sifruj;
    char min;
    char max;
} SPOL;

typedef struct dataGen {
    SPOL * data;
} GEN;

typedef struct dataSif {
    SPOL * data;
    int  posun;
} SIF;

//takto musí byť deklarovaná premenná
void * generujVelkePismena(void * data) {
    GEN * dataG = static_cast<GEN *> (data);
    char znak = 'a';
    cout << "Vlakno generujúce znaky začína činnosť" << endl;
    for (int i = 0; i < dataG->data->pocet; ++i) {
        znak = 'A' +  rand() % (dataG->data->min - dataG->data->max + 1); // to plus 1 tam musí byť aby nám to generovalo aj Z
        pthread_mutex_lock(dataG->data->mutexGS); // krytická sekcia musi byť čo najmenšia aby nezabíjala paralelizmus
        if (dataG->data->obsadenie >= dataG->data->velkostBuffera) {
            pthread_cond_wait(dataG->data->generuj,dataG->data->mutexGS); // čakám kým nepríde správa na generuj
        }
        dataG->data->poleZnakov[dataG->data->obsadenie++] = znak;
        pthread_cond_signal(dataG->data->sifruj);
        pthread_mutex_unlock(dataG->data->mutexGS); // krytická sekcia musi byť čo najmenšia aby nezabíjala paralelizmus
        cout << i << ". znak je " << znak << endl;
    }


    cout << "Vlakno generujúce znaky konči činnosť" << endl;
    // toto musi byť na konci kodu
    pthread_exit(NULL);
}

void * sifrujVelkePismena(void * data) {
    SIF * dataS = static_cast<SIF *> (data);
    char znak = 'a';
    cout << "Vlakno sifrujúce znaky začína činnosť" << endl;
    for (int i = 0; i < dataS->data->pocet; ++i) {
        pthread_mutex_lock(dataS->data->mutexGS);
        if (dataS->data->obsadenie <= 0) {
            pthread_cond_wait(dataS->data->sifruj,dataS->data->mutexGS); // čakám kým nepríde správa na generuj
        }
        znak = dataS->data->poleZnakov[dataS->data->obsadenie--];
        pthread_cond_signal(dataS->data->generuj);
        pthread_mutex_unlock(dataS->data->mutexGS);
        cout << i << ". znak je po sifrovani " << znak << endl;
    }


    cout << "Vlakno sifrujúce znaky konči činnosť" << endl;
    // toto musi byť na konci kodu
    pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
    srand(time(NULL));
    int pocet = 10;
    const int buffPocet = 5;
    //char buffer[pocet]; //statické generovanie dát

    pthread_cond_t generuj, sifruj = PTHREAD_COND_INITIALIZER; // cez makro inicializovane
    pthread_cond_init(&generuj, NULL);

    pthread_mutex_t  mutGS;
    pthread_mutex_init(&mutGS, NULL); // keď som da init tak na konci musím dať destroy

    //char * pole = static_cast<char *>( malloc(pocet*sizeof(char)));   // na koniec treba dať free
    char * buffer = static_cast<char *>( malloc(buffPocet*sizeof(char)));   // vytvárame buffer

    SPOL spData = {pocet, buffer, buffPocet, 0,&mutGS, &generuj, &sifruj,'A', 'Z' };
    GEN genData = {&spData};

    SIF sifData;
    sifData.data = &spData;
    sifData.posun = 3;

    pthread_t vlaknoG, vlaknoS; // vyrobil som si premmenu pre vlakno
    pthread_create(&vlaknoG, NULL, generujVelkePismena, &genData); // pozor je tam smerník na vlákno, musím to pretipovať na void *
    // toto vlákno sa mohle vytvoriť v tomto bode ale nemuselo, nie je to garantované, závisí od systému

    pthread_create(&vlaknoS, NULL, sifrujVelkePismena, &sifData); // pozor je tam smerník na vlákno, musím to pretipovať na void *


    //pthread_detach(vlaknoG);// keď pužijeme detach môže byť zrada, nevieme ako skončilo, ale neriešime jeho životnosť
    sleep(2);
    for (int i = 0; i < 5; ++i) {// toto bude zobrazovať hlavné vlákno
        cout << "Hello world" << endl;
    }


    pthread_join(vlaknoG, NULL);
    pthread_join(vlaknoS, NULL);
    cout << "Hlavne vlakno konči činnosť" << endl;
    pthread_mutex_destroy(&mutGS);
    pthread_cond_destroy(&sifruj);
    pthread_cond_destroy(&generuj);
    free(buffer);
    return 0;
}

