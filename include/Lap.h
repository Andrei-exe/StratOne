//
// Created by Andrei on 27-Oct-26.
//

#ifndef OOP_LAPS_H
#define OOP_LAPS_H
#include <array>
#include <ostream>


class Lap {
    int number;
    double duration;
    std::array<double,3> sectors;
    int speedTrap;
    bool isPitOutLap;

public:
    Lap(int number_, double duration_, std::array<double,3> sectors_, int speedTrap_, bool isPitOutLap_);

    int getLapNumber() const;
    double getLapDuration() const;
    double getSector(int i) const;
    int getSpeedTrap() const;
    bool isRepresentative() const;

    friend std::ostream& operator<<(std::ostream& os, const Lap& lap);
};


#endif //OOP_LAPS_H
