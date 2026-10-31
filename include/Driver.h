//
// Created by Andrei on 27-Oct-26.
//

#ifndef OOP_DRIVER_H
#define OOP_DRIVER_H
#include <string>
#include "Stint.h"


class Driver {
    int number;
    std::string acronym;
    std::string team;
    std::vector<Stint> stints;

    double bestSectorOverall(int i) const;

public:
    Driver(int number_, const std::string& acronym_, const std::string& team_);
    Driver(const Driver& other);
    Driver& operator=(const Driver& other);
    ~Driver();

    const std::string& getAcronym() const;

    void addStint(const Stint& stint);
    bool addLap(const Lap& lap);

    double idealLapTime() const;
    int topSpeed() const;
    const Lap* fastestLap() const;

    bool hasNumber(int number_) const;
    bool hasAcronym(const std::string& acronym_) const;

    friend std::ostream& operator<<(std::ostream& os, const Driver& driver);
};


#endif //OOP_DRIVER_H
