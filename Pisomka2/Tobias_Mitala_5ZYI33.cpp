#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

using namespace std;

enum typZakanika { NORMALNY = 1, FANUSIK = 2, ZBERATEL = 3};

#define POCET_KUPUJUCICH 8
#define VELKOST_PULTU 8


void zakaznik(int id, vector<double> &databazaHodnotenia, mutex *mut, condition_variable *pridaj, condition_variable *odober,condition_variable *pridi, condition_variable *volnaDb, int typZak, int *pocetLudi, int *pocetCD, bool *volnaDatabaza) {
    printf("Zakaznik %d.: Je vytvorený!\n", id);

    int cesta = 2+rand()%6;
    this_thread::sleep_for(chrono::seconds(cesta));
    printf("Zakaznik %d: Prichadza pred predajnu!\n", id);
    unique_lock<mutex> lock(*mut);
    {
        printf("Zakaznik %d: Vošiel do predajne!\n", id);
        while (*pocetCD < typZak) {
            printf("Zakanik %d: Vošiel do pradajne. Čakám na CD!\n", id);
            odober->wait(lock);
        }
        *pocetCD -= typZak;
        printf("Zakanik %d: Kupil %d singlov! \n", id, typZak);
        printf("Zakanik %d: Vysiel z obchodu \n", id);
        pridaj->notify_one();
        lock.unlock();
    }
    printf("Zakanik %d: Odchádza domov! \n", id);
    this_thread::sleep_for(chrono::seconds(cesta));
    printf("Zakanik %d: Prisiel domov!\n", id);
    while (!volnaDatabaza) {
        printf("Zakanik %d: Databazu niekto pouziva! Čakám!\n", id);
        volnaDb->wait(lock);
    }
    lock.lock();
    printf("Zakanik %d: Vstupujem do DB!\n", id);
    *volnaDatabaza = false;
    double hodnoteniePriprava = rand() / (double) RAND_MAX;
    double hodnotenie = 7 + hodnoteniePriprava * (10-7);
    printf("Zakanik %d: Zadavam hodnotenie %f!\n", id, hodnotenie);
    databazaHodnotenia.push_back(hodnotenie);
    *volnaDatabaza = true;
    volnaDb->notify_all();
    lock.unlock();
    printf("Zakanik %d: Konci robotu!\n", id);

}

void predavajuci(int pocetPiesni, mutex *mut, condition_variable * pridaj, condition_variable * odober, int *pocetCD) {
    printf("Predavajuci: začina fungovať!\n");
    int pocetVylozenychCD = 0;
    int cenaPredaja = 0;
    while (pocetVylozenychCD < pocetPiesni) {
        printf("Predavajuci: idem sa pozried do pultu!\n");
        unique_lock<mutex> lock( *mut);
        while (*pocetCD >= 3) {
            printf("Predavajuci: Na pulte je %d cd, Čakám!\n", *pocetCD);
            pridaj->wait(lock);
        }
        int chyba = 8 - *pocetCD;
        pocetVylozenychCD += chyba;
        *pocetCD += chyba;
        odober->notify_all();
        printf("Predavajuci: vylozil som %d CD v cene %d€!\n", chyba, chyba * 10);
        cenaPredaja += chyba * 10;
    }

    printf("Predavajuci: Vyprazdnil zasoby a konci. Zarobil %d€!\n", cenaPredaja);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int pocetZakaznikov = POCET_KUPUJUCICH;
    if (argc > 1) {
        int hodnota = stoi(argv[1]);
        if (hodnota % 8 != 0) {
            cout << "Zle zadany vstup, cislo nie je delitelne 8, bude použitých 8 zakanikov" << endl;
            pocetZakaznikov = POCET_KUPUJUCICH;
        } else {
            pocetZakaznikov = hodnota;
        }
    }
    cout << "Pocet zakaznikov ktorý budu nakupovať: "<< pocetZakaznikov << endl;
    vector<double> databazaHodnotenia;
    mutex mut;
    condition_variable pridaj, odober, pridi, volnaDB;
    int pocetLudi = 0;
    int pocetCD = 0;
    bool volnaDatabaza = true;



    thread zakaznici[pocetZakaznikov];
    int pocetPiesni = 0;
    for (int i = 0; i < pocetZakaznikov; ++i) {
        int typZakanika;
        if (i%8 <= 3) {
            typZakanika = typZakanika::NORMALNY;
        } else if (i%8 <= 6 ) {
            typZakanika =  typZakanika:: FANUSIK;
        } else {
            typZakanika =  typZakanika::ZBERATEL;
        }
        pocetPiesni += typZakanika;
        zakaznici[i] = thread(zakaznik, i, ref(databazaHodnotenia), &mut, &pridaj, &pridi, &odober, &volnaDB, typZakanika, &pocetLudi, &pocetCD, &volnaDatabaza);
    }
    thread obchod(predavajuci, pocetPiesni, &mut, &pridaj, &odober, &pocetCD);


    printf("Its on! Sock Astdio!\n");
    for (int i = 0; i < pocetZakaznikov; ++i) {
        zakaznici[i].join();
    }
    obchod.join();

    cout << "Never give up!\n";

    double minimum = 10;
    double maximum = 0;
    double mean = 0;
    double sucet = 0;
    for (int i = 0; i < databazaHodnotenia.size(); ++i) {
        sucet += databazaHodnotenia.at(i);
        minimum = min(minimum, databazaHodnotenia.at(i));
        maximum = max(maximum, databazaHodnotenia.at(i));
    }
    mean = sucet / databazaHodnotenia.size();
    printf("Hodnotenie singlu: min: %.2f, max: %.2f, priemer: %.2f\n", minimum, maximum, mean);
    return 0;
}
