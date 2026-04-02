/*Entry point only. Handles command-line arguments and
 * orchestrates function calls. Should not contain any analysis logic*/
#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "waveform.h"

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
        return 1;
    }


}
