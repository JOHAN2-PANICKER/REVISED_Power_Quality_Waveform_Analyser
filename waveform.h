//
// Created by johan on 02/04/2026.
//

#ifndef REVISED_POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H
#define REVISED_POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H
#include "WaveformSampleStruct.h"
// Structure for Waveform Report.

typedef struct {
    double rmsA, rmsB, rmsC;
    double p2pA, p2pB, p2pC;
    double dcA, dcB, dcC;
    int clipA, clipB, clipC;

    int compliantA, compliantB, compliantC;

    double freqMin, freqMax;
    double pfMin, pfMax;
    double thdMin, thdMax;
}WaveformReport;

WaveformReport analyseWaveform (const WaveformSample *samples, int count);
#endif //REVISED_POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H
