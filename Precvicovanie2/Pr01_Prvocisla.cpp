#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

#define VELKOST_BUFFERA 10
using namespace  std;

void rozkladNaPrimeNumber(int n, vector<int> &buffer, mutex *mut, condition_variable * generuj, condition_variable * nacitavaj) {
    printf("Hlavne vlakno: Začina robotu!\n");
    for (int i = 0; i < n; ++i) {
        unique_lock<mutex> lock (*mut);
        printf("Hlavne vlakno: Idem sa pozreiť do buffera!\n");
        while (buffer.empty()) {
            printf("Hlavne vlakno: Buffer je prázdy čakám!\n");
            nacitavaj->wait(lock);
        }
        int cislo = buffer.back();
        buffer.pop_back();
        generuj->notify_all();
        //lock.unlock();
        printf("Hlavne vlakno: Z bufera som vybral cislo %d\n", cislo);
        printf("Hlavne vlakno: Zacinam rozklad cisla %d\n", cislo);
        int p = 2;
        printf("Hlavne vlakno: %d = ", cislo);
        do {
            if ( cislo % p == 0) {
                printf(" %d *", p);
                cislo /= p;
            } else {
                p += 1;
            }
        } while (cislo > p * p);
        printf(" %d\n", cislo);
    }
}

void generujeCislo (int a, int b, int n, vector<int> &buffer, mutex *mut, condition_variable * generuj, condition_variable * spracuj) {
    printf("Generator: Vedlajsie vlakno zacina pracovat! \n");
    for (int i = 0; i < n; ++i) {
        printf("Generator: Idem sa pozried do buffera!\n");
        unique_lock<mutex> lock (*mut);
        while (buffer.size() >= VELKOST_BUFFERA) {
            printf("Generator: Buffer je plny čakám!\n");
            generuj->wait(lock);
        }
        printf("Generator: Začinam generovať\n");
        int randomHodnota = a + rand() % b;
        buffer.push_back(randomHodnota);
        printf("Generator: Vlozil som do buffera cislo %d \n", randomHodnota);
        //lock.unlock();
        spracuj->notify_all();
        printf("Generator: Dokončil som vkladanie\n");
    }
    printf("Generator: Vedlajsie cislo konci pracovat! \n");
}


int main (int argc, char *argv[]) {
    int a, b ,n;
    mutex mut;
    condition_variable generuj, spracuj;
    srand(time(NULL));
    if  (argc < 4) {
        cerr << "Zlé parametre" << endl;
        return -1;
    } else {
        a = stoi(argv[1]);
        b = stoi(argv[2]);
        n = stoi(argv[3]);
    }
    vector<int> buffer;
    thread vedlajsieVlakno(generujeCislo, a, b, n, ref(buffer), &mut ,&generuj, &spracuj);
    thread hlavneVlakno(rozkladNaPrimeNumber,n, ref(buffer), &mut ,&generuj, &spracuj);

    hlavneVlakno.join();
    vedlajsieVlakno.join();

    cout << "Koniec programu!" << endl;
    return 0;
}