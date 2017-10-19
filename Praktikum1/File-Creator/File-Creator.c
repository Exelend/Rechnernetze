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
#include <string.h>

#include <sys/types.h>// für Linux
#include <sys/stat.h> // für Linux

//#include <io.h>       // für Windows
//#include <sys\stat.h> // für Windows

#include <fcntl.h>

#include "File-Creator.h"
/*
 * 
 */
int curAnzahlThreads = 0;
pthread_mutex_t mutex_AnzahlThreads;
int MAX_BUFFER_GROESSE = 20;
int MAXdateiNamenLaenge = 15;
char ** ppDateiNamenBuffer;
int dateinamenBufferFuellZeiger;
int dateinamenBufferEntnahmeZeiger;
pthread_mutex_t mutex_DateiNamen;
char * pNamensDateiName;

static void hilfetextAusgeben(){
    printf("usage: File-Creator [-c <MaxAnzahlThreads>] <NAMENDATEI>");
}

static void namenEinlesen(){
    FILE * pEinleseDatei = fopen(pNamensDateiName,"r");
    char * tempDateiname;
    char* pTempDateiName = malloc(sizeof(char)*15);
    fgets(pTempDateiName, MAXdateiNamenLaenge, pEinleseDatei);
    while(pTempDateiName != NULL){
        bool dateinameIstGeschrieben = false;
        while(!dateinameIstGeschrieben){
            pthread_mutex_lock(mutex_DateiNamen);
            int tempDateinamenBufferFuellZeiger;
            int tempDateinamenBufferEntnahmeZeiger;
            pthread_mutex_unlock(mutex_DateiNamen);
            int space = tempDateinamenBufferFuellZeiger - tempDateinamenBufferEntnahmeZeiger;
            if((space >= 0) && (space < MAX_BUFFER_GROESSE)){
                char * pEndLine = strchr(pTempDateiName, '\n');
                if(pEndLine != NULL){
                    *pEndLine = '\0';
                }            
                int bufferWritePosition = tempDateinamenBufferFuellZeiger%MAX_BUFFER_GROESSE;
                pthread_mutex_lock(mutex_DateiNamen);
                ppDateiNamenBuffer[bufferWritePosition] = pTempDateiName;
                tempDateinamenBufferFuellZeiger++;
                pthread_mutex_unlock(mutex_DateiNamen);
                dateinameIstGeschrieben = true;
            }    
        }
        fgets(pTempDateiName, MAXdateiNamenLaenge, pEinleseDatei);
    }
    exit(3);
}

static void * create() {
    bool nameGeholt = false;
    char * dateiname;
    while(!nameGeholt){
        pthread_mutex_lock(mutex_DateiNamen);
        int tempDateinamenBufferFuellZeiger = dateinamenBufferFuellZeiger;
        int tempDateinamenBufferEntnahmeZeiger = dateinamenBufferEntnahmeZeiger;
        int tempSize = tempDateinamenBufferFuellZeiger-tempDateinamenBufferEntnahmeZeiger;
        if(tempSize>0){
            dateiname = ppDateiNamenBuffer[tempDateinamenBufferEntnahmeZeiger%MAX_BUFFER_GROESSE];
            dateinamenBufferEntnahmeZeiger++;
        }
        pthread_mutex_unlock(mutex_DateiNamen);
    }    
    
    FILE * pFile;
    if (dateiname != NULL) {
        pFile = fopen(dateiname, "w");
        if (pFile != NULL) {
            fclose(pFile);
            pFile = NULL;
        }
    }
    
    pFile = NULL;
    pthread_mutex_lock (&mutex_AnzahlThreads);
    curAnzahlThreads--;
    pthread_mutex_unlock (&mutex_AnzahlThreads);
    exit(3);
}

int main(int argc, char* argv[]) {
    pthread_mutex_init (&mutex_DateiNamen, NULL);
    pthread_mutex_init (&mutex_AnzahlThreads, NULL);
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

    pNamensDateiName = argv[argc-1];
    
    // Testen, ob Datei lesbar.
    FILE * pFile = fopen(pNamensDateiName,"r");
    if(pFile == NULL){
        printf("Datei nicht lesbar oder nicht übergeben.");
        return EXIT_FAILURE;
    }
    fclose(pFile);
    pFile = NULL;
    
    // Globale Variablen initialisieren
    pthread_mutex_lock(mutex_DateiNamen);
    dateinamenBufferFuellZeiger = 0;
    dateinamenBufferEntnahmeZeiger = 0;
    malloc(ppDateiNamenBuffer, sizeof(char)*MAXdateiNamenLaenge*(MAX_BUFFER_GROESSE+1));
    pthread_mutex_unlock(mutex_DateiNamen);
    
    // Einlesethread starten.
    pthread_t pEinleseThread;
    if(pthread_create(&pEinleseThread, NULL, create, NULL) != 0 ){
        printf("EinleseThread nicht gestartet!");
        return EXIT_FAILURE;
    }
    
    // Threads starten und Listen
    pthread_t* pThreadArray[];
    int threadCounter = 0;
    
    while()
    
    
    
    
    /*
    do{
        fgets(pTempDateiName, MAXdateiNamenLaenge, pFile);
        //TODO \n entfernen???
        int tempCurAnzahlThreads;
        if(pTempDateiName != NULL){
            bool threadIstGestartet = false;
            while(!threadIstGestartet){
                pthread_mutex_lock (&mutex_AnzahlThreads);
                tempCurAnzahlThreads = curAnzahlThreads;
                pthread_mutex_unlock (&mutex_AnzahlThreads);
                if(tempCurAnzahlThreads < MAXanzahlThreads){
                    pthread_t pTh;
                    if( pthread_create(&pTh, NULL, create, NULL) != 0){
                        errorcounter++;
                    } else {
                        threadIstGestartet = true;
                        pthread_mutex_lock (&mutex_AnzahlThreads);
                        curAnzahlThreads++;
                        pthread_mutex_unlock (&mutex_AnzahlThreads);
                    }
                }
            } 
        } 
    } while(pTempDateiName != NULL);
    */
    
    
    
    
    
    
    // TODO warten auf Threads!!!
    
    free(ppDateiNamenBuffer);
    close(pFile);
    FILE * pFile = NULL;


    return (EXIT_SUCCESS);
}

