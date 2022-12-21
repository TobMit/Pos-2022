#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

using namespace std;

#define POCET_ZBERACOV 6
#define K_KUSOV_OVOCIA 4

enum ovocie {JABLKO = 1, HRUSKA = 2, SLIVKA = 3};

bool pokazene(ovocie pOvocie) {
    switch (pOvocie) {
        case JABLKO:
            return rand() % 100 < 30 ? true : false;
        case HRUSKA:
            return rand() % 100 < 60 ? true : false;
        default:
            return rand() % 100 < 10 ? true : false;
    }
}
ovocie getOvocie(){
    int randomHodnota = rand() % 100;
    if (randomHodnota < 50) {
        return JABLKO;
    } else if (randomHodnota < 80) {
        return HRUSKA;
    } else {
        return SLIVKA;
    }
}

string getNazovOvocia (ovocie pOvocie) {
    switch (pOvocie) {
        case JABLKO:
            return "JABLKO";
        case HRUSKA:
            return "HRUŠKA";
        default:
            return "SLIVKA";
    }
}


void zberac(int id, mutex *mut, condition_variable *pridaj, condition_variable *odober, vector<ovocie> &ovocieBuffer, int *celkovyPocet, int *pokazenePocet ) {
    printf("Zberač %d: Vytvoreny!\n", id);
    int pocetOvocia  = 0;
    while (pocetOvocia < K_KUSOV_OVOCIA * 4) {
        printf("Zberač %d: Som pri bedničke a začinam presun k sadu!\n", id);
        this_thread::sleep_for(chrono::milliseconds(1000 + rand()%3001));
        printf("Zberač %d: Prišiel som k sadu!\n", id);
        {
            unique_lock<mutex> lock(*mut);
            while (ovocieBuffer.empty()) {
                printf("Zberač %d: V sade nie je ovocie, čakam pred sadom!\n", id);
                odober->wait(lock);
            }
            printf("Zberač %d: Vstupujem do sadu!\n", id);
            int aktualneNazberane = 0;
            while(!ovocieBuffer.empty() && aktualneNazberane < K_KUSOV_OVOCIA && pocetOvocia < K_KUSOV_OVOCIA *4) {
                ovocie zozbieraneOvocie = ovocieBuffer.at(ovocieBuffer.size() - 1);
                ovocieBuffer.pop_back();
                printf("Zberač %d: Nasiel som %s a v sade je %d ovocia!\n", id, getNazovOvocia(zozbieraneOvocie).c_str(), ovocieBuffer.size());
                *celkovyPocet +=1;
                aktualneNazberane++;
                pocetOvocia++;
                *pokazenePocet += pokazene(zozbieraneOvocie) ? 0 : 1;
            }
            pridaj->notify_one();
            printf("Zberač %d: Dozbieral som!\n", id);
        }
        printf("Zberač %d: Opustil som sad a idem k bedničke\n", id);
        this_thread::sleep_for(chrono::milliseconds(1000 + rand()%3001));
        printf("Zberač %d: Som pri bedničke a vykladám ovocie!\n", id);
    }
    printf("Zberač %d: Konči robotu, nazbierane!\n", id);
}

void sad(mutex *mut, condition_variable * pridaj, condition_variable * odober, vector<ovocie> &ovocieBuffer, int pocetOvocia) {
    printf("SAD: Začinam robotu!\n");
    int pocetOvociaNaGenerovanie = pocetOvocia;
    while (pocetOvociaNaGenerovanie > 0 ) {
        printf("SAD: Idem sa pozrieť ci netreba priložiť ovocie!\n");
        {
            unique_lock<mutex> lock(*mut);
            while (ovocieBuffer.size() >= 15) {
                printf("SAD: Pult je plny, čakam!\n");
                pridaj->wait(lock);
            }
            printf("SAD: Idem pridať ovocie!\n");
            while (ovocieBuffer.size() < 15 && pocetOvociaNaGenerovanie-- > 0){
                ovocie vytvoreneOvocie = getOvocie();
                printf("SAD: Pridal som %s!\n", getNazovOvocia(vytvoreneOvocie).c_str());
                ovocieBuffer.push_back(vytvoreneOvocie);
            }
            printf("SAD: Ovocie pridané, odpocivam!\n");
            odober->notify_all();
        }
        this_thread::sleep_for(chrono::seconds(1));
        printf("SAD: Doodpočívane!\n");

    }
    printf("SAD: Konči robotu robotu!\n");
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int pocetZberacov = POCET_ZBERACOV;
    if (argc > 1) {
        int parameter = stoi(argv[1]);
        pocetZberacov = parameter > 0 ? parameter : POCET_ZBERACOV;
    }
    int statistika[pocetZberacov][2];
    vector<ovocie> ovocieBuffer;
    mutex mut;
    condition_variable pridaj, odober;

    cout << "Vytváram zberačov" << endl << endl;
    thread hubar[pocetZberacov];
    for (int i = 0; i < pocetZberacov; ++i) {
        statistika[i][0] = 0;
        statistika[i][1] = 0;
        hubar[i] = thread(zberac, i, &mut, &pridaj, &odober, ref(ovocieBuffer), &statistika[i][0], &statistika[i][1] );
    }
    printf("Vytvaram sad\n\n");
    thread sucicka(sad, &mut, &pridaj, &odober, ref(ovocieBuffer), pocetZberacov * 4 * K_KUSOV_OVOCIA);


    printf("Čas zberu ovocia nastal, hor sa na to!\n");
    //sleep(1);
    for (int i = 0; i < pocetZberacov; ++i) {
        hubar[i].join();
    }
    sucicka.join();

    cout << "Je nazberané! Paráda!\n";
    for (int i = 0; i < pocetZberacov; ++i) {
        cout << "Zberač " << i << ": ma uspoešnosť " << (double)((statistika[i][0] - statistika[i][1])*100/statistika[i][0]) <<"%!" << endl;
        //cout << "Zberač " << i << ": ma všetko ovocie " << statistika[i][0] <<" a pokazene ovocie "<< statistika[i][1] << endl;
    }

/*  int i = 0;
    for (auto item: pult) {
        cout << i++ << ". " << item << endl;
    }*/
    return 0;
}
