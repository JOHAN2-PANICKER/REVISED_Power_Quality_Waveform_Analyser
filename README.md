# Power Quality Waveform Analyser

#Overview
This project is a C-based Power Quality Waveform Analyser that processes a CSV file containing 3-phase power quality measurements, analysis the waveform data and produces a structured report. 

This project performs waveform analyses and evaluates power quality against engineering standards. 

#Core Analysis
- RMS Voltage (Phase A, B and C)
- Peak-to-Peak Voltage
- DC Offset
- Clipped Sample Count (|V|>= 324.9V)
- Voltage Compliance (EN 50160: 230V +- 10%)

#Extended Analysis
- Frequency Range (Min/Max)
- Frequency Drift (Mean - 50Hz)
- Power Factor Range
- THD Percent Range
- Standard Deviation

#Project Structure
**main.c** - Entry point, handles command-line arguments and orchestrates execution.

**io.c / io.h** - CSV loading, raw data output, results file writing. 

**waveform.c / waveform.h** - All analysis functions: RMS, peak-to-peak, DC offset, clipping, compliance check, and any extensions. 

**CMakeLists.txt** - Build configuration file (C99).

#Build Instructions
**OPTION 1: Using CLion**
1) Open the 'REVISED_POWER_QUALITY_WAVEFORM_ANALYSER' project folder in CLion.
2) Allow CMake to automatically configure the project. 
3) Click Build Icon.

*CLion uses the provided CMakeLists.txt to compile:
- main.c
- io.c
- waveform.c

**OPTION 2: Command-Line (gcc)**
Navigate to the project directory and run:

[gcc -std=c99 -Wall -Wextra -o waveform_analyser main.c io.c waveform.c -lm]

-std=c99 ensures compatibility with code.

-Wall-Wextra enables useful compiler warnings. 

-lm links the math library. 

#Run Instructions
**OPTION 1: Running in CLion**
1) Go to Run -> Edit Configurations.
2) Ensure Program arguments is set to: 'power_quality_log.csv'
3) Ensure the CSV file is located in the working directory (i.e. cmake-build-debug).

**OPTION 2: Command-Line Execution**
1. Use the following command-line execution: './REVISED_Power_Quality_Waveform_Analyser power_quality_log.csv'

#Output Files
After successful compiling and execution, the program produces a 'results.txt' containing voltage analysis results,
compliance checks, frequency/PF/THD ranges, and standard deviation values. 

The program can also output an optional debug 'raw_data.txt' which is a full dataset dump enabled via 
macro in main.c, used to confirm that the program has correctly read and recorded the csv datasheet. 

#DEBUG Features
The following DEBUG features are included in main.c via macros:
- RAW_DATA_TEST
- RMS_VALIDATION_TEST
- PHASE_SHIFT_TEST

Set macro switch values to 1 to enable raw data export, rms validation and phase shift verification. 

#Project Assumptions
Assumes CSV file contains:
- timestamp
- Phase A, B and C voltages
- Current
- Frequency
- Power Factor
- THD
- Sampling Rate ~ 5000 samples/sec
- Nominal Frequency = 50 Hz
- All data types are in correct column order in the csv file.

#Summary
This project demonstrates:
- modular C program design
- pointer-based data handling
- file I/O and CSV parsing
- waveform analysis and engineering compliance checks
- use of bitwise status flags for per-phase health reporting