# Bengali-English Universal Calendar System

A comprehensive C++ command-line application designed to handle complex conversions and full-year grid generations across multiple calendar systems. It bridges historical and regional calendar standards in a single workspace.

## Features

### 1. Full-Year Calendar Generator
Generates clean monthly calendar grids for a chosen year across four modes:
* **Bangla Year ➔ Bangla Grid** (Using Modern Bangladesh 2019 standards)
* **English Year ➔ Bangla Grid** (Finds the intersecting primary Bangla year)
* **Bangla Year ➔ English Grid** * **English Year ➔ English Grid**

### 2. Single Date Conversion Matrix
Input any standard Gregorian date to view its exact equivalent across 4 localized systems simultaneously:
* **Julian Date** (Accounts for the historical astronomical day gap)
* **Bangla (Bangladesh 1987–2018 Standard)**
* **Bangla (Bangladesh 2019–Present Standard)**
* **Bangla (India Traditional Standard)**
* **Bangla (India Saha Committee / National Standard)**
* **Day of the week calculation** (Powered by Zeller's Congruence)

## How to Compile and Run

### Code::Blocks Setup
1. Open **Code::Blocks**.
2. Go to `File -> New -> Project` and select **Console Application**.
3. Name the project `UniversalCalendarSystem`.
4. Replace the contents of `main.cpp` with the source code provided in this repository.
5. Click **Build and Run** (or press `F9`).

### Terminal / Command Line
Compile using any standard GCC/G++ compiler:
```bash
g++ -o calendar_system BengaliEnglishCalendarConversion.cpp
./calendar_system
