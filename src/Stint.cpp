//
// Created by Andrei on 27-Oct-26.
//

#include "Stint.h"

Stint::Stint(const std::string& compound_, int startLap_, int endLap_) :
    compound(compound_), startLap(startLap_), endLap(endLap_) {}

bool Stint::isLapInStint(int lapNumber) const {
    return lapNumber >= startLap && lapNumber <= endLap;
}

void Stint::addLap(const Lap& lap) {
    laps.push_back(lap);
}

double Stint::averagePace() const {
    double totalPace = 0;
    int lapCount = 0;
    for (const Lap& lap : laps) {
        if (lap.isRepresentative()) {
            totalPace += lap.getLapDuration();
            lapCount++;
        }
    }
    if (lapCount > 0) {
        return totalPace / lapCount;
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

std::ostream& operator<<(std::ostream& os, const Stint& stint) {
    if (stint.startLap == stint.endLap) {
        os << "Lap " << stint.startLap;
    }
    else {
        os << "Laps " << stint.startLap << " - " << stint.endLap;
    }
    os << ": " << stint.compound;

    double avgPace = stint.averagePace();
    if (avgPace > 0) {
        os << ", avg " << avgPace << "s";
    }
    else {
        os << ", avg -";
    }
    os << '\n';

    for (const Lap& lap : stint.laps) {
        os << '\t' << lap << '\n';
    }
    return os;
}