//
// Created by Andrei on 01-Nov-26.
//

#ifndef OOP_FUEL_H
#define OOP_FUEL_H
#include <ostream>


class Fuel {
    double startFuelQty;
    double secondsPerKg;

public:
    Fuel(double startFuelQty_, double secondsPerKg_);

    double timeCost(int lapNr, int totalLaps) const;

    friend std::ostream& operator<<(std::ostream& os, const Fuel& fuel);
};


#endif //OOP_FUEL_H
