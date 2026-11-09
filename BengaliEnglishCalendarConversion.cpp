#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>

using namespace std;

long long gregorianToJDN(int y, int m, int d) {
    return (1461 * (y + 4800 + (m - 14) / 12)) / 4
         + (367 * (m - 2 - 12 * ((m - 14) / 12))) / 12
         - (3 * ((y + 4900 + (m - 14) / 12) / 100)) / 4
         + d - 32075;
}

void jdnToGregorian(long long jdn, int &y, int &m, int &d) {
    long long l = jdn + 68569;
    long long n = (4 * l) / 146097;
    l = l - (146097 * n + 3) / 4;
    long long i = (4000 * (l + 1)) / 1461001;
    l = l - (1461 * i) / 4 + 31;
    long long j = (80 * l) / 2447;
    d = (int)(l - (2447 * j) / 80);
    l = j / 11;
    m = (int)(j + 2 - 12 * l);
    y = (int)(100 * (n - 49) + i + l);
}

bool isGregorianLeap(int year) {
    if (year % 400 == 0) return true;
    if (year % 100 == 0) return false;
    return year % 4 == 0;
}

bool isJulianLeap(int year) {
    return (year % 4 == 0);
}

string getDayOfWeek(int d, int m, int y) {
    if (m < 3) { m += 12; y--; }
    int q = d, k = y % 100, j = y / 100;
    int h = (q + 13 * (m + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    string days[] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    return days[h];
}

int getDayOfWeekNum(int d, int m, int y) {
    if (m < 3) { m += 12; y--; }
    int q = d, k = y % 100, j = y / 100;
    return (q + 13 * (m + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
}

int getDayOfYear(int d, int m, int y) {
    int mLen[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isGregorianLeap(y)) mLen[2] = 29;
    int dayCount = 0;
    for (int i = 1; i < m; i++) dayCount += mLen[i];
    return dayCount + d;
}

void getGregorianFromDayOfYear(int doy, int y, int &d, int &m) {
    int mLen[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isGregorianLeap(y)) mLen[2] = 29;
    m = 1;
    while (doy > mLen[m]) {
        doy -= mLen[m];
        m++;
    }
    d = doy;
}

bool isValidGregorianDate(int d, int m, int y) {
    if (y < 1 || m < 1 || m > 12 || d < 1) return false;
    int mLen[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isGregorianLeap(y)) mLen[2] = 29;
    return d <= mLen[m];
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void getBanglaMonthLengths(int by, int version, int bLens[]) {
    if (version == 3) {
        int base[] = {31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 30};
        for (int i = 0; i < 12; i++) bLens[i] = base[i];

        if (isGregorianLeap(by + 594)) bLens[11] = 31;
    } else {
        bool isLeap = isGregorianLeap(by + 594);
        if (version == 1 || version == 4) {
            int base[] = {31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 30};
            for (int i = 0; i < 12; i++) bLens[i] = base[i];
            if (isLeap) bLens[10] = 31;
        } else {
            int base[] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 29, 30};
            for (int i = 0; i < 12; i++) bLens[i] = base[i];
            if (isLeap) bLens[10] = 30;
        }
    }
}

void getBanglaDateDetails(int d, int m, int y, int ver, int &bd, string &bm, int &by) {
    string bMonths[] = {"Boishakh", "Jyoishtho", "Asharh", "Shrabon", "Bhadro", "Ashshin",
                        "Kartik", "Ogrohayon", "Poush", "Magh", "Falgun", "Choitro"};
    int doy = getDayOfYear(d, m, y);

    int startOffset = 104;
    if (ver == 3) startOffset = 105;
    if (isGregorianLeap(y)) startOffset++;

    by = (doy >= startOffset) ? (y - 593) : (y - 594);

    int relDays;
    if (doy >= startOffset) {
        relDays = doy - startOffset + 1;
    } else {
        int prevYearDays = isGregorianLeap(y - 1) ? 366 : 365;
        int prevStartOffset = (ver == 3) ? (isGregorianLeap(y - 1) ? 106 : 105) : (isGregorianLeap(y - 1) ? 105 : 104);
        relDays = (prevYearDays - prevStartOffset + 1) + doy;
    }

    int bLens[12];
    getBanglaMonthLengths(by, ver, bLens);

    int idx = 0;
    while (relDays > bLens[idx] && idx < 11) {
        relDays -= bLens[idx];
        idx++;
    }
    bd = relDays;
    bm = bMonths[idx];
}

void banglaToGregorian(int bd, int bmIdx, int by, int ver, int &gd, int &gm, int &gy) {
    int bLens[12];
    getBanglaMonthLengths(by, ver, bLens);

    int accumulated = 0;
    for (int i = 0; i < bmIdx; i++) accumulated += bLens[i];
    accumulated += (bd - 1);

    int startOffset = (ver == 3) ? 105 : 104;
    gy = by + 593;
    if (isGregorianLeap(gy)) startOffset++;

    int doy = startOffset + accumulated;
    int totalDays = isGregorianLeap(gy) ? 366 : 365;

    if (doy > totalDays) {
        doy -= totalDays;
        gy++;
    }

    getGregorianFromDayOfYear(doy, gy, gd, gm);
}

void addDaysToDate(int d, int m, int y, int delta, int &nd, int &nm, int &ny) {
    long long jdn = gregorianToJDN(y, m, d);
    jdn += delta;
    jdnToGregorian(jdn, ny, nm, nd);
}

void dateDifference(int d1, int m1, int y1, int d2, int m2, int y2,
                    int &years, int &months, int &days) {
    long long j1 = gregorianToJDN(y1, m1, d1);
    long long j2 = gregorianToJDN(y2, m2, d2);
    if (j1 > j2) {
        swap(j1, j2);
        swap(d1, d2); swap(m1, m2); swap(y1, y2);
    }

    years = y2 - y1;
    months = m2 - m1;
    days = d2 - d1;

    if (days < 0) {
        months--;
        int prevM = m2 - 1;
        int prevY = y2;
        if (prevM < 1) { prevM = 12; prevY--; }
        int mLen[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
        if (isGregorianLeap(prevY)) mLen[2] = 29;
        days += mLen[prevM];
    }
    if (months < 0) {
        years--;
        months += 12;
    }
}

void printJulianDate(int d, int m, int y) {
    long long julianDays = gregorianToJDN(y, m, d) - 1721423;

    int julianYear = 1;
    while (true) {
        int diy = isJulianLeap(julianYear) ? 366 : 365;
        if (julianDays <= diy) break;
        julianDays -= diy;
        julianYear++;
    }
    int julianMonths[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isJulianLeap(julianYear)) julianMonths[2] = 29;

    int julianMonth = 1;
    while (julianDays > julianMonths[julianMonth]) {
        julianDays -= julianMonths[julianMonth];
        julianMonth++;
    }

    string monthNames[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    cout << "Julian Date            : " << julianDays << " " << monthNames[julianMonth] << " " << julianYear << "\n";
}

void printEnglishMonthGrid(int month, int year) {
    string months[] = {"", "January", "February", "March", "April", "May", "June",
                       "July", "August", "September", "October", "November", "December"};
    int mLen[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isGregorianLeap(year)) mLen[2] = 29;

    cout << "\n      " << months[month] << " " << year << "\n";
    cout << "  S   M   T   W   T   F   S\n";

    int startDay = getDayOfWeekNum(1, month, year);
    int spaces = (startDay + 6) % 7;

    for (int i = 0; i < spaces; i++) cout << "    ";
    for (int day = 1; day <= mLen[month]; day++) {
        cout << setw(3) << day << " ";
        if ((day + spaces) % 7 == 0) cout << "\n";
    }
    cout << "\n";
}

void printBanglaMonthGrid(int bMonthIdx, int bYear, int version) {
    string bMonths[] = {"Boishakh", "Jyoishtho", "Asharh", "Shrabon", "Bhadro", "Ashshin",
                        "Kartik", "Ogrohayon", "Poush", "Magh", "Falgun", "Choitro"};
    cout << "\n      " << bMonths[bMonthIdx] << " " << bYear << " BS\n";
    cout << "  S   M   T   W   T   F   S\n";

    int gregYear = bYear + 593;
    int baseDoy = (version == 3) ? 105 : 104;
    if (isGregorianLeap(gregYear)) baseDoy++;

    int bLens[12];
    getBanglaMonthLengths(bYear, version, bLens);

    int accumulatedDays = 0;
    for (int i = 0; i < bMonthIdx; i++) accumulatedDays += bLens[i];

    int targetDoy = baseDoy + accumulatedDays;
    int totalGregDays = isGregorianLeap(gregYear) ? 366 : 365;
    if (targetDoy > totalGregDays) {
        targetDoy -= totalGregDays;
        gregYear++;
    }

    int gd, gm;
    getGregorianFromDayOfYear(targetDoy, gregYear, gd, gm);
    int startDay = getDayOfWeekNum(gd, gm, gregYear);
    int spaces = (startDay + 6) % 7;

    for (int i = 0; i < spaces; i++) cout << "    ";
    for (int day = 1; day <= bLens[bMonthIdx]; day++) {
        cout << setw(3) << day << " ";
        if ((day + spaces) % 7 == 0) cout << "\n";
    }
    cout << "\n";
}

void convertGregorianToBangla() {
    int d, m, y;
    cout << "\nEnter Gregorian Date (DD MM YYYY): ";
    if (!(cin >> d >> m >> y)) {
        clearInput();
        cout << "Invalid input format.\n";
        return;
    }
    if (!isValidGregorianDate(d, m, y)) { cout << "Invalid Date.\n"; return; }

    string mNames[] = {"", "January", "February", "March", "April", "May", "June",
                       "July", "August", "September", "October", "November", "December"};
    cout << "\n================ DATE CONVERSION RESULTS ================\n";
    cout << left << setw(23) << "Gregorian Date" << ": " << d << " " << mNames[m] << " " << y << "\n";

    printJulianDate(d, m, y);

    cout << left << setw(23) << "Day of the Week" << ": " << getDayOfWeek(d, m, y) << "\n";
    cout << "---------------------------------------------------------\n";

    int bd; string bm; int by;
    getBanglaDateDetails(d, m, y, 1, bd, bm, by);
    cout << left << setw(23) << "Bangla (BD 1987-2018)" << ": " << bd << " " << bm << " " << by << " BS\n";
    getBanglaDateDetails(d, m, y, 2, bd, bm, by);
    cout << left << setw(23) << "Bangla (BD 2019-Pres)" << ": " << bd << " " << bm << " " << by << " BS\n";
    getBanglaDateDetails(d, m, y, 3, bd, bm, by);
    cout << left << setw(23) << "Bangla (India Trad)" << ": " << bd << " " << bm << " " << by << " BS\n";
    getBanglaDateDetails(d, m, y, 4, bd, bm, by);
    cout << left << setw(23) << "Bangla (India Saha)" << ": " << bd << " " << bm << " " << by << " BS\n";

    cout << "=========================================================\n";
}

void convertBanglaToGregorian() {
    cout << "\nEnter Bangla day: ";
    int bd; cin >> bd;
    cout << "Enter Bangla month number (1=Boishakh ... 12=Choitro): ";
    int bm; cin >> bm;
    cout << "Enter Bangla year (BS): ";
    int by; cin >> by;
    cout << "Version (1=BD old, 2=BD 2019+, 3=India Trad, 4=India Saha) [default 2]: ";

    int ver = 2;
    string verStr;
    getline(cin >> ws, verStr);
    if (!verStr.empty()) {
        try { ver = stoi(verStr); }
        catch (...) { ver = 2; }
    }

    if (bm < 1 || bm > 12 || bd < 1 || by < 1 || ver < 1 || ver > 4) {
        cout << "Invalid input (Check date bounds and version 1-4).\n";
        return;
    }

    int gd, gm, gy;
    banglaToGregorian(bd, bm - 1, by, ver, gd, gm, gy);

    string mNames[] = {"", "January", "February", "March", "April", "May", "June",
                       "July", "August", "September", "October", "November", "December"};
    cout << "\n================ DATE CONVERSION RESULTS ================\n";
    cout << left << setw(23) << "Corresponding Gregorian" << ": " << gd << " " << mNames[gm] << " " << gy << "\n";
    cout << left << setw(23) << "Day of Week" << ": " << getDayOfWeek(gd, gm, gy) << "\n";
    cout << "=========================================================\n";
}

void runMergedConversionMenu() {
    cout << "\n--- Date Conversion Options ---\n";
    cout << "2.1 Gregorian → Bangla\n";
    cout << "2.2 Bangla → Gregorian\n";
    cout << "Select Direction (2.1 or 2.2): ";
    string subOpt;
    cin >> subOpt;

    if (subOpt == "2.1" || subOpt == "1") {
        convertGregorianToBangla();
    }
    else if (subOpt == "2.2" || subOpt == "2") {
        convertBanglaToGregorian();
    }
    else {
        cout << "Invalid Selection.\n";
    }
}

void runAgeCalculatorOption() {
    cout << "\n--- Age / Date Difference Calculator ---\n";
    cout << "Enter first date  (DD MM YYYY) [e.g. birth]: ";
    int d1, m1, y1;
    if (!(cin >> d1 >> m1 >> y1) || !isValidGregorianDate(d1, m1, y1)) {
        clearInput();
        cout << "Invalid first date.\n";
        return;
    }
    cout << "Enter second date (DD MM YYYY) [e.g. today]: ";
    int d2, m2, y2;
    if (!(cin >> d2 >> m2 >> y2) || !isValidGregorianDate(d2, m2, y2)) {
        clearInput();
        cout << "Invalid second date.\n";
        return;
    }

    int years, months, days;
    dateDifference(d1, m1, y1, d2, m2, y2, years, months, days);

    long long j1 = gregorianToJDN(y1, m1, d1);
    long long j2 = gregorianToJDN(y2, m2, d2);
    long long totalDays = abs(j2 - j1);

    cout << "\nExact difference: " << years << " years, " << months << " months, " << days << " days\n";
    cout << "Total days      : " << totalDays << " days\n";
}

void runAddSubtractDaysOption() {
    cout << "\n--- Add / Subtract Days ---\n";
    cout << "Enter starting Gregorian date (DD MM YYYY): ";
    int d, m, y;
    if (!(cin >> d >> m >> y) || !isValidGregorianDate(d, m, y)) {
        clearInput();
        cout << "Invalid date.\n";
        return;
    }
    cout << "Enter number of days to add (negative to subtract): ";
    int delta;
    if (!(cin >> delta)) {
        clearInput();
        cout << "Invalid number.\n";
        return;
    }

    int nd, nm, ny;
    addDaysToDate(d, m, y, delta, nd, nm, ny);

    string mNames[] = {"", "January", "February", "March", "April", "May", "June",
                       "July", "August", "September", "October", "November", "December"};

    cout << "\nResult Gregorian : " << nd << " " << mNames[nm] << " " << ny << "\n";
    cout << "Day of Week      : " << getDayOfWeek(nd, nm, ny) << "\n";

    int bd; string bm; int by;
    getBanglaDateDetails(nd, nm, ny, 2, bd, bm, by);
    cout << "Bangla (BD 2019+): " << bd << " " << bm << " " << by << " BS\n";
}

void runYearCalendarOption() {
    cout << "\n--- Year-Calendar Options ---\n";
    cout << "1.1 Enter Bangla Year  → Show Bangla Year Calendar\n";
    cout << "1.2 Enter English Year → Show Bangla Year Calendar\n";
    cout << "1.3 Enter Bangla Year  → Show English Year Calendar\n";
    cout << "1.4 Enter English Year → Show English Year Calendar\n";
    cout << "Select Sub-option (1.1 - 1.4): ";
    string subOpt;
    cin >> subOpt;

    int inputYear;
    cout << "Enter Year: ";
    if (!(cin >> inputYear)) {
        clearInput();
        cout << "Invalid input format.\n";
        return;
    }

    if (subOpt == "1.1") {
        cout << "\nGenerating Bangla Calendar for " << inputYear << " BS (Bangladesh Modern 2019 Rules):\n";
        for (int m = 0; m < 12; m++) printBanglaMonthGrid(m, inputYear, 2);
    }
    else if (subOpt == "1.2") {
        int convertedBanglaYear = inputYear - 593;
        cout << "\nEnglish Year " << inputYear << " maps primarily to Bangla Year " << convertedBanglaYear << " BS.\n";
        for (int m = 0; m < 12; m++) printBanglaMonthGrid(m, convertedBanglaYear, 2);
    }
    else if (subOpt == "1.3") {
        int convertedGregorianYear = inputYear + 593;
        cout << "\nBangla Year " << inputYear << " BS intersects heavily with English Year " << convertedGregorianYear << ".\n";
        for (int m = 1; m <= 12; m++) printEnglishMonthGrid(m, convertedGregorianYear);
    }
    else if (subOpt == "1.4") {
        cout << "\nGenerating English Year Calendar for " << inputYear << ":\n";
        for (int m = 1; m <= 12; m++) printEnglishMonthGrid(m, inputYear);
    }
    else {
        cout << "Invalid Sub-option choice.\n";
    }
}

int main() {
    while (true) {
        cout << "\n=========================================================\n";
        cout << "         Bengali – English Calendar Conversion System\n";
        cout << "=========================================================\n";
        cout << "1. Year-Calendar Generator\n";
        cout << "2. Date Conversion (Gregorian ↔ Bangla)\n";
        cout << "3. Age / Date Difference Calculator\n";
        cout << "4. Add / Subtract Days from a Date\n";
        cout << "5. Exit System\n";
        cout << "Choose Option: ";
        int choice;
        if (!(cin >> choice)) {
            clearInput();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            runYearCalendarOption();
        } else if (choice == 2) {
            runMergedConversionMenu();
        } else if (choice == 3) {
            runAgeCalculatorOption();
        } else if (choice == 4) {
            runAddSubtractDaysOption();
        } else if (choice == 5) {
            break;
        } else {
            cout << "Invalid Option. Try again.\n";
        }
    }
    return 0;
}
