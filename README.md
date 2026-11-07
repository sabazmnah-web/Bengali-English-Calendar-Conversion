# Bengali-English Universal Calendar System

A comprehensive C++ command-line application designed to handle complex conversions, date arithmetic, and full-year grid generations across multiple calendar systems. It bridges historical and regional calendar standards in a single workspace.

While primarily built for the Bangladeshi diaspora and locals, it is equally useful for global users needing precise Gregorian, Julian, or Islamic (Hijri) date calculations.

## Features

### 1. Single Date Conversion Matrix
Input any standard Gregorian date to view its exact equivalent across multiple localized systems simultaneously:
* **Julian Date** (Accounts for the historical astronomical day gap)
* **Bangla (Bangladesh 1987–2018 Standard)**
* **Bangla (Bangladesh 2019–Present Standard)**
* **Bangla (India Traditional Standard)**
* **Bangla (India Saha Committee / National Standard)**
* **Hijri Calendar** (Calculated using 3 different schemes: Kuwaiti/Microsoft, Kūshyār, and Standard Type II)
* **Day of the week calculation** (Powered by Zeller's Congruence)

### 2. Reverse Conversion (Bangla ➔ English)
Input any Bangla date (with your choice of the 4 regional standards) and instantly find the exact Gregorian date, day of the week, and corresponding Hijri date.

### 3. Date Arithmetic (Time Machine)
* **Age / Date Difference Calculator:** Input two dates to calculate the exact difference in Years, Months, and Days, as well as the total number of days.
* **Add / Subtract Days:** Input a starting date and a number of days (e.g., `+45` or `-30`). The system uses Julian Day Numbers to calculate the exact future or past date across Gregorian and Bangla calendars.

### 4. Full-Year Calendar Generator
Generates clean, properly aligned ASCII monthly calendar grids for a chosen year across four modes:
* **Bangla Year ➔ Bangla Grid** (Using Modern Bangladesh 2019 standards)
* **English Year ➔ Bangla Grid** (Finds the intersecting primary Bangla year)
* **Bangla Year ➔ English Grid** 
* **English Year ➔ English Grid**

## Technical Highlights

* **Absolute Julian Day Number (JDN) Bridge:** All date arithmetic and conversions are routed through the JDN, eliminating floating-point errors and complex leap-year edge cases. 
* **Astronomical Calculations:** The Indian Traditional calendar dynamically calculates the exact solar ingress (Sankranti) into sidereal zodiac signs, accounting for the precession of the equinoxes (Ayanamsa).
* **Robust Input Validation:** The program safely handles invalid dates (like February 30th) and prevents infinite loops on accidental text inputs.

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
