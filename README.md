# Bengali-English Universal Calendar System

A comprehensive C++ command-line application designed to handle complex conversions, date arithmetic, and full-year grid generations across multiple calendar systems. It bridges historical and regional calendar standards in a single workspace.

While primarily built for the Bangladeshi diaspora and locals, it is equally useful for global users needing precise Gregorian, Julian, or regional Bangla date calculations.

## Features

### 1. Date Conversion (Gregorian ↔ Bangla)
A fully integrated, bi-directional conversion tool utilizing a unified sub-menu:

**Gregorian ➔ Bangla (Option 2.1)**
Input any standard Gregorian date to view its exact equivalent across multiple localized systems simultaneously:
* **Julian Date** (Accurately accounts for the historical astronomical day gap)
* **Bangla (Bangladesh 1987–2018 Standard)**
* **Bangla (Bangladesh 2019–Present Standard)**
* **Bangla (India Traditional Standard)**
* **Bangla (India Saha Committee / National Standard)**
* **Day of the week calculation** (Powered by Zeller's Congruence)

**Bangla ➔ Gregorian (Option 2.2)**
Input any Bangla date along with your choice of the 4 regional standards using a simplified single-line input (e.g., `23 2 1480 2`) to instantly find the exact Gregorian date and day of the week.

### 2. Date Arithmetic (Time Machine)
* **Age / Date Difference Calculator:** Input two dates to calculate the exact difference in Years, Months, and Days, as well as the total number of days.
* **Add / Subtract Days:** Input a starting date and a number of days (e.g., `+45` or `-30`). The system uses Julian Day Numbers to calculate the exact future or past date across Gregorian and Bangla calendars.

### 3. Full-Year Calendar Generator
Generates clean, properly aligned ASCII monthly calendar grids for a chosen year across four modes:
* **Bangla Year ➔ Bangla Grid** (Using Modern Bangladesh 2019 standards)
* **English Year ➔ Bangla Grid** (Finds the intersecting primary Bangla year)
* **Bangla Year ➔ English Grid** 
* **English Year ➔ English Grid**

## Technical Highlights

* **Absolute Julian Day Number (JDN) Bridge:** All date arithmetic and conversions are routed through the JDN, eliminating floating-point errors and complex leap-year edge cases.
* **Historical Julian Accuracy:** Correctly handles the 1-to-13 day gap between the Gregorian and Julian calendars depending on the century, using the exact historical epoch offsets.
* **Mathematical Regional Models:** The India Traditional and Saha standards are handled using precise mathematical year-length models (5 months of 31 days, 7 months of 30 days, plus leap-day variations) to ensure zero drift.
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
g++ -o calendar_system main.cpp
./calendar_system
