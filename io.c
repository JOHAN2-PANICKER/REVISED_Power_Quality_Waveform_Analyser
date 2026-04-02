//
// Created by johan on 02/04/2026.
/*All file I/O: loading the CSV into your struct array, writing the results file.*/

#include <stdio.h>
#include <stdlib.h>
#include "io.h"

//Create 'countRows' Function to count number of rows in the CSV file.
static int CountRows (const char*filename) {
    FILE *fp = fopen(filename, "r");
    //Print error message if file cannot be opened.
    if (fp == NULL) {
        printf("countRows: failed to open %s\n",filename);
        return -1;
    }

    char line[256];
    int count = 0;

    //Closes file if datasheet is empty.
    if (fgets(line, sizeof(line), fp) != NULL) {
        fclose(fp);
        return 0;
    }
    //Create while loop to count rows until empty.
    while (fgets(line,sizeof(line),fp) != NULL) {
        count++;
    }
    fclose(fp);
    return count;
}

//

