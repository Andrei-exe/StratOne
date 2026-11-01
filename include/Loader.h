//
// Created by Andrei on 30-Oct-26.
//

#ifndef OOP_LOADER_H
#define OOP_LOADER_H
#include "Session.h"
#include <string>
#include <vector>

class Loader {
    std::string rootFolder;

    std::string sessionFolder(const std::string& event, const std::string& sessionType) const;
    void readDrivers(Session& session, const std::string& folder) const;
    void readStints(Session& session, const std::string& folder) const;
    void readLaps(Session& session, const std::string& folder) const;
    Fuel readFuel(const std::string& folder) const;
    void readPitStops(Session& session, const std::string& folder) const;

public:
    explicit Loader(const std::string &rootFolder_);
    Session load(const std::string& event, const std::string& sessionType) const;
    std::vector<std::string> availableEvents(const std::string& sessionType) const;

    friend std::ostream& operator<<(std::ostream& os, const Loader& loader);
};


#endif //OOP_LOADER_H
