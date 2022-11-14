#include <iostream>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

//takto musí byť deklarovaná premenná
void * generujVelkePismena(void * data) {
    char znak = 'a';
    srand(time(NULL));
    for (int i = 0; i < 10; ++i) {
        znak = 'A' +  rand() % ('Z' - 'A' + 1); // to plus 1 tam musí byť aby nám to generovalo aj Z
        cout << i << ". znak je " << znak << endl;
    }


    cout << "Vlakno generujúce znaky konči činnosť" << endl;
    // toto musi byť na konci kodu
    pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
    pthread_t vlakno; // vyrobil som si premmenu pre vlakno
    pthread_create(&vlakno, NULL, generujVelkePismena, NULL); // pozor je tam smerník na vlákno
    // toto vlákno sa mohle vytvoriť v tomto bode ale nemuselo, nie je to garantované, závisí od systému

    for (int i = 0; i < 5; ++i) {// toto bude zobrazovať hlavné vlákno
        cout << "Hello world" << endl;
    }

    pthread_join(vlakno, NULL);
    cout << "Hlavne vlakno konči činnosť" << endl;
    return 0;
}

