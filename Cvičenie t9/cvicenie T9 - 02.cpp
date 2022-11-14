#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

typedef struct spolData {
    int pocet;
    char * poleZnakov;
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
        dataG->data->poleZnakov[i] = znak;
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
        znak = dataS->data->poleZnakov[i];
        cout << i << ". znak je " << znak << endl;
    }


    cout << "Vlakno sifrujúce znaky konči činnosť" << endl;
    // toto musi byť na konci kodu
    pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
    srand(time(NULL));
    int pocet = 10;
    //char buffer[pocet]; //statické generovanie dát

    char * pole = static_cast<char *>( malloc(pocet*sizeof(char)));   // na koniec treba dať free

    SPOL spData = {pocet, pole, 'A', 'Z' };
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
    free(pole);
    return 0;
}

