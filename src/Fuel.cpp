//
// Created by Andrei on 01-Nov-26.
//

#include "Fuel.h"

Fuel::Fuel(double startFuelQty_, double fuelCostPerLap_) :
    startFuelQty(startFuelQty_), secondsPerKg(fuelCostPerLap_) {}

double Fuel::timeCost(int lapNr, int totalLaps) const {
    if (totalLaps <= 0) {
        return 0;
    }
    double kgFuelPerLap = startFuelQty / totalLaps;
    double fuelAtLapStart = startFuelQty - kgFuelPerLap * (lapNr - 1);
    return fuelAtLapStart * secondsPerKg;
}

std::ostream & operator<<(std::ostream &os, const Fuel &fuel) {
    os << "Fuel data: " << fuel.startFuelQty << " kg at start, " << fuel.secondsPerKg << "s/kg";
    return os;
}
