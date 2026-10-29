#include "Session.h"
#include "csv.hpp"
#include <iostream>

int main() {
    Stint soft("SOFT", 1, 3);
    soft.addLap(Lap(1, 80.3, {32.1, 26.5, 20.7}, 290, false));
    soft.addLap(Lap(2, 75.1, {28.0, 26.3, 20.8}, 312, false));
    soft.addLap(Lap(3, 74.9, {28.1, 26.0, 20.8}, 315, false));

    Driver ver(1, "VER", "Red Bull Racing");
    ver.addStint(soft);

    Session race("Dutch GP 2023 - Race");
    race.addDriver(ver);

    std::cout << race;
    std::cout << "Ideal lap VER: " << ver.idealLapTime() << "s\n";

    return 0;
}