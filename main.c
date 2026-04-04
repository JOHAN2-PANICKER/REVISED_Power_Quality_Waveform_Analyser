/*Entry point only. Handles command-line arguments and
 * orchestrates function calls. Should not contain any analysis logic*/
#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "waveform.h"

#define RAW_DATA_TEST 1

int main(int argc, char*argv[]){ /*argc=argument count (how many strings passed),
 * argv= argument vector (array of character points, each entry is one string).
 * argv[1] is the csv filename.*/

    int count = 0;// count to store number of data rows in the CSV file.

    //Check if filename is provided.
    if (argc <2){ // check to ensure argv[0] has program name and argv[1] has filename.
        printf("Usage: %s <csv_file>\n",argv[0]); // Error message prompting the correct layout for the CSV file.
        return 1;
    }

    WaveformSample *samples = loadCSV(argv[1], &count);
    if (samples == NULL){
        printf("Failed to load CSV file in main.c.\n");
        return 2;
    }
#if RAW_DATA_TEST
    printf("\n--- DEBUG: Loaded %d samples ---\n",count);

    WaveformSample *ptr = samples;

    for (int i=0; i < count; i++, ptr++){
        printf("Sample %d:\n",i);
        printf("Time: %.3f\n",ptr->timestamp);
        printf("Phase A Voltage: %.3f\n",ptr->phase_A_voltage);
        printf("Phase B Voltage: %.3f\n",ptr->phase_B_voltage);
        printf("Phase C Voltage: %.3f\n", ptr->phase_C_voltage);
        printf("Current: %.3f\n", ptr->line_current);
        printf("Freq: %.3f\n", ptr->frequency);
        printf("PF: %.3f\n", ptr->power_factor);
        printf("THD: %.3f\n", ptr->thd_percent);
        printf("\n");
    }

#endif
}
