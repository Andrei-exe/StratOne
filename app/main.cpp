#include "Session.h"
#include "Loader.h"
#include <iostream>

#include "Strategy.h"

namespace {
    const Driver* readDriver(const Session& session) {
        std::string acronym;
        std::cout << "\nDriver acronym: ";
        if (!(std::cin >> acronym)) {
            std::cout << "\nError reading acronym\n";
            return nullptr;
        }
        const Driver* driver = session.findDriverByAcronym(acronym);
        if (driver == nullptr) {
            std::cout << "\nError finding driver\n";
            return nullptr;
        }
        return driver;
    }

    void showDriverInfo(const Session& session) {
        const Driver* driver = readDriver(session);
        if (driver == nullptr) {
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
        std::cout << "\nIdeal lap: " << driver->idealLapTime() << "s by " << driver->getAcronym() << '\n';
    }

    void showStrategyReport(const Session& session) {
        const Driver* driver = readDriver(session);
        if (driver == nullptr) {
            return;
        }
        if (!session.isDryRace()) {
            std::cout << "\nStrategy report not available for wet races yet\n";
            return;
        }
        Strategy strategy = session.strategy();
        double deg = driver->averageDeg();
        int optimalStops = strategy.optimalStops(deg);
        if (optimalStops == 0) {
            std::cout << "\nNot enough data for strategy estimate\n";
            return;
        }
        int actualStops = driver->pitStopCount();
        std::cout << '\n' << strategy
            << "\nAverage degradation: " << deg
            << "\nOptimal stops: " << optimalStops
            << "\nOptimal stint length: " << strategy.optimalStintLength(deg)
            << "\nActual stops: " << actualStops << '\n';
        int diff = actualStops - optimalStops;
        if (diff == 0) {
            std::cout << "\nStrategy matched the optimum\n";
        }
        else if (diff > 0) {
            std::cout << '\n' << diff << " more stop(s) than optimal\n";
        }
        else {
            std::cout << '\n' << optimalStops - actualStops << " fewer stop(s) than optimal\n";
        }
    }

    void runMenu(const Session& session) {
        while (true) {
            std::cout << "\n1. Driver info\n" << "2. Fastest lap\n" << "3. Ideal lap\n" << "4. Strategy report\n" << "0. Exit\n" << "Option: ";
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
                case 4:
                    showStrategyReport(session);
                case 0:
                    return;
                default:
                    std::cout << "\nInvalid option\n";
            }
        }
    }
}

int main() {
    Loader loader("assets/japan_2025");
    Session session(loader.load("japan_2025"));

    runMenu(session);

    return 0;
}
