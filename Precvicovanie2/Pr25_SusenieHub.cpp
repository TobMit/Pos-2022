#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <unistd.h>

using namespace std;

void hubary(int id, int *zarobok) {
    printf("Hubar %d začina zbieranie!\n", id);
    this_thread::sleep_for(chrono::seconds(1 + rand()%4));
    *zarobok = id;
    printf("Hubar %d konči zbieranie!\n", id);

}

void susicMada() {

}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int pocetHubarov = 5;
    if (argc > 1) {
        pocetHubarov = stoi(argv[1]);
    }
    int zarobok[pocetHubarov];
    thread hubar[pocetHubarov];
    for (int i = 0; i < pocetHubarov; ++i) {
        hubar[i] = thread(hubary, i, &zarobok[i]);
    }
    thread sucicka(susicMada);


    printf("Čas sušenia nastal, hor sa na to!\n");
    //sleep(1);
    for (int i = 0; i < pocetHubarov; ++i) {
        hubar[i].join();
    }
    sucicka.join();

    cout << "Je nasušené! Paráda!\n";
    int celkovyZarobok = 0;
    for (int i = 0; i < pocetHubarov; ++i) {
        celkovyZarobok += zarobok[i];
    }
    cout << "Celkový zárobok je " << celkovyZarobok << "€" << endl;
    for (int i = 0; i < pocetHubarov; ++i) {
        cout << "Hubar " << i << ". zarobil " << zarobok[i] << "€ a podiel "<< zarobok[i]*100/celkovyZarobok << "%" << endl;
    }
    return 0;
}
