//
// Created by Andrei on 27-Oct-26.
//

#include "Driver.h"

Driver::Driver(int number_, const std::string& acronym_, const std::string& team_) :
    number(number_), acronym(acronym_), team(team_) {}

Driver::Driver(const Driver& other) :
    number(other.number), acronym(other.acronym), team(other.team), stints(other.stints) {}

Driver& Driver::operator=(const Driver& other) {
    if (this != &other) {
        number = other.number;
        acronym = other.acronym;
        team = other.team;
        stints = other.stints;
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

std::ostream& operator<<(std::ostream& os, const Driver& driver) {
    os << "#" << driver.number << " " << driver.acronym << " - " << driver.team << '\n';
    for (const Stint& stint : driver.stints) {
        os << " " << stint;
    }
    return os;
}
