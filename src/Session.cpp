//
// Created by Andrei on 28-Oct-26.
//

#include "Session.h"
#include "Statistics.h"
#include <algorithm>

Session::Session(const std::string& name_) : name(name_) {}

void Session::addDriver(const Driver& driver) {
    drivers.push_back(driver);
}

const Driver *Session::idealFastestDriver() const {
    const Driver* fastest = nullptr;
    for (const Driver& driver : drivers) {
        double idealTime = driver.idealLapTime();
        if (idealTime > 0 && (fastest == nullptr || idealTime < fastest->idealLapTime())) {
            fastest = &driver;
        }
    }
    return fastest;
}

const Driver *Session::fastestLapDriver() const {
    const Driver* fastest = nullptr;
    const Lap* lap = nullptr;
    for (const Driver& driver : drivers) {
        const Lap* temp = driver.fastestLap();
        if (temp != nullptr && (fastest == nullptr || temp->isFasterThan(*lap))) {
            fastest = &driver;
            lap = temp;
        }
    }
    return fastest;
}

const Driver *Session::fastestPitstopDriver() const {
    const Driver* fastest = nullptr;
    for (const Driver& driver : drivers) {
        const PitStop* temp = driver.fastestPitStop();
        if (temp == nullptr) {
            continue;
        }
        if (fastest == nullptr || temp->isFasterThan(*(fastest->fastestPitStop()))) {
            fastest = &driver;
        }
    }
    return fastest;
}

Driver *Session::findDriverByNumber(int number) {
    for (Driver& driver : drivers) {
        if (driver.hasNumber(number)) {
            return &driver;
        }
    }
    return nullptr;
}

int Session::sessionLength() const {
    int length = 0;
    for (const Driver& driver : drivers) {
        if (driver.lastLapNumber() > length) {
            length = driver.lastLapNumber();
        }
    }
    return length;
}

std::vector<std::pair<const Driver *, std::chrono::sys_time<std::chrono::microseconds>>> Session::crossingOrder(int lapNumber) const {
    std::vector<std::pair<const Driver*, std::chrono::sys_time<std::chrono::microseconds>>> crossings;
    for (const Driver& driver : drivers) {
        const Lap* lap = driver.findLap(lapNumber);
        if (lap == nullptr || !lap->hasStartTime()) {
            continue;
        }
        crossings.emplace_back(&driver, lap->endTime());
    }
    std::ranges::sort(crossings, [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    return crossings;
}

std::vector<const Driver*> Session::finalOrder() const {
    std::vector<const Driver*> finalClassification;
    for (const Driver& driver : drivers) {
        const Lap* finalDriverLap = driver.findLap(driver.lastLapNumber());
        if (finalDriverLap == nullptr || !finalDriverLap->hasStartTime()) {
            continue;
        }
        finalClassification.push_back(&driver);
    }
    std::ranges::sort(finalClassification, [](const auto& a, const auto& b) {
        if (a->lastLapNumber() != b->lastLapNumber()) {
            return a->lastLapNumber() > b->lastLapNumber();
        }
        return (a->findLap(a->lastLapNumber()))->endTime() < (b->findLap(b->lastLapNumber()))->endTime();
    });
    return finalClassification;
}

const Driver *Session::findDriverByAcronym(const std::string& acronym) const {
    for (const Driver& driver : drivers) {
        if (driver.hasAcronym(acronym)) {
            return &driver;
        }
    }
    return nullptr;
}

bool Session::addStint(int number, const Stint &stint) {
    Driver* driver = findDriverByNumber(number);
    if (driver != nullptr) {
        driver->addStint(stint);
        return true;
    }
    return false;
}

bool Session::addLap(int number, const Lap &lap) {
    Driver* driver = findDriverByNumber(number);
    if (driver != nullptr) {
        return driver->addLap(lap);
    }
    return false;
}

void Session::applyFuelCorrection(const Fuel &fuel) {
    int length = sessionLength();
    for (Driver& driver : drivers) {
        driver.applyFuelCorrection(fuel, length);
    }
}

bool Session::addPitStop(int number, const PitStop &pitStop) {
    Driver* driver = findDriverByNumber(number);
    if (driver == nullptr) {
        return false;
    }
    driver->addPitStop(pitStop);
    return true;
}

double Session::pitLoss() const {
    std::vector<double> pitLoss;
    for (const Driver& driver : drivers) {
        std::vector<double> durations = driver.pitLaneDurations();
        pitLoss.insert(pitLoss.end(), durations.begin(), durations.end());
    }
    return stats::median(pitLoss);
}

Strategy Session::strategy() const {
    return Strategy{pitLoss(), sessionLength()};
}

bool Session::isDryRace() const {
    for (const Driver& driver : drivers) {
        if (!driver.racedOnSlicksOnly()) {
            return false;
        }
    }
    return true;
}

std::vector<Standing> Session::standingsAfterLap(int lapNumber) const {
    const auto crossings = crossingOrder(lapNumber);
    if (crossings.empty()) {
        return {};
    }
    const auto leaderTime = crossings[0].second;
    std::vector<Standing> standings;
    for (std::size_t i = 0; i < crossings.size(); ++i) {
        const double gap = std::chrono::duration<double>(crossings[i].second - leaderTime).count();
        standings.emplace_back(crossings[i].first->getAcronym(), static_cast<int>(i) + 1, gap);
    }
    return standings;
}

std::vector<PositionChange> Session::positionChanges() const {
    const auto start = crossingOrder(1);
    const auto finish = finalOrder();
    std::vector<PositionChange> changes;
    for (std::size_t i = 0; i < finish.size(); ++i) {
        int startPos = 0;
        for (std::size_t j = 0; j < start.size(); ++j) {
            if (start[j].first == finish[i]) {
                startPos = static_cast<int>(j) + 1;
                break;
            }
        }
        if (startPos == 0) {
            continue;
        }
        changes.emplace_back(finish[i]->getAcronym(), startPos, static_cast<int>(i) + 1);
    }
    std::ranges::stable_sort(changes, [](const PositionChange& a, const PositionChange& b) {
        return a.positionsGained() > b.positionsGained();
    });
    return changes;
}

std::ostream& operator<<(std::ostream& os, const Session& session) {
    os << "=== " << session.name << " ===\n";
    for (const Driver& driver : session.drivers) {
        os << driver << '\n';
    }
    return os;
}
