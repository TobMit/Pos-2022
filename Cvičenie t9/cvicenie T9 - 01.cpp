#include <iostream>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

//takto musí byť deklarovaná premenná
void * generujVelkePismena(void * data) {
    int pocet = *(int *)data; // toto musí byť keď chcem referovať na parametre
    char znak = 'a';
    for (int i = 0; i < pocet; ++i) {
        znak = 'A' +  rand() % ('Z' - 'A' + 1); // to plus 1 tam musí byť aby nám to generovalo aj Z
        cout << i << ". znak je " << znak << endl;
    }


    cout << "Vlakno generujúce znaky konči činnosť" << endl;
    // toto musi byť na konci kodu
    pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
    srand(time(NULL));
    int pocet = 10;
    pthread_t vlakno; // vyrobil som si premmenu pre vlakno
    pthread_create(&vlakno, NULL, generujVelkePismena, (void *)&pocet); // pozor je tam smerník na vlákno, musím to pretipovať na void *
    // toto vlákno sa mohle vytvoriť v tomto bode ale nemuselo, nie je to garantované, závisí od systému

    for (int i = 0; i < 5; ++i) {// toto bude zobrazovať hlavné vlákno
        cout << "Hello world" << endl;
    }

    pthread_join(vlakno, NULL);
    cout << "Hlavne vlakno konči činnosť" << endl;
    return 0;
}

