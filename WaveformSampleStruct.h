//
// Created by johan on 02/04/2026.
/* Create a struct type in a header file (.h) that holds
 * all 8 fields for a single row of the CSV.*/

#ifndef REVISED_POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORMSAMPLESTRUCT_H
#define REVISED_POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORMSAMPLESTRUCT_H

typedef struct{
    double timestamp;
    double phase_A_voltage;
    double phase_B_voltage;
    double phase_C_voltage;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
}WaveformSample;
#endif //REVISED_POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORMSAMPLESTRUCT_H
