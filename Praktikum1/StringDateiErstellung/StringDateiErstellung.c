/*
 *
 */

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int anzahlNamen = 5;
    /*
    if(argc > 0){
        atoi(argv[1]);
    }
    */
    FILE * pFile = fopen("namenFuerFile-Creator.txt", "wt");
    char * dummyString = "Dummy";
    for(int i = 0; i<anzahlNamen; i++){
        fprintf(pFile, "%s%06d\n", dummyString, i);
    }
    fclose(pFile);
    return 0;
}