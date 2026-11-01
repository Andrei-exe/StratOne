//
// Created by Andrei on 01-Nov-26.
//

#ifndef OOP_STRATEGY_H
#define OOP_STRATEGY_H
#include <ostream>


class Strategy {
    double pitLoss;
    int raceLength;

    double timeLost(int stops, double deg) const;

public:
    Strategy(double pitLoss_, int raceLength_);
    int optimalStintLength(double deg) const;
    int optimalStops(double deg) const;

    friend std::ostream& operator<<(std::ostream& os, const Strategy& strategy);
};


#endif //OOP_STRATEGY_H
