//
// Created by johan on 02/04/2026.
//

#ifndef REVISED_POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H
#define REVISED_POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H
#include "WaveformSampleStruct.h"
#include <stdint.h>
#define STATUS_CLIPPED (1u << 0) // Binary 1. (00000001)
#define STATUS_OUT_OF_TOL (1u << 1) //Binary 2. (00000010)

// Structure for Waveform Report.

typedef struct {
    double rmsA, rmsB, rmsC;
    double p2pA, p2pB, p2pC;
    double dcA, dcB, dcC;
    int clipA, clipB, clipC;

    double freqMin, freqMax, freqMean, freqDrift;
    double pfMin, pfMax;
    double thdMin, thdMax;
    double stdA, stdB, stdC;

    uint8_t statusA;
    uint8_t statusB;
    uint8_t statusC;

}WaveformReport;

WaveformReport analyseWaveform (const WaveformSample *samples, int count);

// Temporary debug helpers.
void debugRMSValidation (int count, const WaveformReport *report);
void debugPhaseShiftCheck (const WaveformSample *samples, int count);

#endif //REVISED_POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H
