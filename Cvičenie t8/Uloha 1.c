//
// Created by Tobias on 07/11/2022.
//
#include <stdio.h>

int main(int argc, char *argv[]) {
    if  (argc < 1) {
        printf("Program bez argumentov");
    }
    for (int i = 0; i < argc; ++i) {
        printf("%d. argument je: %s\n ",i, argv[i]);
    }
}
