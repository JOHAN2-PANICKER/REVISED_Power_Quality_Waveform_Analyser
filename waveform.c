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
// Helper function to compute the ranges for Frequency, Power Factor and THD.
static void compute_range (const WaveformSample *samples, int count, WaveformReport *report) {
    const WaveformSample *ptr = samples;
    // Direct data from samples to structure values for frequency range.
    report->freqMin = samples->frequency;
    report->freqMax = samples->frequency;
    //Direct data from samples to structure values for PF range.
    report->pfMin = samples->power_factor;
    report->pfMax = samples->power_factor;
    //Direct data from samples to structure values for THD range.
    report->thdMin = samples->thd_percent;
    report->thdMax = samples->thd_percent;

    //Loop to identify Min and Max Values for above variables.
    double sum = 0.0;

    for (int i=0; i<count; i++, ptr++){
        if (ptr->frequency < report->freqMin) report ->freqMin = ptr->frequency;
        if (ptr->frequency > report->freqMax) report->freqMax = ptr->frequency;

        if (ptr->power_factor < report->pfMin) report->pfMin = ptr->power_factor;
        if (ptr->power_factor > report->pfMax) report->pfMax = ptr->power_factor;

        if (ptr->thd_percent < report->thdMin) report->thdMin = ptr->thd_percent;
        if (ptr->thd_percent > report->thdMax) report->thdMax = ptr ->thd_percent;

        sum+= ptr->frequency;
    }
    // Calculating frequency drift from standard 50 Hz.
    report->freqMean = sum/count;
    report->freqDrift = report->freqMean - 50.0;
}
//Helper Function to compute Standard Deviation.
static double compute_std_dev (const WaveformSample *samples, int count, int phase, double mean) {
    double sum_sq = 0.0;
    const WaveformSample *ptr = samples;

    for (int i=0; i<count; i++, ptr++) {
        double v = getPhaseVoltage(ptr, phase);
        double diff = v - mean;
        sum_sq += diff * diff;
    } return sqrt (sum_sq / count);
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

   // RMS Compliance and Clipping Logic
   report.statusA = 0;
   report.statusB = 0;
   report.statusC = 0;

   //If clipping occurred, set bit 0.
    if (report.clipA >0)
        report.statusA |= STATUS_CLIPPED;
    if (report.clipB >0)
        report.statusB |= STATUS_CLIPPED;
    if (report.clipC >0)
        report.statusC |= STATUS_CLIPPED;

    // If RMS is out of tolerance (outside of 230V +- 10%), set bit 1.
    if (!check_compliance(report.rmsA, 230.0))
        report.statusA |= STATUS_OUT_OF_TOL;
    if (!check_compliance(report.rmsB, 230.0))
        report.statusB |= STATUS_OUT_OF_TOL;
    if (!check_compliance(report.rmsC, 230.0))
        report.statusC |= STATUS_OUT_OF_TOL;

    //Calculate ranges for Frequency, Power Factor and THD percentage.
    compute_range(samples, count, &report);

    //Calculate standard deviation.
    report.stdA = compute_std_dev(samples, count, 0, report.dcA);
    report.stdB = compute_std_dev(samples, count, 1, report.dcB);
    report.stdC = compute_std_dev(samples, count, 2, report.dcC);

    return report;

}

// Debug helper: validate RMS for each Phase.
 void debugRMSValidation ( int count, const WaveformReport *report){
    if (!report || count <= 0) return;
    // Alternative RMS estimate Formula: Vrms =~ Vpp / 2 * sqrt(2)
  double altRmsA = report -> p2pA / (2.0 * sqrt(2.0));
  double altRmsB = report -> p2pA / (2.0 * sqrt(2.0));
  double altRmsC = report -> p2pA / (2.0 * sqrt(2.0));

  //Compare difference between RMS calculated through compute_RMS() and altRMS.
  double diffA = fabs(report->rmsA - altRmsA);
  double diffB = fabs(report->rmsB - altRmsB);
  double diffC = fabs(report->rmsC - altRmsC);

  // allow small difference due to sampling/noise.
  double tolerance = 2.0;

  printf("\nRMS Validation (alt formula: Vrms = Vpp / 2*sqrt(2): \n");

  printf ("Phase A: calc = %.3f V, alt = %.3f V, diff = %.3f V (%s)\n",
          report->rmsA, altRmsA, diffA,
          (diffA <= tolerance) ? "OK" : "CHECK RMS");

  printf ("Phase B: calc = %.3f V, alt = %.3f V, diff = %.3f V (%s)\n",
          report->rmsB, altRmsB, diffB,
          (diffB <= tolerance) ? "OK" : "CHECK RMS");

  printf ("Phase A: calc = %.3f V, alt = %.3f V, diff = %.3f V (%s)\n",
          report->rmsC, altRmsC, diffC,
          (diffC <= tolerance) ? "OK" : "CHECK RMS");
}

//Debug Helper: validate Phase Shift between Phase A and Phase B.
void debugPhaseShiftCheck (const WaveformSample *samples, int count){
    if (!samples || count <= 0) return;
    // Code to ensure only first 100 rows are reviewed for phase shift difference.
    int samplesPerCycle = 100;
    if (count < samplesPerCycle) samplesPerCycle = count;

    int idxA = 0, idxB = 0;

    // loop to find the max voltage for Phase A and Phase B.
    for (int i = 1; i < samplesPerCycle; i++) {
        if (samples[i].phase_A_voltage > samples[idxA].phase_A_voltage)
            idxA = i;

        if (samples[i].phase_B_voltage > samples [idxB].phase_B_voltage)
            idxB = i;
    }
    //to calculate difference in rows between Phase A and Phase B.
    int diff = idxB - idxA;
    if (diff < 0) diff = -diff;
    //Waveform is periodic.Signal repeats every 100 rows per cycle. half-cycle = 50 rows.
    if (diff >50){
        diff = 100 - diff;
    }

    printf("\n Phase Shift Check at 50 Hz: \n");
    printf("Phase A peak row = %d\n", idxA);
    printf("Phase B peak row = %d\n", idxB);
    printf("Row Difference (between Phase A and Phase B) = %d (%s) \n",
           diff,
           (diff >= 31 && diff <= 35) ? "OK (~120 degree phase shift)" : "Phase Shift is not correct");

}
