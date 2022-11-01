#include "date.h"
//
// Created by Tobias on 01/11/2022.
//

int date_compare(DATE dateA, DATE dateB) {
    if (dateA.year < dateB.year ||
        dateA.year == dateB.year && dateA.month < dateB.month ||
        dateA.year == dateB.year && dateA.month == dateB.month && dateA.day < dateB.day) {
        return -1;
    } else if (dateA.year == dateB.year && dateA.month == dateB.month && dateA.day == dateB.day) {
        return 0;
    } else {
        return 1;
    }
}

void date_print(DATE dateA) {
    printf("%d.%d.%d", dateA.day, dateA.month, dateA.year);
}