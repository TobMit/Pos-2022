#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

using namespace std;

enum ovocie { JABLKA = 1, HRUSKY = 2, SLIVKY = 3, BROSKYNE = 4 };

#define POCET_DODAVATERLOV 6
//#define POCET_HUB 15
#define SKLAD_SUSIARNE 15

int cestaZoSkladu( ovocie typOvocia) {
    switch (typOvocia) {
        case ovocie::JABLKA:
            return (1000+rand()%2000) *  3;
        case ovocie::HRUSKY:
            return (1000+rand()%2000) *  2;
        case ovocie::SLIVKY:
            return (1000+rand()%2000) *  1;
        default:
            return (1000+rand()%2000) *  3;
    }
}

ovocie getTypOvocia() {
    int random = 1 + rand() % 100;
    if (random <= 40) {
        return ovocie::JABLKA;
    } else if (random <= 65) {
        return ovocie::HRUSKY;
    } else if (random <= 75) {
        return ovocie::SLIVKY;
    } else {
        return ovocie::BROSKYNE;
    }
}

int getVaha(ovocie typOvocia) {
    switch (typOvocia) {
        case ovocie::JABLKA:
            return 800+rand()%1200;
        case ovocie::HRUSKY:
            return 300+rand()%500;
        case ovocie::SLIVKY:
            return 600+rand()%1200;
        default:
            return 100+rand()%600;
    }
}

void dodavatel(int id, vector<int> &skladSusiarne, mutex *mut, condition_variable *pridaj, condition_variable *odober, int velkostSkladu, ovocie typOvocia) {
    printf("Dodavatel %d. začina dodavat!\n", id);

    for (int i = 0; i < velkostSkladu; ++i) {
        //printf("Hubar %d zobral hubu %d z auta! \n", id, huba);
        this_thread::sleep_for(chrono::milliseconds(cestaZoSkladu(typOvocia)));
        printf("Dodavatel %d: som pred pultom z hubov %d! Čakám! \n", id, typOvocia);
        {
            unique_lock<mutex> lock(*mut);
            while (skladSusiarne.size() >= SKLAD_SUSIARNE) {
                printf("Dodavatel %d: na sklade je %d bedniciek! Čakám! \n", id, skladSusiarne.size());
                pridaj->wait(lock);
            }
            printf("Dodavatel %d: vkladam bednicku do skladu susiarne! \n", id);
            skladSusiarne.push_back(typOvocia);
            printf("Dodagvatel %d: odlozil som bednicku do skladu sisicky! Idem k k svojmu skladu! \n", id);
            odober->notify_one();
            //lock.unlock();
        }
        this_thread::sleep_for(chrono::milliseconds(1000 + rand() % 2000));
        printf("Dodavatel %d: som pri aute\n", id);
    }
    printf("Dodavatel %d: končil dodavanie!\n", id);

}

void susicLachimAnder(int pocetPrepraviek, vector<int> &pult, mutex *mut, condition_variable * pridaj, condition_variable * odober, vector<int> &vahaOvocia) {
    printf("Susicka Lachim Ander: začina fungovať!\n");
    int pocetSpracovanyHub = 0;
    int akutualneSpracovane = 0;
    while (pocetSpracovanyHub < pocetPrepraviek) {
        printf("Susicka Lachim Ander: idem sa pozried do pultu!\n");
        unique_lock<mutex> lock( *mut);
        while (pult.empty()) {
            printf("Susicka Lachim Ander: pult je prázdny, čakám!\n");
            odober->wait(lock);
        }
        while (!pult.empty()) {
            int huba = pult.back();
            printf("Susicka Lachim Ander: spracovávam ovocie %d\n", huba);
            pult.pop_back();
            pocetSpracovanyHub++;
            akutualneSpracovane++;
        }
        pridaj->notify_all();
        //lock.unlock();
        printf("Susicka Lachim Ander: susim ovocie", akutualneSpracovane);
        this_thread::sleep_for(chrono::seconds(akutualneSpracovane*2));
        akutualneSpracovane = 0;
        printf("Susicka Lachim Ander: ovocie je ususene!\n");
    }

    printf("Susicka Mada Šád dosušila!\n");
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int pocetDodavatelov = POCET_DODAVATERLOV;
    if (argc > 1) {
        pocetDodavatelov = stoi(argv[1]);
    }
    vector<int> vahaOvocia;
    vector<int> skadSusicky;
    mutex mut;
    condition_variable pridaj, odober;


    thread hubar[pocetDodavatelov];
    int pocetPrepraviek = 0;
    for (int i = 0; i < pocetDodavatelov; ++i) {
        int velkostSkladu;
        printf("Zadajte veľkosť skladu: ");
        scanf("%d", &velkostSkladu);
        pocetPrepraviek += velkostSkladu;
        hubar[i] = thread(dodavatel, i, ref(skadSusicky), &mut, &pridaj, &odober, velkostSkladu, getTypOvocia());
    }
    thread sucicka(susicLachimAnder, pocetPrepraviek, ref(skadSusicky), &mut, &pridaj, &odober, ref(vahaOvocia));


    printf("Hey Apple!\n");
    //sleep(1);
    for (int i = 0; i < pocetDodavatelov; ++i) {
        hubar[i].join();
    }
    sucicka.join();

    cout << "Yummy! Yummy!\n";
    int celkovaVaha = 0;
    for (int i = 0; i < vahaOvocia.size(); ++i) {
        celkovaVaha += vahaOvocia.at(i);
    }
    cout << "Celkový zárobok je " << celkovaVaha << "€" << endl;
    for (int i = 0; i < vahaOvocia.size(); ++i) {
        cout << "Ovocie " << i << ". vazilo " << vahaOvocia.at(i) << "€ a podiel "<< vahaOvocia.at(i)*100/celkovaVaha << "%" << endl;
    }
/*  int i = 0;
    for (auto item: pult) {
        cout << i++ << ". " << item << endl;
    }*/
    return 0;
}
