//
// Created by johan on 02/04/2026.
/*All analysis functions: RMS, peak-to-peak, DC offset, clipping,
 * compliance check, and any extensions.*/

#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "waveform.h"

/* Helper: return voltage for selected phase
 * phase=0 -> A, 1 -> B, 2-> C */
static double getPhaseVoltage (const WaveformSample *sample, int phase) {
    if (phase == 0) return sample -> phase_A_voltage;
    if (phase == 1) return sample -> phase_B_voltage;
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

/* Count clipped samples for one phase
 * Clipping threshold from PFE_AC_Waveforms_Reference.pdf:|voltage| >= 324.9 */
static int count_clipped (const WaveformSample *samples, int n, int phase, double limit){
    // variable to count the total clipped samples for each phase.
    int count = 0;
    const WaveformSample *ptr = samples;
    // loop to identify the phase voltage of each sample.
    for (int i = 0; i < n; i++, ptr++){
        double v = getPhaseVoltage (ptr,phase);
        // fabs () to calculate the absolute value of double v.
        if (fabs (v) >= limit) {
            count ++;
        }
    } return count;
}
/* Check EN 50160 compliance for 230 V +- 10%
 * 207 V <= RMS <= 253 v */

static int check_compliance (double rms, double nominal) {
    double lower = nominal * 0.9;
    double upper = nominal * 1.1;

    return (rms >= lower && rms <= upper);
}

// Main analysisWaveform () function.
WaveformReport analyseWaveform (const WaveformSample *samples, int count) {
    WaveformReport report = {0};
    // Error code if samples are empty.
    if (samples == NULL || count <= 0){
        return report;
    }
    // RMS
    report.rmsA = compute_rms (samples, count, 0);
    report.rmsB = compute_rms (samples, count, 1);
    report.rmsC = compute_rms (samples, count, 2);

    // Peak-to-Peak
    report.p2pA = compute_peak_to_peak(samples, count , 0);
    report.p2pB = compute_peak_to_peak(samples, count , 1);
    report.p2pC = compute_peak_to_peak(samples, count , 2);

    //DC Offset
    report.dcA = compute_dc_offset(samples, count, 0);
    report.dcB = compute_dc_offset(samples, count, 1);
    report.dcC = compute_dc_offset(samples, count, 2);

    // Clipped samples per phase
    report.clipA = count_clipped (samples, count, 0, 324.9);
    report.clipB = count_clipped (samples, count, 1, 324.9);
    report.clipC = count_clipped (samples, count, 2, 324.9);

    //Compliance
    report.compliantA = check_compliance(report.rmsA, 230.0);
    report.compliantB = check_compliance(report.rmsB, 230.0);
    report.compliantC = check_compliance(report.rmsC, 230.0);

    return report;

}

// Debug helper: validate RMS for each Phase.
 void debugRMSValidation ( int count, const WaveformReport *report){
    if (!report || count <= 0) return;
    // RMS tolerance band.
    double lower = 207.0;
    double upper = 253.0;

    printf("\n RMS Check (EN 50160 +-10% band):\n");

    printf("Phase A: %.3f V (%s)\n",
           report->rmsA,
           (report->rmsA >= lower && report->rmsA <= upper) ? "OK" : "OUT OF RANGE");

    printf("Phase B: %.3f V (%s)\n",
           report->rmsB,
           (report->rmsB >= lower && report->rmsB <= upper) ? "OK" : "OUT OF RANGE");

    printf("Phase C: %.3f V (%s)\n",
           report->rmsC,
           (report->rmsC >= lower && report->rmsC <= upper) ? "OK" : "OUT OF RANGE");
}

//Debug Helper: validate Phase Shift between Phase A and Phase B.
void debugPhaseShiftCheck (const WaveformSample *samples, int count){
    if (!samples || count <= 0) return;

    int idxA = 0, idxB = 0;

    // loop to find the max voltage for Phase A and Phase B.
    for (int i = 1; i < count; i++) {
        if (samples[i].phase_A_voltage > samples[idxA].phase_A_voltage)
            idxA = i;

        if (samples[i].phase_B_voltage > samples [idxB].phase_B_voltage)
            idxB = i;
    }
    //to calculate difference in rows between Phase A and Phase B.
    int diff = idxB - idxA;
    if (diff < 0) diff = -diff;


    printf("\n Phase Shift Check at 50 Hz: \n");
    printf("Row Difference (between Phase A and Phase B) = %d (%s) \n",
           diff,
           (diff == 33) ? "OK (~120 degree phase shift)" : "Phase Shift is not correct");

}
