//
// Created by Andrei on 27-Oct-26.
//

#ifndef OOP_STINT_H
#define OOP_STINT_H
#include "Lap.h"
#include <string>
#include <vector>


class Stint {
    std::string compound;
    int tyreAgeAtStart;
    int startLap;
    int endLap;
    std::vector<Lap> laps;

    double medianPace() const;
    int tyreAgeAt(const Lap& lap) const;

public:
    Stint(const std::string& compound_, int tyreAgeAtStart_, int startLap_, int endLap_);

    bool isLapInStint(int lapNumber) const;
    void addLap(const Lap& lap);

    double averagePace() const;
    double bestSectorInStint(int i) const;
    const Lap* fastestLap() const;
    int topSpeed() const;
    double tyreDeg() const;

    friend std::ostream& operator<<(std::ostream& os, const Stint& stint);
};


#endif //OOP_STINT_H
