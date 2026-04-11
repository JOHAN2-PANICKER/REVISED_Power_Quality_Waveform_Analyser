//
// Created by johan on 02/04/2026.
/*All file I/O: loading the CSV into your struct array, writing the results file.*/

#include <stdio.h>
#include <stdlib.h>
#include "io.h"

//Create a function to count number of rows in the CSV file.
static int countRows (const char*filename) { //static= fn only visible inside c. int = format of fn output.
    FILE *fp = fopen(filename, "r");
    //Error message if file cannot be opened.
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
    //Create while loop to count rows after the header until empty.
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
static int populateSamples(FILE *fp,WaveformSample *samples) {
    char line[256];

    //Skip header row.
    if (fgets(line, sizeof(line), fp) == NULL) {
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

    return 1;

}

// Create function to load csv data into 'WaveformSample' array.
WaveformSample *loadCSV(const char*filename, int *count){
    *count = countRows(filename);
    //Return if csv file has empty rows.
    if (*count <= 0){
        return NULL;
    }
    //Return if samples are empty.
    WaveformSample*samples = allocateSamples(*count);
    if (samples==NULL){
        return NULL;
    }
    //Return and error message if csv file failed to open.
    FILE*fp = fopen(filename,"r");
    if (fp==NULL){
        printf("loadCSV:failed to open %s\n", filename);
        free(samples);
        return NULL;
    }

    // Return and error message if function failed to populate samples.
    if (!populateSamples(fp,samples)) { //Not statement
        printf("loadCSV: failed to populate samples.\n");
        fclose(fp);
        free(samples);
        return NULL;
    }
    fclose(fp);
    return samples;
}

int writeRawData (const char *filename, const WaveformSample *samples, int count) {
    FILE *fp = fopen (filename, "w");
    if (fp == NULL){
        // Error message for writeRawData.
        printf("writeRawData: failed to open %s\n",filename);
        return 0;
    }

    fprintf(fp, "Sample, Time, Phase_A_Voltage, Phase_B_Voltage, Phase_C_Voltage, Current, Freq, PF, THD\n");

    const WaveformSample *ptr = samples;

    // Loop to print data of all samples for raw data file.
    for (int i = 0; i < count; i++, ptr++){
        fprintf(fp,"%d,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n",
                i,
                ptr->timestamp,
                ptr->phase_A_voltage,
                ptr->phase_B_voltage,
                ptr->phase_C_voltage,
                ptr->line_current,
                ptr->frequency,
                ptr->power_factor,
                ptr->thd_percent);
    }
    fclose(fp);
    return 1;
}


int writeResults (const char*filename, const WaveformReport *report){
    FILE *fp = fopen(filename,"w");
    if (fp == NULL){
        return 0;
    }
    fprintf(fp,"Power Quality Analysis Results \n");
    fprintf(fp,"-------------------------------\n");

    fprintf(fp,"RMS Voltage: \n");
    fprintf(fp,"Phase A: %.2f V\nPhase B: %.2f V\nPhase C: %.2f V\n",
            report->rmsA,report->rmsB,report->rmsC);

    fprintf(fp,"\nPeak-to-Peak Voltage:\n");
    fprintf(fp,"Phase A: %.2f V\nPhase B: %.2f V\nPhase C: %.2f V\n",
            report->p2pA, report->p2pB, report->p2pC);

    fprintf(fp,"\nDC Offset:\n");
    fprintf(fp,"Phase A: %.2f V\nPhase B: %.2f V\nPhase C: %.2f V\n",
            report->dcA, report->dcB, report->dcC);

    fprintf(fp, "\nPhase Health Check (RMS Compliance (+/- 10%% of 230V) and Clipping): \n");
    fprintf(fp,"\nPhase A: %s, %s\n",
            (report->statusA & STATUS_OUT_OF_TOL) ? "OUT OF TOLERANCE" : "RMS COMPLIANT",
            (report->statusA & STATUS_CLIPPED) ? "CLIPPED" : "NO CLIPPING");
    fprintf(fp, "Number of Clipped Samples (|V| >= 324.9V): %d\n",report->clipA);

    fprintf(fp,"\nPhase B: %s, %s\n",
            (report->statusB & STATUS_OUT_OF_TOL) ? "OUT OF TOLERANCE" : "RMS COMPLIANT",
            (report->statusB & STATUS_CLIPPED) ? "CLIPPED" : "NO CLIPPING");
    fprintf(fp, "Number of Clipped Samples (|V| >= 324.9V): %d\n",report->clipB);

    fprintf(fp,"\nPhase C: %s, %s\n",
            (report->statusC & STATUS_OUT_OF_TOL) ? "OUT OF TOLERANCE" : "RMS COMPLIANT",
            (report->statusC & STATUS_CLIPPED) ? "CLIPPED" : "NO CLIPPING");
    fprintf(fp, "Number of Clipped Samples (|V| >= 324.9V): %d\n",report->clipC);


    fprintf(fp, "\nFrequency Range (over the 200 ms window):\n");
    fprintf(fp, "Min: %.3f Hz\nMax: %.3f Hz\nFrequency Drift: %.3f Hz\n",
             report->freqMin, report->freqMax, report->freqDrift);

    fprintf(fp, "\nPower Factor Range:\n");
    fprintf(fp, "Min: %.3f\nMax: %.3f\n",
            report->pfMin, report->pfMax);

    fprintf(fp, "\nTHD Range:\n");
    fprintf(fp, "Min: %.2f %%\nMax: %.2f %%\n",
            report->thdMin, report->thdMax);

    fprintf(fp, "\nStandard Deviation: \n");
    fprintf(fp, "Phase A: %.2f V\nPhase B: %.2f V\nPhase C: %.2f V\n",
    report->stdA, report->stdB, report->stdC);

    fclose (fp);
    return 1;
}