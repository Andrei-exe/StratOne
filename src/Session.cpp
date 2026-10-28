//
// Created by Andrei on 28-Oct-26.
//

#include "Session.h"

Session::Session(const std::string& name_) : name(name_) {}

void Session::addDriver(const Driver& driver) {
    drivers.push_back(driver);
}

const Driver *Session::fastestDriver() const {
    const Driver* fastest = nullptr;
    for (const Driver& driver : drivers) {
        double idealTime = driver.idealLapTime();
        if (idealTime > 0 && (fastest == nullptr || idealTime < fastest->idealLapTime())) {
            fastest = &driver;
        }
    }
    return fastest;
}

std::ostream& operator<<(std::ostream& os, const Session& session) {
    os << "=== " << session.name << " ===\n";
    for (const Driver& driver : session.drivers) {
        os << driver << '\n';
    }
    return os;
}