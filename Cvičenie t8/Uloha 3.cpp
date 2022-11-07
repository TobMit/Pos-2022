#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
int main() {
    printf("Vystvaram potomka, som %d\n", getpid());
    pid_t ret = fork(); // vytvorí child proces ako duplicitu aktuálneho procesu
    if (ret < 0) {
        perror("nastala chyba pri forku! \n");
    } else if (ret == 0) {
        printf("Nachádzam sa v potomkovy, moje id je %d, môj predok ma id %d\n", getpid(), getppid());
        return 55;
    } else {
        printf("Predok sa ozyva po forku, moje pid je %d, môj predok je %d  a môj potomok je %d\n", getpid(), getppid(), ret);
        return 0;
    }
    return 0;
}
