//
// Created by Andrei on 27-Oct-26.
//

#include "Driver.h"

Driver::Driver(int number_, const std::string& acronym_, const std::string& fullName_, const std::string& team_) :
    number(number_), acronym(acronym_), fullName(fullName_), team(team_) {}

Driver::Driver(const Driver& other) :
    number(other.number), acronym(other.acronym), fullName(other.fullName), team(other.team), stints(other.stints), pitStops(other.pitStops) {}

Driver& Driver::operator=(const Driver& other) {
    if (this != &other) {
        number = other.number;
        acronym = other.acronym;
        fullName = other.fullName;
        team = other.team;
        stints = other.stints;
        pitStops = other.pitStops;
    }
    return *this;
}

Driver::~Driver() {}

const std::string &Driver::getAcronym() const {
    return acronym;
}

void Driver::addStint(const Stint &stint) {
    stints.push_back(stint);
}

bool Driver::addLap(const Lap &lap) {
    for (Stint& stint : stints) {
        if (stint.isLapInStint(lap.getLapNumber())) {
            stint.addLap(lap);
            return true;
        }
    }
    return false;
}

int Driver::lastLapNumber() const {
    int last = 0;
    for (const Stint& stint : stints) {
        if (stint.lastLapNumber() > last) {
            last = stint.lastLapNumber();
        }
    }
    return last;
}

void Driver::addPitStop(const PitStop &pitStop) {
    pitStops.push_back(pitStop);
}

double Driver::bestSectorOverall(int i) const {
    double best = -1;
    for (const Stint& stint : stints) {
        double s = stint.bestSectorInStint(i);
        if (s > 0 && (best < 0 || s < best)) {
            best = s;
        }
    }
    return best;
}

double Driver::idealLapTime() const {
    double idealLap = 0;
    for (int i = 0; i < 3; i++) {
        double best = bestSectorOverall(i);
        if (best < 0) {
            return 0;
        }
        idealLap += best;
    }
    return idealLap;
}

int Driver::topSpeed() const {
    int top = 0;
    for (const Stint& stint : stints) {
        if (stint.topSpeed() > top) {
            top = stint.topSpeed();
        }
    }
    return top;
}

const Lap *Driver::fastestLap() const {
    const Lap* fastest = nullptr;
    for (const Stint& stint : stints) {
        const Lap* temp = stint.fastestLap();
        if (temp != nullptr && (fastest == nullptr || temp->isFasterThan(*fastest))) {
            fastest = temp;
        }
    }
    return fastest;
}

void Driver::applyFuelCorrection(const Fuel &fuel, int totalLaps) {
    for (Stint& stint : stints) {
        stint.applyFuelCorrection(fuel, totalLaps);
    }
}

std::vector<double> Driver::pitLaneDurations() const {
    std::vector<double> pitLaneDurations;
    for (const PitStop& pitStop : pitStops) {
        pitLaneDurations.push_back(pitStop.getPitLaneDuration());
    }
    return pitLaneDurations;
}

double Driver::averageDeg() const {
    double weightedDegSum = 0;
    int lapsCounted = 0;
    for (const Stint& stint : stints) {
        double stintTyreDeg = stint.tyreDeg();
        if (stintTyreDeg == 0) {
            continue;
        }
        weightedDegSum += stintTyreDeg * stint.lapCount();
        lapsCounted += stint.lapCount();
    }
    if (lapsCounted == 0) {
        return 0;
    }
    return weightedDegSum / lapsCounted;
}

int Driver::pitStopCount() const {
    return static_cast<int>(pitStops.size());
}

const Lap *Driver::findLap(int lapNumber) const {
    for (const Stint& stint : stints) {
        if (!stint.isLapInStint(lapNumber)) {
            continue;
        }
        return stint.findLap(lapNumber);
    }
    return nullptr;
}

bool Driver::hasNumber(int number_) const {
    if (number_ == number) {
        return true;
    }
    return false;
}

bool Driver::hasAcronym(const std::string &acronym_) const {
    if (acronym_ == acronym) {
        return true;
    }
    return false;
}

bool Driver::racedOnSlicksOnly() const {
    for (const Stint& stint : stints) {
        if (!stint.isSlick()) {
            return false;
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const Driver& driver) {
    os << "#" << driver.number << " " << driver.fullName << " (" <<  driver.acronym << ") - " << driver.team << '\n';
    for (const Stint& stint : driver.stints) {
        os << " " << stint;
    }
    if (!driver.pitStops.empty()) {
        os << "Pit stops: \n";
        for (const PitStop& pitStop : driver.pitStops) {
            os << '\t' << pitStop << '\n';
        }
    }
    return os;
}
