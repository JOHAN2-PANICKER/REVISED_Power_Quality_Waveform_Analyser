//
// Created by johan on 02/04/2026.
//

#ifndef REVISED_POWER_QUALITY_WAVEFORM_ANALYSER_IO_H
#define REVISED_POWER_QUALITY_WAVEFORM_ANALYSER_IO_H

#include "WaveformSampleStruct.h"
#include "waveform.h"

WaveformSample*loadCSV(const char*filename, int*count);

int writeRawData (const char *filename, const WaveformSample *samples, int count);

int writeResults (const char *filename, const WaveformReport *report);

#endif //REVISED_POWER_QUALITY_WAVEFORM_ANALYSER_IO_H
