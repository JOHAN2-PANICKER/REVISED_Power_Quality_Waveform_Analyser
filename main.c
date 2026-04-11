/*Entry point only. Handles command-line arguments and
 * orchestrates function calls. Should not contain any analysis logic*/
#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "waveform.h"

#define RAW_DATA_TEST 0
#define RMS_VALIDATION_TEST 0
#define PHASE_SHIFT_TEST 0

int main(int argc, char*argv[]){ /*argc=argument count (how many strings passed),
 * argv= argument vector (array of character points, each entry is one string).
 * argv[1] is the csv filename.*/

    int count = 0;// count to store number of data rows in the CSV file.

    //Check if filename is provided.
    if (argc <2){ // check to ensure argv[0] has program name and argv[1] has filename.
        // Error message prompting the correct layout for the CSV file.
        printf("Usage: %s <csv_file>\n Add datasheet csv file into 'cmake-build-debug' folder.",argv[0]);
        return 1;
    }

    WaveformSample *samples = loadCSV(argv[1], &count);
    if (samples == NULL){
        printf("Failed to load CSV file in main.c.\n");
        return 2;
    }

    WaveformReport report = analyseWaveform (samples, count);

    if (!writeResults ("results.txt", &report)) {
        printf("Failed to write results file in main.\n");
        free(samples);
        return 3;
    }else{
        printf ("results.txt created successfully.\n");
    }
#if RAW_DATA_TEST
    // This is a debug to check and confirm that main.c has successfully read and recorded the csv file data.
    //Error code if raw data file is not created.
    if (!writeRawData("raw_data.txt", samples, count)){
        printf ("Failed to create raw_data.txt\n");
        free(samples);
        return 3;

        } else{
        printf("raw_data.txt created successfully.\n");
    }
#endif

#if RMS_VALIDATION_TEST
    debugRMSValidation(count, &report);
#endif

#if PHASE_SHIFT_TEST
    debugPhaseShiftCheck(samples, count);
#endif

    free (samples);
    return 0;
}
