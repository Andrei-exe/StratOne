//
// Created by Andrei on 28-Oct-26.
//

#ifndef OOP_SESSION_H
#define OOP_SESSION_H
#include <string>
#include "Driver.h"


class Session {
    std::string name;
    std::vector<Driver> drivers;

    Driver* findDriverByNumber(int number);


public:
    explicit Session(const std::string& name_);

    void addDriver(const Driver& driver);
    const Driver* findDriverByAcronym(const std::string& acronym) const;

    const Driver* idealFastestDriver() const;
    const Driver* fastestLapDriver() const;

    bool addStint(int number, const Stint& stint);
    bool addLap(int number, const Lap& lap);

    friend std::ostream& operator<<(std::ostream& os, const Session& session);
};


#endif //OOP_SESSION_H
