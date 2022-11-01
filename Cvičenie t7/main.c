
#include "stdio.h"
#include "date.h"
#include "array_list.h"
#include "stdlib.h"

void test_array_list(void) { // keď neočakávame žiadne parametre tak treba tam dať void
    ARRAY_LIST list;
    array_list_init(&list);

    for (int i = 0; i < 10; ++i) {
        DATE  date = {2022, 1 + rand() % 12, 1 + rand() % 28};

        array_list_add(&list, &date);
        printf("%d\n", i);
    }
    array_list_print(&list);
    array_list_dispose(&list);
}

int main(void) {
    /*
    char name[30+1]; // posledný znak je určite null
    int year;


    printf("Zadaj meno: ");
    //scanf("%s", name); // polia sa automaticky konvertujú na char* takže preto to tam nemusím dávať
    fgets(name, 30+1, stdin); // tá 30+1 načíta maximálne (30+1)-1 znakov, takže keď chcem načítať 30 znakov tak musím pripočítať ešte 1
    name[strlen(name) - 1] = '\0'; // keďže fgets načítava aj nový riadk, tak posledný znak ktorý bol načítaný nahradíme za null
    printf("Zadaj rok narodenia: ");
    scanf("%d", &year);

    printf("Ahoj %s, mas %d rokov.\n" , name, 2022-year);
    */
    test_array_list();
    return 0;
}