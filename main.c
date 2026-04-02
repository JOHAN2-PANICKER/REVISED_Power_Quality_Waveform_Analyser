/*Entry point only. Handles command-line arguments and
 * orchestrates function calls. Should not contain any analysis logic*/
#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "waveform.h"

int main(void) {
    FILE*fp = fopen("power_quality_log.csv","r");
    if (fp == NULL) {
        printf ("Error opening CSV file.\n");
        return 1;
    }

}
