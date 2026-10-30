//
// Created by Andrei on 30-Oct-26.
//

#ifndef OOP_LOADER_H
#define OOP_LOADER_H
#include "Session.h"
#include <string>

class Loader {
    std::string folderPath;

    void readDrivers(Session& session) const;
    void readStints(Session& session) const;
    void readLaps(Session& session) const;

public:
    explicit Loader(const std::string &folderPath_);
    Session load(const std::string& sessionName) const;

    friend std::ostream& operator<<(std::ostream& os, const Loader& loader);
};


#endif //OOP_LOADER_H
