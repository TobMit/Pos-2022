#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

using namespace std;

enum huby { BEDLA = 0, DUBAK = 1, KOZAK = 2, MUCHOTRAVKA = 3 };

#define POCET_HUBAROV 5
#define POCET_HUB 15
#define VELKOST_PULTU 5

int getHubu() {
    int random = 1 + rand() % 100;
    if (random <= 25) {
        return BEDLA;
    } else if (random <= 35) {
        return DUBAK;
    } else if (random <= 60) {
        return KOZAK;
    } else {
        return MUCHOTRAVKA;
    }
}

int zarobenie(int huba) {
    switch (huba) {
        case huby::DUBAK:
            return 20;
        case huby::BEDLA:
            return 10;
        case huby::KOZAK:
            return 5;
        default:
            return 0;
    }
}

void hubary(int id, int *zarobok, vector<int> &pult, mutex *mut, condition_variable * pridaj, condition_variable * odober) {
    printf("Hubar %d. začina zbieranie!\n", id);
    *zarobok = 0;
    for (int i = 0; i < POCET_HUB; ++i) {
        int huba = getHubu();
        //printf("Hubar %d zobral hubu %d z auta! \n", id, huba);
        this_thread::sleep_for(chrono::milliseconds(1000 + rand() % 2000));
        printf("Hubar %d. som pred pultom z hubov %d! Čakám! \n", id, huba);
        unique_lock<mutex> lock(*mut);
        while (pult.size() >= VELKOST_PULTU) {
            printf("Hubar %d. na pulte je %d hub! Čakám! \n", id, pult.size());
            pridaj->wait(lock);
        }
        printf("Hubar %d. vkladam hubu na pult! \n", id);
        pult.push_back(huba);
        *zarobok += zarobenie(huba);
        odober->notify_one();
        lock.unlock();
        printf("Hubar %d. odlozil som hubu %d na pult! Idem k autu! \n", id, huba);
        this_thread::sleep_for(chrono::milliseconds(1000 + rand() % 2000));
        printf("Hubar %d. som pri aute\n", id);
    }
    printf("Hubar %d konči!\n", id);

}

void susicMada(int maxPocetHub, vector<int> &pult, mutex *mut, condition_variable * pridaj, condition_variable * odober) {
    printf("Susicka Mada Šád začina fungovať!\n");
    int pocetSpracovanyHub = 0;
    int akutualneSpracovane = 0;
    while (pocetSpracovanyHub < maxPocetHub) {
        printf("Susicka Mada Šád: idem sa pozried do pultu!\n");
        unique_lock<mutex> lock( *mut);
        while (pult.empty()) {
            printf("Susicka Mada Šád: pult je prázdny, čakám!\n");
            odober->wait(lock);
        }
        while (!pult.empty()) {
            int huba = pult.back();
            printf("Susicka Mada Šád: spracovávam hubu %d\n", huba);
            pult.pop_back();
            pocetSpracovanyHub++;
            akutualneSpracovane++;
        }
        pridaj->notify_all();
        lock.unlock();
        printf("Susicka Mada Šád: ukladam %d hub do poličiek\n", akutualneSpracovane);
        this_thread::sleep_for(chrono::seconds(akutualneSpracovane*2));
        akutualneSpracovane = 0;
        printf("Susicka Mada Šád: huby sú uložené\n");
    }

    printf("Susicka Mada Šád dosušila!\n");
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int pocetHubarov = POCET_HUBAROV;
    if (argc > 1) {
        pocetHubarov = stoi(argv[1]);
    }
    int zarobok[pocetHubarov];
    vector<int> pult;
    mutex mut;
    condition_variable pridaj, odober;


    thread hubar[pocetHubarov];
    for (int i = 0; i < pocetHubarov; ++i) {
        hubar[i] = thread(hubary, i, &zarobok[i], ref(pult), &mut, &pridaj, &odober);
    }
    thread sucicka(susicMada, pocetHubarov * POCET_HUB,ref(pult), &mut, &pridaj, &odober);


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
/*  int i = 0;
    for (auto item: pult) {
        cout << i++ << ". " << item << endl;
    }*/
    return 0;
}
