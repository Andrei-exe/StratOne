//
// Created by Andrei on 28-Oct-26.
//

#ifndef OOP_SESSION_H
#define OOP_SESSION_H
#include <string>
#include <chrono>
#include <utility>
#include "Driver.h"
#include "PositionChange.h"
#include "Standing.h"
#include "Strategy.h"


class Session {
    std::string name;
    std::vector<Driver> drivers;

    Driver* findDriverByNumber(int number);
    int sessionLength() const;
    std::vector<std::pair<const Driver*, std::chrono::sys_time<std::chrono::microseconds>>> crossingOrder(int lapNumber) const;
    std::vector<const Driver*> finalOrder() const;

public:
    explicit Session(const std::string& name_);

    void addDriver(int number, const Driver& driver);
    const Driver* findDriverByAcronym(const std::string& acronym) const;

    const Driver* idealFastestDriver() const;
    const Driver* fastestLapDriver() const;
    const Driver* fastestPitstopDriver() const;

    bool addStint(int number, const Stint& stint);
    bool addLap(int number, const Lap& lap);
    void applyFuelCorrection(const Fuel& fuel);
    bool addPitStop(int number, const PitStop& pitStop);
    double pitLoss() const;
    Strategy strategy() const;
    bool isDryRace() const;
    std::vector<Standing> standingsAfterLap(int lapNumber) const;
    std::vector<PositionChange> positionChanges() const;

    friend std::ostream& operator<<(std::ostream& os, const Session& session);
};


#endif //OOP_SESSION_H
