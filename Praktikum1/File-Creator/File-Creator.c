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
#include <getopt.h>

#include <sys/types.h>// für Linux
#include <sys/stat.h> // für Linux

//#include <io.h>       // für Windows
//#include <sys\stat.h> // für Windows

#include <fcntl.h>

//#include "File-Creator.h"

// Globale Variablen
#define FEHLER -1;
#define KEIN_FEHLER 0;
#define BUFFER_NAMEN_GROESSE 10;

/*
 * Globale Variablen 
 */
bool debugModeOn = false;
int curAnzahlThreads = 0;
int MAXanzahlThreads = 3;
// Dateibuffer variablen
pthread_mutex_t mutex_AnzahlThreads;
int MAX_BUFFER_GROESSE = 20;
int MAXdateiNamenLaenge = 15;
char * * ppDateiNamenBuffer;
int dateinamenBufferFuellZeiger;
int dateinamenBufferEntnahmeZeiger;
pthread_mutex_t mutex_DateiNamen;
char * pNamensDateiName;
// Dateinamen variablen
int curAnzahlDateinamen = 0;
bool alleDateienEingelesen = false;
pthread_mutex_t mutex_AnzahlDateinamen;
// Thread beendet variablen
int threadsBeendet = 0;
pthread_mutex_t mutex_ThreadsBeendet;


static void hilfetextAusgeben(){
    fprintf(stderr,"usage: File-Creator [-c <MaxAnzahlThreads>] <NAMENDATEI>\n");
}

static void * namenEinlesen(){
    if(debugModeOn){
        fprintf(stderr,"EinleseThread gestartet\n");
    }
    
    FILE * pEinleseDatei = fopen(pNamensDateiName,"r");
    //char * tempDateiname;
    char * pTempDateiName = malloc(sizeof(char)*15);
    char * endLineChecker = fgets(pTempDateiName, MAXdateiNamenLaenge, pEinleseDatei);
    while(endLineChecker != NULL){
        
        if(debugModeOn){
            fprintf(stderr,"EinleseThread while Anfang----------\n%s\n", pTempDateiName
            );
        }
        
        // Endline wegnehmen
        char * pEndLine = strchr(pTempDateiName, '\n');
        if(pEndLine != NULL){
            *pEndLine = '\0';
        }   
        
        if(debugModeOn){
            fprintf(stderr,"%s\n", pTempDateiName );
        }
        
        bool dateinameIstGeschrieben = false;
        while(!dateinameIstGeschrieben){
            
            pthread_mutex_lock(&mutex_DateiNamen);
            int tempDateinamenBufferFuellZeiger = dateinamenBufferFuellZeiger;
            int tempDateinamenBufferEntnahmeZeiger = dateinamenBufferEntnahmeZeiger;
            pthread_mutex_unlock(&mutex_DateiNamen);
            int space = tempDateinamenBufferFuellZeiger - tempDateinamenBufferEntnahmeZeiger;
            if((space >= 0) && (space < MAX_BUFFER_GROESSE)){
                         
                int bufferWritePosition = (tempDateinamenBufferFuellZeiger % MAX_BUFFER_GROESSE);
                // Globale Buffervariable ändern und in den Buffer schreiben.
                pthread_mutex_lock(&mutex_DateiNamen);
                ppDateiNamenBuffer[bufferWritePosition] = pTempDateiName;
                dateinamenBufferFuellZeiger++;
                if(debugModeOn){
                    fprintf(stderr,"bufferWriterPosition: %d %s\n", bufferWritePosition, ppDateiNamenBuffer[bufferWritePosition]);
                }
                pthread_mutex_unlock(&mutex_DateiNamen);
                
                // Globalen Dateinamencounter inkrmentieren.
                pthread_mutex_lock(&mutex_AnzahlDateinamen);
                curAnzahlDateinamen++;
                pthread_mutex_unlock(&mutex_AnzahlDateinamen);
                
                dateinameIstGeschrieben = true;
            }    
        }
        
        if(debugModeOn){
            fprintf(stderr,"1 Name eingelesen\n");
        }
        
        endLineChecker = fgets(pTempDateiName, MAXdateiNamenLaenge, pEinleseDatei);
    }
    
    if(debugModeOn){
        fprintf(stderr,"Einlese Thread wird beendet\n");
    }
    
    pthread_mutex_lock(&mutex_AnzahlDateinamen);
    alleDateienEingelesen = true;
    pthread_mutex_unlock(&mutex_AnzahlDateinamen);
    pthread_exit(NULL); //exit(3);
}

static void * create() {
    
    if(debugModeOn){
        fprintf(stderr,"Create Thread wurde gestartet\n");
    }
    
    bool nameGeholt = false;
    char * dateiname = malloc(sizeof(char)*15);
    while(!nameGeholt){
        if(debugModeOn){
            fprintf(stderr,"Schleifenanfang im Thread\n");
        }
        pthread_mutex_lock(&mutex_DateiNamen);
        int tempDateinamenBufferFuellZeiger = dateinamenBufferFuellZeiger;
        int tempDateinamenBufferEntnahmeZeiger = dateinamenBufferEntnahmeZeiger;
        int tempSize = tempDateinamenBufferFuellZeiger-tempDateinamenBufferEntnahmeZeiger;
        
        if(tempSize > 0){
            dateiname = ppDateiNamenBuffer[tempDateinamenBufferEntnahmeZeiger % MAX_BUFFER_GROESSE];
            if(debugModeOn){
                fprintf(stderr,"DateinamenBufferPos: %d\n%s\n", tempDateinamenBufferEntnahmeZeiger % MAX_BUFFER_GROESSE, ppDateiNamenBuffer[tempDateinamenBufferEntnahmeZeiger % MAX_BUFFER_GROESSE]);
            }
            dateinamenBufferEntnahmeZeiger++;
            nameGeholt = true;
        }
        pthread_mutex_unlock(&mutex_DateiNamen);
    }    
    
    if(debugModeOn){
        fprintf(stderr,"Create Thread hat Namen geholt: %s\n", dateiname);
    }
    
    FILE * pFile;
    if (dateiname != NULL) {
        pFile = fopen(dateiname, "w");
        if (pFile != NULL) {
            fclose(pFile);
            pFile = NULL;
            
            if(debugModeOn){
                fprintf(stderr,"Datei erstellt\n");
            }
        }
    }
    
    pFile = NULL;
    pthread_mutex_lock (&mutex_ThreadsBeendet);
    threadsBeendet++;
    pthread_mutex_unlock (&mutex_ThreadsBeendet);
    pthread_mutex_lock (&mutex_AnzahlThreads);
    curAnzahlThreads--;
    pthread_mutex_unlock (&mutex_AnzahlThreads);
    
    if(debugModeOn){
        fprintf(stderr,"Create Thread wird beendet!\n");
    }
    
    pthread_exit(NULL); //exit(3);
}
/*
int getPlatzImArray(pthread_t* pThreadArray[]){
    for(int i = 0; i < MAXanzahlThreads; i++){
        if(pThreadArray[i] == NULL){
            return i;
        }
    }
    return FEHLER;
}
*/
int main(int argc, char* argv[]) {
    fprintf(stderr,"Programm läuft...\n");
    pthread_mutex_init(&mutex_DateiNamen, NULL);
    pthread_mutex_init(&mutex_AnzahlThreads, NULL);
    pthread_mutex_init(&mutex_AnzahlDateinamen, NULL);
    curAnzahlThreads = 0;
    int c;
    

    while ((c = getopt(argc, argv, "vC:")) != -1) {
        fprintf(stderr,"while Durchlauf\n");
        switch (c) {
            case 'C':
                MAXanzahlThreads = atoi(optarg);
                fprintf(stderr,"Max Anzahl Threads gesetzt: %d\n", MAXanzahlThreads);
                break;
            case 'v':
                fprintf(stderr,"Debug Mode!\n");
                debugModeOn = true;
                break;
            default :
                hilfetextAusgeben();
                return EXIT_SUCCESS;
                break;    
        }
    }
    fprintf(stderr,"while verlassen\n");
    
    if(MAXanzahlThreads <= 0){
        fprintf(stderr,"!!! Für -C <Ganzzahl>0> !!!\n");
        return EXIT_FAILURE;
    }
    
    pNamensDateiName = argv[argc-1];
    
    if(debugModeOn){
        fprintf(stderr,"Dateiname: %s\n", pNamensDateiName);
        fprintf(stderr,"MAXanzahlThreads: %d\n", MAXanzahlThreads);
        fprintf(stderr,"MAX_BUFFER_GROESSE: %d\n", MAX_BUFFER_GROESSE);
    }
    
    // Testen, ob Datei lesbar.
    FILE * pFile = fopen(pNamensDateiName,"r");
    if(pFile == NULL){
        fprintf(stderr,"Datei nicht lesbar oder nicht übergeben.\n");
        return EXIT_FAILURE;
    }
    
    if(debugModeOn){
        fprintf(stderr,"Datei lesbar\n");
    }
    
    fclose(pFile);
    pFile = NULL;
    
    // Globale Variablen initialisieren
    pthread_mutex_lock(&mutex_DateiNamen);
    dateinamenBufferFuellZeiger = 0;
    dateinamenBufferEntnahmeZeiger = 0;
    ppDateiNamenBuffer = malloc(sizeof(char*)*(MAX_BUFFER_GROESSE+1));
    pthread_mutex_unlock(&mutex_DateiNamen);
    
    // Einlesethread starten
    pthread_t pEinleseThread;
    if(pthread_create(&pEinleseThread, NULL, namenEinlesen, NULL) != 0 ){
        fprintf(stderr,"EinleseThread nicht gestartet!");
        return EXIT_FAILURE;
    }
    
    // Threads starten und Listen
    bool alleThreadsGestartet = false;
    //pthread_t* pThreadArray[MAXanzahlThreads];
    //malloc(pThreadArray, sizeof(pthread_t)*(MAXanzahlThreads));
    int threadStartedCounter = 0;
    
    do {
//         if(debugModeOn){
//             fprintf(stderr,"Creater Threads Startschleife anfang\n");
//         }
        // Globalen Dateinamencounter abfragen
        pthread_mutex_lock(&mutex_AnzahlDateinamen);
        int tempAnzahlDateinamen = curAnzahlDateinamen;
        pthread_mutex_unlock(&mutex_AnzahlDateinamen);
        while(tempAnzahlDateinamen > threadStartedCounter ){
//             if(debugModeOn){
//                 fprintf(stderr,"Creater Threads innere Schleife anfang\n");
//             }
            //int arrayPlatz = getPlatzImArray(pThreadArray);
            //if(arrayPlatz < 0){
            //    fprintf(stderr,"Fehler im Thread-Array!");
            //    return EXIT_FAILURE;
            //}
            pthread_t myThread = 0;
            if(pthread_create(&myThread, NULL, create, NULL) == 0){
                //myThreadArray[arrayPlatz] = thread;
                threadStartedCounter++;
                fprintf(stderr,"Thread %d wurde gestartet.\n", threadStartedCounter);
            } else {
                fprintf(stderr,"Thread %d konnte nicht gestartet werden.\n", threadStartedCounter);
            }
            // Globalen Dateinamencounter abfragen.    
            pthread_mutex_lock(&mutex_AnzahlDateinamen);
            tempAnzahlDateinamen = curAnzahlDateinamen;
            pthread_mutex_unlock(&mutex_AnzahlDateinamen);
//             if(debugModeOn){
//                 fprintf(stderr,"Creater Threads innere Schleife Ende\n");
//             }
        }
        pthread_mutex_lock(&mutex_AnzahlDateinamen);
        bool tempAlleDateienEingelesen = alleDateienEingelesen;
        pthread_mutex_unlock(&mutex_AnzahlDateinamen);
        if(tempAlleDateienEingelesen && (tempAnzahlDateinamen > 0) && (tempAnzahlDateinamen == threadStartedCounter)){
            alleThreadsGestartet = true;
        }
    } while(!alleThreadsGestartet);

    if(debugModeOn){
        fprintf(stderr,"Creater Threads Startschleife beendet\n");
    }
    
    // warten auf alle Threads
    pthread_mutex_lock (&mutex_ThreadsBeendet);
        int tempThreadsBeendet = threadsBeendet;
    pthread_mutex_unlock (&mutex_ThreadsBeendet);
    do{
        pthread_mutex_lock (&mutex_ThreadsBeendet);
        tempThreadsBeendet = threadsBeendet;
        pthread_mutex_unlock (&mutex_ThreadsBeendet);
//         if(debugModeOn){
//             fprintf(stderr,"threadsBeendet < threadStartedCounter:\n       %d       <       %d\n", tempThreadsBeendet, threadStartedCounter);
//         }
        
    } while(tempThreadsBeendet < threadStartedCounter);
    
    if(debugModeOn){
        fprintf(stderr,"Alle Threades beendet\n");
    }
    
    // Aufräumen
    //free(pThreadArray);
    //pThreadArray = NULL;
    free(ppDateiNamenBuffer);
    ppDateiNamenBuffer = NULL;
 
    if(debugModeOn){
        fprintf(stderr,"Programm Ende\n");
    }
    return (EXIT_SUCCESS);
}

