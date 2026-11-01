//
// Created by Andrei on 01-Nov-26.
//

#include "Strategy.h"
#include <cmath>

Strategy::Strategy(double pitLoss_, int raceLength_) :
    pitLoss(pitLoss_), raceLength(raceLength_) {}

double Strategy::timeLost(int stops, double deg) const {
    //Time lost in pits with x stops
    double totalLossInPits = stops * pitLoss;
    //Time lost in laps with (stops + 1) stints, loss on stint = deg * lap_count^2 / 2
    double totalLossInLaps = deg * raceLength *raceLength / (2 * (stops + 1));
    return totalLossInPits + totalLossInLaps;
}

int Strategy::optimalStintLength(double deg) const {
    if (deg <= 0 || pitLoss <= 0) {
        return 0;
    }
    //Loss on stint = deg * lap_count^2 / 2; pitLoss accounted for => sqrt(2 * pitLoss / deg)
    int optimalLap = static_cast<int>(std::round(std::sqrt((2 * pitLoss) / deg)));
    if (optimalLap > raceLength) {
        return raceLength;
    }
    return optimalLap;
}

int Strategy::optimalStops(double deg) const {
    if (deg <= 0 || pitLoss <= 0) {
        return 0;
    }
    constexpr int minStops = 1, maxStops = 3;
    int bestStops = minStops;
    double bestTime = timeLost(minStops, deg);
    for (int stops = minStops + 1; stops <= maxStops; ++stops) {
        double candidateTime = timeLost(stops, deg);
        if (candidateTime < bestTime) {
            bestTime = candidateTime;
            bestStops = stops;
        }
    }
    return bestStops;
}

std::ostream & operator<<(std::ostream &os, const Strategy &strategy) {
    os << "Strategy model: pit loss " << strategy.pitLoss << "s, " << strategy.raceLength << " laps";
    return os;
}
