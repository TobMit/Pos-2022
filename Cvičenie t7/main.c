//
// Created by Tobias on 01/11/2022.
//

#include "stdio.h"
#include "string.h"
int main() {
    char name[30+1]; // posledný znak je určite null
    int year;


    printf("Zadaj meno: ");
    //scanf("%s", name); // polia sa automaticky konvertujú na char* takže preto to tam nemusím dávať
    fgets(name, 30+1, stdin); // tá 30+1 načíta maximálne (30+1)-1 znakov, takže keď chcem načítať 30 znakov tak musím pripočítať ešte 1
    name[strlen(name) - 1] = '\0'; // keďže fgets načítava aj nový riadk, tak posledný znak ktorý bol načítaný nahradíme za null
    printf("Zadaj rok narodenia: ");
    scanf("%d", &year);

    printf("Ahoj %s, mas %d rokov.\n" , name, 2022-year);
    return 0;
}