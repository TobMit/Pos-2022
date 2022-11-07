#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <stdlib.h>

int main(int argc, char * argv[]) {
    printf("Hodnota premennej USER je: %s\n", getenv("USER"));
    printf("Vystvaram potomka, som %d\n", getpid());
    pid_t ret = fork(); // vytvorí child proces ako duplicitu aktuálneho procesu
    // keď poutžívame fork a pokiaľ nepočkáme z predka na potomoka, mohol by vzniknúť zoombie process
    // treba najskôr počkať aby skončil aby predok stihol ukončiť potomka
    if (ret < 0) {
        perror("nastala chyba pri forku! \n");
    } else if (ret == 0) {
        //execlp("ls", "ls", "-l", "-a", "/tmp", (char *) NULL); // vypíše nam tmp
        printf("Nachádzam sa v potomkovy, moje id je %d, môj predok ma id %d\n", getpid(), getppid());
        execvp(argv[1], argv+1); // pozerám sa na to ako na pole ale začínam od 1 prvku (nie od nultého) // exec prepíše všetko čo sa spúšťa po tom
        return 55; // táto hodnota sa nám vracia po skončení detského procesu a nájdeme ju v wstatus
    } else {
        printf("Predok sa ozyva po forku, moje pid je %d, môj predok je %d  a môj potomok je %d\n", getpid(), getppid(), ret);

        int wstatus; // čaká na proces kým nezmení status
        wait(&wstatus);
        if (WIFEXITED(wstatus)) {
            printf("wstatus. %d\n", WEXITSTATUS(wstatus));
        } else {
            perror("nastala chyba pri ukončeni potomka");
        }

        //printf("wstatus. %d\n", wstatus);
        return 0;
    }
    return 0;
}
