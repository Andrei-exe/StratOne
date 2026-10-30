//
// Created by Andrei on 28-Oct-26.
//

#include "Session.h"

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

Driver *Session::findDriverByNumber(int number) {
    for (Driver& driver : drivers) {
        if (driver.hasNumber(number)) {
            return &driver;
        }
    }
    return nullptr;
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

std::ostream& operator<<(std::ostream& os, const Session& session) {
    os << "=== " << session.name << " ===\n";
    for (const Driver& driver : session.drivers) {
        os << driver << '\n';
    }
    return os;
}