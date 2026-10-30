#include "Session.h"
#include "Loader.h"
#include <iostream>

int main() {
    Loader loader("assets/netherlands_2023");
    Session session(loader.load("netherlands_2023"));
    const Driver* driver = session.fastestLapDriver();
    if (driver != nullptr) {
        const Lap* fastestLap = driver->fastestLap();
        if (fastestLap != nullptr) {
            std::cout << *fastestLap << std::endl;
        }
        //std::cout << *driver;
    }

    return 0;
}
