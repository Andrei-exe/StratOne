//
// Created by Andrei on 01-Nov-26.
//

#ifndef OOP_PITSTOP_H
#define OOP_PITSTOP_H
#include <ostream>


class PitStop {
    int inLap;
    double pitlaneDuration;
    double stopDuration;

public:
    PitStop(int inLap_, double pitlaneDuration_, double stopDuration_);

    friend std::ostream& operator<<(std::ostream& os, const PitStop& pitStop);
};


#endif //OOP_PITSTOP_H
