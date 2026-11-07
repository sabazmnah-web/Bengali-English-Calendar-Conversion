#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <limits>

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

double dateToJD(int year, int month, int day) {
    if (month <= 2) { year -= 1; month += 12; }
    int A = year / 100;
    int B = 2 - A + (A / 4);
    return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + B - 1524.5;
}

double getSunTropicalLongitude(double jd) {
    double n = jd - 2451545.0;
    double L = fmod(280.460 + 0.9856474 * n, 360.0);
    double g = fmod(357.528 + 0.9856003 * n, 360.0) * M_PI / 180.0;
    double lambda = L + 1.915 * sin(g) + 0.020 * sin(2 * g);
    return fmod(lambda + 360.0, 360.0);
}

double getAyanamsa(double jd) {
    double year = 2000.0 + (jd - 2451545.0) / 365.25;
    return 23.85 + 0.01397 * (year - 2000.0);
}

double getSiderealLongitude(double jd) {
    double trop = getSunTropicalLongitude(jd);
    double ay = getAyanamsa(jd);
    double sid = trop - ay;
    if (sid < 0) sid += 360.0;
    return sid;
}

double findSankranti(double targetLong, double startJD) {
    double jd = startJD;
    double prevLong = getSiderealLongitude(jd);
    for (int i = 0; i < 40; i++) {
        jd += 1.0;
        double currLong = getSiderealLongitude(jd);
        bool crossed = false;

        if (targetLong == 0) {
            if (prevLong > 300 && currLong < 60) crossed = true;
        } else {
            if (prevLong < targetLong && currLong >= targetLong) crossed = true;
        }

        if (crossed) {
            double low = jd - 1.0, high = jd;
            for (int k = 0; k < 20; k++) {
                double mid = (low + high) / 2.0;
                double midLong = getSiderealLongitude(mid);
                if (targetLong == 0) {
                    if (midLong > 300) low = mid; else high = mid;
                } else {
                    if (midLong < targetLong) low = mid; else high = mid;
                }
            }
            return high;
        }
        prevLong = currLong;
    }
    return startJD + 30;
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

void printJulianDate(int d, int m, int y) {
    int gregMonths[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isGregorianLeap(y)) gregMonths[2] = 29;
    long long totalDays = 0;
    for (int i = 1; i < y; i++) totalDays += isGregorianLeap(i) ? 366 : 365;
    for (int i = 1; i < m; i++) totalDays += gregMonths[i];
    totalDays += d;

    long long julianDays = totalDays + 2;
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

void getBanglaMonthLengths(int by, int version, int bLens[]) {
    if (version == 3) {
        double startJD = dateToJD(by + 593, 4, 10);
        double targetLong = 0.0;
        double prevJD = findSankranti(0.0, startJD);

        for (int i = 0; i < 12; i++) {
            targetLong = (i + 1) * 30.0;
            if (targetLong >= 360.0) targetLong -= 360.0;

            double nextJD = findSankranti(targetLong, prevJD + 28.0);

            int startDay = (int)floor(prevJD + 0.5);
            int nextDay = (int)floor(nextJD + 0.5);

            bLens[i] = nextDay - startDay;
            prevJD = nextJD;
        }
    } else {
        bool isLeap = isGregorianLeap(by + 594);
        if (version == 1 || version == 4) {
            int base[] = {31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 30};
            for(int i=0; i<12; i++) bLens[i] = base[i];
            if (isLeap) bLens[10] = 31;
        } else if (version == 2) {
            int base[] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 29, 30};
            for(int i=0; i<12; i++) bLens[i] = base[i];
            if (isLeap) bLens[10] = 30;
        }
    }
}

void getBanglaDateDetails(int d, int m, int y, int ver, int &bd, string &bm, int &by) {
    string bMonths[] = {"Boishakh", "Jyoishtho", "Asharh", "Shrabon", "Bhadro", "Ashshin", "Kartik", "Ogrohayon", "Poush", "Magh", "Falgun", "Choitro"};
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

bool isValidDate(int d, int m, int y) {
    if (y < 1 || m < 1 || m > 12 || d < 1) return false;
    int mLen[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isGregorianLeap(y)) mLen[2] = 29;
    return d <= mLen[m];
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void runDateConversionOption() {
    int d, m, y;
    cout << "\nEnter Gregorian Date (DD MM YYYY): ";
    if (!(cin >> d >> m >> y)) {
        clearInput();
        cout << "Invalid input format.\n";
        return;
    }
    if (!isValidDate(d, m, y)) { cout << "Invalid Date.\n"; return; }

    string mNames[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    cout << "\n================ DATE CONVERSION RESULTS ================\n";
    cout << "Gregorian Date         : " << d << " " << mNames[m] << " " << y << "\n";
    printJulianDate(d, m, y);
    cout << "Day of the Week        : " << getDayOfWeek(d, m, y) << "\n";
    cout << "---------------------------------------------------------\n";

    int bd; string bm; int by;
    getBanglaDateDetails(d, m, y, 1, bd, bm, by);
    cout << "Bangla (BD 1987-2018)  : " << bd << " " << bm << " " << by << " BS\n";
    getBanglaDateDetails(d, m, y, 2, bd, bm, by);
    cout << "Bangla (BD 2019-Pres)  : " << bd << " " << bm << " " << by << " BS\n";
    getBanglaDateDetails(d, m, y, 3, bd, bm, by);
    cout << "Bangla (India Trad)    : " << bd << " " << bm << " " << by << " BS\n";
    getBanglaDateDetails(d, m, y, 4, bd, bm, by);
    cout << "Bangla (India Saha)    : " << bd << " " << bm << " " << by << " BS\n";
    cout << "=========================================================\n";
}

void printEnglishMonthGrid(int month, int year) {
    string months[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
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
    string bMonths[] = {"Boishakh", "Jyoishtho", "Asharh", "Shrabon", "Bhadro", "Ashshin", "Kartik", "Ogrohayon", "Poush", "Magh", "Falgun", "Choitro"};
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

void runYearCalendarOption() {
    cout << "\n--- Year-Calendar Options ---\n";
    cout << "1.1 Enter Bangla Year  -> Show Bangla Year Calendar\n";
    cout << "1.2 Enter English Year -> Show Bangla Year Calendar\n";
    cout << "1.3 Enter Bangla Year  -> Show English Year Calendar\n";
    cout << "1.4 Enter English Year -> Show English Year Calendar\n";
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

    if (subOpt == "1.1" || subOpt == "1") {
        cout << "\nGenerating Bangla Calendar for " << inputYear << " BS (Bangladesh Modern 2019 Rules):\n";
        for (int m = 0; m < 12; m++) printBanglaMonthGrid(m, inputYear, 2);
    }
    else if (subOpt == "1.2" || subOpt == "2") {
        int convertedBanglaYear = inputYear - 593;
        cout << "\nEnglish Year " << inputYear << " maps primarily to Bangla Year " << convertedBanglaYear << " BS.\n";
        for (int m = 0; m < 12; m++) printBanglaMonthGrid(m, convertedBanglaYear, 2);
    }
    else if (subOpt == "1.3" || subOpt == "3") {
        int convertedGregorianYear = inputYear + 593;
        cout << "\nBangla Year " << inputYear << " BS intersects heavily with English Year " << convertedGregorianYear << ".\n";
        for (int m = 1; m <= 12; m++) printEnglishMonthGrid(m, convertedGregorianYear);
    }
    else if (subOpt == "1.4" || subOpt == "4") {
        cout << "\nGenerating English Year Calendar for " << inputYear << ":\n";
        for (int m = 1; m <= 12; m++) printEnglishMonthGrid(m, inputYear);
    }
    else {
        cout << "Invalid Sub-option choice.\n";
    }
}

int main() {
    while (true) {
        cout << "\n=============================================\n";
        cout << "         Bengali English Calendar Conversion SYSTEM           \n";
        cout << "=============================================\n";
        cout << "1. Year-Calendar Generator (Full Range View)\n";
        cout << "2. Single Date Conversion Matrix (4 Regional Stds)\n";
        cout << "3. Exit System\n";
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
            runDateConversionOption();
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid Option. Try again.\n";
        }
    }
    return 0;
}
