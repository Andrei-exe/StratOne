//
// Created by Andrei on 27-Oct-26.
//

#include "Lap.h"

Lap::Lap(int number_, double duration_, std::array<double, 3> sectors_, int speedTrap_, bool isPitOutLap_) :
    number(number_), duration(duration_), sectors(sectors_), speedTrap(speedTrap_), isPitOutLap(isPitOutLap_) {}

int Lap::getLapNumber() const {
    return number;
}

double Lap::getLapDuration() const {
    return duration;
}

double Lap::getSector(int i) const {
    return sectors.at(i);
}

int Lap::getSpeedTrap() const {
    return speedTrap;
}

bool Lap::isRepresentative() const {
    return !isPitOutLap && duration > 0;
}

std::ostream& operator<<(std::ostream& os, const Lap& lap) {
    os << "Lap " << lap.number << ": " << lap.duration << '\n'
       << '\t' << "Speed trap: " << lap.speedTrap << " km/h";
    if (lap.isPitOutLap) {
        os << " (Pit out lap)";
    }
    return os;
}