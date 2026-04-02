//
// Created by johan on 02/04/2026.
/*All file I/O: loading the CSV into your struct array, writing the results file.*/

#include <stdio.h>
#include <stdlib.h>
#include "io.h"

//Create a function to count number of rows in the CSV file.
static int CountRows (const char*filename) { //static= fn only visible inside c. int = format of fn output.
    FILE *fp = fopen(filename, "r");
    //Print error message if file cannot be opened.
    if (fp == NULL) {
        printf("countRows: failed to open %s\n",filename);
        return -1;
    }

    char line[256];//temp storage to hold one line of text from CSV file.256 because strings can take max of 255 char + \0.
    int count = 0;

    //Closes file if datasheet is empty.
    if (fgets(line, sizeof(line), fp) == NULL) {
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

//Create a function to allocate memory for waveform samples.
static WaveformSample* allocateSamples(int count){
    WaveformSample *samples = malloc((size_t)count* sizeof(WaveformSample));
    // Error message if no samples are detected.
    if (samples == NULL){
        printf("allocateSamples: memory allocation failed.\n");
        return NULL;
    } return samples;
}

//Create a function that populates samples array using pointer-based access.
static int populateSamples(FILE *fp,WaveformSample *samples){
    char line [256];

    //Skip header row.
    if (fgets(line,sizeof(line),fp) == NULL) {
        return 0;
    }
    //Create pointer for pointer-based access.
    WaveformSample *ptr = samples;

    //Create while loop to capture each row into the samples array until row is empty.
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
                   &ptr->timestamp,
                   &ptr->phase_A_voltage,
                   &ptr->phase_B_voltage,
                   &ptr->phase_C_voltage,
                   &ptr->line_current,
                   &ptr->frequency,
                   &ptr->power_factor,
                   &ptr->thd_percent) == 8) // '==8' included as fail-safe to ensure 8 rows of data is captured.
            ptr++; //pointer-based traversal.
    }
    fclose(fp); //close the file once loop is completed.
    return samples; //gives address to the populated array (address of first element).
    }


