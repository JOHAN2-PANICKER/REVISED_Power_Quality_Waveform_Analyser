cmake_minimum_required(VERSION 3.21)
project(REVISED_Power_Quality_Waveform_Analyser C)

set(CMAKE_C_STANDARD 99)

add_executable(REVISED_Power_Quality_Waveform_Analyser main.c WaveformSampleStruct.h waveform.c waveform.h io.c io.h)
