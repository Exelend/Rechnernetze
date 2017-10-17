/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   File-Creator.c
 * Author: MaWi
 *
 * Created on 17. Oktober 2017, 10:52
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
static FILE* create(char* name){
    FILE * pFile = NULL;
    if(name != NULL){
        pFile = fopen(name, "w");
        fclose(pFile);
        pFile = NULL;
    }
    return pFile;
}


int main(int argc, char* argv[]) {
    
    char * pFilename = NULL;
    int MAXanzahlThreads = 0;
    int curAnzahlThreads = 0;
    
    if(argc > 1){
        int temp = 0;
        for(int i = 0; i < argc; i++) {
            int j = 0;
            if(strchr("-C", argv[i])){
                MAXanzahlThreads = atoi(argv[i+1]);
            } else if(temp = open(argv[i], O_RDONLY) <= 0){ // Lesender Zugriff auf Datei möglich?    
                // Dateiname
                pFilename = argv[i];
                close(temp);
            }
            j++;
	}
        if(pFilename == NULL){
            printf("Datei nicht lesbar");
            return EXIT_FAILURE;
        }
    } else {
        printf("Bitte Textdatei mit Namen uebergeben!");
    }
    // TODO args trennen und prüfen.
    
    pFile = fopen();
    FILE * pFile = NULL;
    
    
    return (EXIT_SUCCESS);
}

