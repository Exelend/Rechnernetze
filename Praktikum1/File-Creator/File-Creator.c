/* 
 * File:   File-Creator.c
 * Author: MaWi
 *
 * Created on 17. Oktober 2017, 10:52
 * 
 * 
 * Compile with -pthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/types.h>// für Linux
#include <sys/stat.h> // für Linux

#include <io.h>       // für Windows
#include <sys\stat.h> // für Windows

#include <fcntl.h>


#include "File-Creator.h"
/*
 * 
 */

// Thread Create

void hilfetextAusgeben(){
    printf("usage: File-Creator [-c <MaxAnzahlThreads>] <NAMENDATEI>");
}

static FILE* create(char* name) {
    FILE * pFile = NULL;
    if (name != NULL) {
        pFile = fopen(name, "w");
        if (pFile != NULL) {
            fclose(pFile);
            pFile = NULL;
        }
    }
    exit(3);
}

int main(int argc, char* argv[]) {

    char * pFilename = NULL;
    int MAXanzahlThreads = 3;
    int curAnzahlThreads = 0;

    int c;

    while ((c = getopt(argc, argv, "C:") != -1)) {
        switch (c) {
            case 'C':
                MAXanzahlThreads = atoi(optarg);
                break;
            case '?':
                hilfetextAusgeben();
                return EXIT_FAILURE;
                break;
        }
    }
    
    if(MAXanzahlThreads <= 0){
        printf("!!! Für -C <Ganzzahl>0> !!!");
        return EXIT_FAILURE;
    }

    pFilename = argv[argc-1];
    
    FILE pFile = NULL;
    if(pFile = fopen() == NULL){
        printf("Datei nicht lesbar oder nicht übergeben.");
        return EXIT_FAILURE;
    }

    pthread_t [] = MAXanzahlThreads;
    
    
    close(pFile);
    FILE * pFile = NULL;


    return (EXIT_SUCCESS);
}

