#include "Session.h"
#include "Loader.h"
#include <iostream>

namespace {
    void showDriverInfo(const Session& session) {
        std::string acronym;
        std::cout << "\nDriver acronym: ";
        if (!(std::cin >> acronym)) {
            std::cout << "\nError reading acronym\n";
            return;
        }

        const Driver* driver = session.findDriverByAcronym(acronym);
        if (driver == nullptr) {
            std::cout << "\nError finding driver\n";
            return;
        }
        std::cout << '\n' << *driver << '\n';

        const Lap* fastestLap = driver->fastestLap();
        if (fastestLap == nullptr) {
            std::cout << "\nNo lap data\n";
        } else {
            std::cout << driver->getAcronym() << " fastest lap: " << *fastestLap << '\n';
        }
        std::cout << driver->getAcronym() << " ideal lap time: " << driver->idealLapTime() << '\n';
        std::cout << driver->getAcronym() << " top speed: " << driver->topSpeed() << '\n';
    }

    void showFastestLap(const Session& session) {
        const Driver* driver = session.fastestLapDriver();
        if (driver == nullptr) {
            std::cout << "\nError finding driver\n";
            return;
        }

        const Lap* fastestLap = driver->fastestLap();
        if (fastestLap == nullptr) {
            std::cout << "\nNo lap data\n";
            return;
        }
        std::cout << "\nFastest lap: " << driver->getAcronym() << " - " << *fastestLap << '\n';
    }

    void showIdealLap(const Session& session) {
        const Driver* driver = session.idealFastestDriver();
        if (driver == nullptr) {
            std::cout << "\nError finding driver\n";
            return;
        }
        std::cout << "\nIdeal lap: " << driver->idealLapTime() << " by " << driver->getAcronym() << " km/h\n";
    }

    void runMenu(const Session& session) {
        while (true) {
            std::cout << "\n1. Driver info\n" << "2. Fastest lap\n" << "3. Ideal lap\n" << "0. Exit\n" << "Option: ";
            int option;
            if (!(std::cin >> option)) {
                std::cout << "\nError reading option\n";
                return;
            }
            switch (option) {
                case 1:
                    showDriverInfo(session);
                    break;
                case 2:
                    showFastestLap(session);
                    break;
                case 3:
                    showIdealLap(session);
                    break;
                case 0:
                    return;
                default:
                    std::cout << "\nInvalid option\n";
            }
        }
    }
}

int main() {
    Loader loader("assets/netherlands_2023");
    Session session(loader.load("netherlands_2023"));

    runMenu(session);

    return 0;
}
