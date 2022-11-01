//
// Created by Tobias on 01/11/2022.
//

#include "stdio.h"
int main() {
    char name[30+1]; // posledný znak je určite null
    int year;


    printf("Zadaj meno: ");
    scanf("%s", name); // polia sa automaticky konvertujú na char* takže preto to tam nemusím dávať
    printf("Zadaj rok narodenia");
    scanf("%d", &year);

    printf("Ahoj %s, mas %d rokov.\n" , name, 2022-year);
    return 0;
}