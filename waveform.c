//
// Created by johan on 02/04/2026.
/*All analysis functions: RMS, peak-to-peak, DC offset, clipping,
 * compliance check, and any extensions.*/

#include <stddef.h>
#include <math.h>
#include "waveform.h"

/* Helper: return voltage for selected phase
 * phase=0 -> A, 1 -> B, 2-> C */
static double getPhaseVoltage (const WaveformSample *sample, int phase) {
    if (phase == 0) return sample -> phase_A_voltage;
    if (phase == 1) return sample -> phase_C_voltage;
    return sample -> phase_C_voltage;
}

/*Compute RMS voltage for one phase
 * RMS = sqrt (1/N) * sum(V^2) */
static double compute_rms (const WaveformSample *samples, int n, int phase) {
    double sum_sq = 0.0; // variable to record the sum squared of all voltages.
    const WaveformSample *ptr = samples;

    // Loop to collate the sum of all squared voltage values.
    for (int i = 0; i < n; i++, ptr++) {
        double v = getPhaseVoltage(ptr, phase);
        sum_sq += v * v;
    }
    return sqrt(sum_sq / n);

}

    /* Compute peak-to-peak voltage for one phase
     * Vpp = Vmax - Vmin */
static double compute_peak_to_peak (const WaveformSample *samples, int n, int phase) {
     const WaveformSample *ptr = samples;
     double max_v = getPhaseVoltage (ptr,phase);
     double min_v = getPhaseVoltage (ptr, phase);

     //Loop to identify the min/max voltage value.
     for (int i = 0; i < n; i++, ptr++) {
         double v = getPhaseVoltage (ptr,phase);

         if (v > max_v) max_v = v;
         if (v < min_v) min_v = v;
     }
     return max_v - min_v;
}

/* Compute DC offset for one phase
 * DC = (1/N) * sum(V) */
static double compute_dc_offset(const WaveformSample *samples, int  n,int phase) {
    double sum =0.0;
    const WaveformSample *ptr = samples;

    // Loop to collate the sum of all standard voltage values,.
    for (int i = 0; i < n; i++, ptr++) {
        sum += getPhaseVoltage (ptr,phase);
    }return sum / n;
}