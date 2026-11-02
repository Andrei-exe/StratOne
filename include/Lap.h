//
// Created by Andrei on 27-Oct-26.
//

#ifndef OOP_LAPS_H
#define OOP_LAPS_H
#include <array>
#include <chrono>
#include <ostream>
#include "Fuel.h"


class Lap {
    int number;
    double duration;
    std::chrono::sys_time<std::chrono::microseconds> startTime;
    std::array<double,3> sectors;
    int speedTrap;
    bool isPitOutLap;
    double fuelPenalty;

public:
    Lap(int number_, double duration_, std::chrono::sys_time<std::chrono::microseconds> startTime_, std::array<double,3> sectors_, int speedTrap_, bool isPitOutLap_);

    int getLapNumber() const;
    double getLapDuration() const;
    double getSector(int i) const;
    int getSpeedTrap() const;

    bool isRepresentative() const;
    bool isFasterThan(const Lap& otherLap) const;
    bool hasStartTime() const;
    void applyFuelCorrection(const Fuel& fuel, int totalLaps);
    double fuelCorrectedDuration() const;
    std::chrono::sys_time<std::chrono::microseconds> endTime() const;

    friend std::ostream& operator<<(std::ostream& os, const Lap& lap);
};


#endif //OOP_LAPS_H
