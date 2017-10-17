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
    }
    return pFile;
}


int main(int argc, char* argv[]) {
    
    if(argc > 1){
        
    }
    // TODO args trennen und prüfen.
    
    int MAXanzahlThreads = 0;
    int curAnzahlThreads = 0;
    pthread_t threadArray[MAXanzahlThreads] = {};
    
    FILE * pNamen = fopen();
    do{
        // TODO Threads starten und verwalten
    } while(//TODO);
        
    return (EXIT_SUCCESS);
}

