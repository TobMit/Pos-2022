#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

#define VELKOST_BUFFERA 10
using namespace  std;

void usporiadaj(pair<int, int> *dvojica) {
    if (dvojica->first < dvojica->second) {
        int tmp = dvojica->first;
        dvojica->first = dvojica->second;
        dvojica->second = tmp;
    }
}

void vypocitajKombinacneCislo(int n, vector<pair<int, int>> &buffer, mutex *mut, condition_variable * generuj, condition_variable * nacitavaj) {
    printf("Hlavne vlakno: Začina robotu!\n");
    for (int i = 0; i < n; ++i) {
        unique_lock<mutex> lock (*mut);
        printf("Hlavne vlakno: Idem sa pozreiť do buffera!\n");
        while (buffer.empty()) {
            printf("Hlavne vlakno: Buffer je prázdy čakám!\n");
            nacitavaj->wait(lock);
        }
        pair<int, int> cislo = buffer.back();
        buffer.pop_back();
        generuj->notify_all();
        printf("Hlavne vlakn: Z bufera som vybral dvojicu %d %d\n", cislo.first, cislo.second);
        usporiadaj(&cislo);
        long vrch = 1;
        for (int j = cislo.first; j > cislo.first-cislo.second ; j--) {
            vrch *= j;
        }
        long spodok = 1;
        for (int j = cislo.second; j > 0 ; j--) {
            spodok *= j;
        }
        long kombinacneCislo = vrch / spodok;
        printf("Hlavne vlakn: Kombinačné čislo: (%d %d) = %d\n", cislo.first, cislo.second, kombinacneCislo);
        //lock.unlock();
    }
}

void generujeCislo (int a, int b, int n, vector<pair<int, int>> &buffer, mutex *mut, condition_variable * generuj, condition_variable * spracuj) {
    printf("Generator: Vedlajsie vlakno zacina pracovat! \n");
    for (int i = 0; i < n; ++i) {
        printf("Generator: Idem sa pozried do buffera!\n");
        unique_lock<mutex> lock (*mut);
        while (buffer.size() >= VELKOST_BUFFERA) {
            printf("Generator: Buffer je plny čakám!\n");
            generuj->wait(lock);
        }
        printf("Generator: Začinam generovať\n");
        int randomHodnota1 = a + rand() % b;
        int randomHodnota2 = a + rand() % b;
        pair<int, int> tmp;
        tmp.first = randomHodnota1;
        tmp.second = randomHodnota2;
        buffer.push_back(tmp);
        printf("Generator: Vlozil som do buffera dvojicu %d %d\n", buffer.back().first, buffer.back().second);
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
    vector<pair<int, int>> buffer;
    thread vedlajsieVlakno(generujeCislo, a, b, n, ref(buffer), &mut ,&generuj, &spracuj);
    thread hlavneVlakno(vypocitajKombinacneCislo,n, ref(buffer), &mut ,&generuj, &spracuj);

    hlavneVlakno.join();
    vedlajsieVlakno.join();

    cout << "Koniec programu!" << endl;
    return 0;
}