//
// Created by Andrei on 27-Oct-26.
//

#include "Stint.h"
#include "Statistics.h"

namespace {
    bool isCleanLap(const Lap& lap, double medianPace) {
        double outlierThreshold = 1.07;
        if (lap.isRepresentative() && lap.fuelCorrectedDuration() <= medianPace * outlierThreshold) {
            return true;
        }
        return false;
    }
}

Stint::Stint(const std::string& compound_, int tyreAgeAtStart_, int startLap_, int endLap_) :
    compound(compound_), tyreAgeAtStart(tyreAgeAtStart_), startLap(startLap_), endLap(endLap_) {}

double Stint::medianPace() const {
    std::vector<double> lapTimes;
    for (const Lap& lap : laps) {
        if (lap.isRepresentative()) {
            lapTimes.push_back(lap.fuelCorrectedDuration());
        }
    }
    return stats::median(lapTimes);
}

int Stint::tyreAgeAt(const Lap &lap) const {
    return tyreAgeAtStart + (lap.getLapNumber() - startLap);
}

bool Stint::isSlick() const {
    if (compound == "SOFT" || compound == "MEDIUM" || compound == "HARD") {
        return true;
    }
    return false;
}

bool Stint::isLapInStint(int lapNumber) const {
    return lapNumber >= startLap && lapNumber <= endLap;
}

void Stint::addLap(const Lap& lap) {
    laps.push_back(lap);
}

int Stint::lastLapNumber() const {
    return endLap;
}

int Stint::lapCount() const {
    return endLap - startLap + 1;
}

double Stint::averagePace() const {
    double totalPace = 0, median = medianPace();
    int lapCountInStint = 0;
    for (const Lap& lap : laps) {
        if (isCleanLap(lap, median)) {
            totalPace += lap.getLapDuration();
            lapCountInStint++;
        }
    }
    if (lapCountInStint > 0) {
        return totalPace / lapCountInStint;
    }
    return 0;
}

double Stint::bestSectorInStint(int i) const {
    double bestSector = -1;
    for (const Lap& lap : laps) {
        double sector = lap.getSector(i);
        if (lap.isRepresentative() && (bestSector < 0 || sector < bestSector) && sector > 0) {
            bestSector = sector;
        }
    }
    return bestSector;
}

const Lap *Stint::fastestLap() const {
    const Lap* fastest = nullptr;
    for (const Lap& lap : laps) {
        if (lap.isRepresentative() && (fastest == nullptr || lap.isFasterThan(*fastest))) {
            fastest = &lap;
        }
    }
    return fastest;
}

int Stint::topSpeed() const {
    int top = 0;
    for (const Lap& lap : laps) {
        if (lap.getSpeedTrap() > top) {
            top = lap.getSpeedTrap();
        }
    }
    return top;
}

double Stint::tyreDeg() const {
    if (!isSlick()) {
        return 0;
    }
    double median = medianPace();
    int cleanLapCount = 0, minLapsForDeg = 5;
    double sumX = 0, sumY = 0, sumXY = 0, sumXX = 0;
    for (const Lap& lap : laps) {
        if (isCleanLap(lap, median)) {
            cleanLapCount++;
            int x = tyreAgeAt(lap);
            double y = lap.fuelCorrectedDuration();
            sumX += x;
            sumY += y;
            sumXY += x * y;
            sumXX += x * x;
        }
    }
    if (cleanLapCount < minLapsForDeg) {
        return 0;
    }
    return (cleanLapCount * sumXY - sumX * sumY) / (cleanLapCount * sumXX - sumX * sumX);
}

void Stint::applyFuelCorrection(const Fuel &fuel, int totalLaps) {
    for (Lap& lap : laps) {
        lap.applyFuelCorrection(fuel, totalLaps);
    }
}

std::ostream& operator<<(std::ostream& os, const Stint& stint) {
    if (stint.startLap == stint.endLap) {
        os << "Lap " << stint.startLap;
    }
    else {
        os << "Laps " << stint.startLap << " - " << stint.endLap;
    }
    os << ": " << stint.compound << " (" << stint.tyreAgeAtStart << " laps old at start)";

    double avgPace = stint.averagePace();
    if (avgPace > 0) {
        os << ", avg " << avgPace << "s";
    }
    else {
        os << ", avg -";
    }

    double deg = stint.tyreDeg();
    if (!stint.isSlick()) {
        os << ", deg - (wet)";
    }
    else if (deg != 0) {
        os << ", deg " << deg << " s/lap";
    }
    else {
        os << ", deg -";
    }
    os << '\n';

    for (const Lap& lap : stint.laps) {
        os << '\t' << lap << '\n';
    }
    return os;
}
