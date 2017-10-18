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
#include <unistd.h>
#include <stdbool.h>

#include <sys/types.h>// für Linux
#include <sys/stat.h> // für Linux

//#include <io.h>       // für Windows
//#include <sys\stat.h> // für Windows

#include <fcntl.h>


#include "File-Creator.h"
/*
 * 
 */
int curAnzahlThreads;
pthread_mutex_t mutex;


static void hilfetextAusgeben(){
    printf("usage: File-Creator [-c <MaxAnzahlThreads>] <NAMENDATEI>");
}

static void create(char* name) {
    FILE * pFile = NULL;
    if (name != NULL) {
        pFile = fopen(name, "w");
        if (pFile != NULL) {
            fclose(pFile);
            pFile = NULL;
        }
    }
    pthread_mutex_lock (&mutex);
    curAnzahlThreads--;
    pthread_mutex_unlock (&mutex);
    exit(3);
}

int main(int argc, char* argv[]) {

    char * pFilename = NULL;
    int MAXanzahlThreads = 3;
    curAnzahlThreads = 0;

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
    
    // Datei öffnen
    FILE pFile = NULL;
    if(pFile = fopen() == NULL){
        printf("Datei nicht lesbar oder nicht übergeben.");
        return EXIT_FAILURE;
    }
    
    // Threads bauen
    int MAXdateiNamenLaenge = 15;
    char* tempDateiName;
    int errorcounter = 0;
    do{
        fgets(tempDateiName, MAXdateiNamenLaenge, pFile);
        int tempCurAnzahlThreads;
        if(tempDateiName != NULL){
            bool threadIstGestartet = false;
            while(!threadIstGestartet){
                pthread_mutex_lock (&mutex);
                tempCurAnzahlThreads = curAnzahlThreads;
                pthread_mutex_unlock (&mutex);
                if(tempCurAnzahlThreads < MAXanzahlThreads){
                    pthread_t pTh = NULL;
                    if(pthread_create(pTh, NULL, create(tempDateiName), NULL)!= 0){
                        errorcounter++;
                    } else {
                        threadIstGestartet = true;
                        pthread_mutex_lock (&mutex);
                        curAnzahlThreads++;
                        pthread_mutex_unlock (&mutex);
                    }
                }
            }
            
            
        }        
        
    } while(tempDateiName != NULL);
    
    
    close(pFile);
    FILE * pFile = NULL;


    return (EXIT_SUCCESS);
}

