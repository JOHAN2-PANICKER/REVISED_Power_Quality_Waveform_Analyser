//
// Created by johan on 02/04/2026.
/*All analysis functions: RMS, peak-to-peak, DC offset, clipping,
 * compliance check, and any extensions.*/

#include <stddef.h>
#include <math.h>
#include "waveform.h"

WaveformReport analyseWaveform (const WaveformSample *samples, int count) {
    //Create a report structure and initialise all values to 0.
    WaveformReport report = {0};
    //Safety Check for invalid input e.g. if there are no samples.
    if (samples == NULL || count <= 0) {
        return report;
    }
    //Running sums of square voltage for RMS calculation.
    double sumSqA = 0.0, sumSqB = 0.0, sunSqC = 0.0;

    //Running sums of voltages for DC offset calculation.
    double sumA = 0.0, sumB = 0.0, sumC = 0.0;

    //start identifying min/max values using the first sample in the csv datasheet.
    double maxA = samples->phase_A_voltage;
    double minA = samples->phase_A_voltage;
    double maxB = samples->phase_B_voltage;
    double minB = samples->phase_B_voltage;
    double maxC = samples->phase_C_voltage;
    double minC = samples->phase_C_voltage;

    //pointer-based access to traverse through sample array.
    const WaveformSample *ptr = samples;

    //Loop through every sample in the dataset.
    for (int i=0; i < count; i++, ptr++) {
        //Store each phase voltage in a temporary variable.
        double a = ptr -> phase_A_voltage;
        double b = ptr -> phase_B_voltage;
        double c = ptr -> phase_C_voltage;
    }
}