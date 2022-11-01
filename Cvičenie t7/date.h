//
// Created by Tobias on 01/11/2022.
//

#ifndef POS_2022_DATE_H
#define POS_2022_DATE_H

//struct date { // buď takto alebo potom cez typedef aby som vo funkciách nemusel dávať všade struct
typedef struct date {
    int year;
    int month;
    int day;
} DATE;

//int date_compare(struct date dateA, struct date dateB);
int date_compare(DATE dateA, DATE dateB);


#endif //POS_2022_DATE_H
