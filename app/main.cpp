#include "Session.h"
#include "Loader.h"
#include <iostream>

#include "PositionChange.h"
#include "Strategy.h"

namespace {
    std::string chooseEvent(const Loader& loader, const std::string& sessionType) {
        const std::vector<std::string> events = loader.availableEvents(sessionType);
        if (events.empty()) {
            std::cout << "\nNo events found\n";
            return "";
        }
        while (true) {
            for (size_t i = 0; i < events.size(); ++i) {
                std::cout << i + 1 << ". " << events[i] << '\n';
            }
            std::cout << "\nChoose event: ";
            int option;
            if (!(std::cin >> option)) {
                std::cout << "\nError reading option\n";
                return "";
            }
            if (option <= 0 || option > static_cast<int>(events.size())) {
                std::cout << "\nInvalid option\n\n";
                continue;
            }
            return events[static_cast<std::size_t>(option - 1)];
        }
    }

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

    void showStandings(const Session& session) {
        std::cout << "\nLap number: ";
        int lapNr;
        if (!(std::cin >> lapNr)) {
            std::cout << "\nError reading lap number\n";
            return;
        }
        const auto standings = session.standingsAfterLap(lapNr);
        if (standings.empty()) {
            std::cout << "\nNo data for lap " << lapNr << "\n";
            return;
        }
        for (const auto& standing : standings) {
            std::cout << standing << '\n';
        }
    }

    void showMovers(const Session& session) {
        const auto changes = session.positionChanges();
        if (changes.empty()) {
            std::cout << "\nNo data for changes\n";
            return;
        }
        std::cout << "\nPositions gained after lap 1 (sorted by gains):\n";
        for (const auto& change : changes) {
            std::cout << change << '\n';
        }
    }

    void showFastestPitstop(const Session& session) {
        const Driver* driver = session.fastestPitstopDriver();
        if (driver == nullptr) {
            std::cout << "\nNo pitstop data\n";
            return;
        }
        std::cout << "\nFastest pitstop: " << driver->getAcronym() << " - " << *(driver->fastestPitStop()) << '\n';
    }

    void runMenu(const Session& session) {
        while (true) {
            std::cout << "\n1. Driver info\n" << "2. Fastest lap\n" << "3. Ideal lap\n" << "4. Strategy report\n" << "5. Standings on any lap\n" << "6. Position changes\n" << "7. Fastest pitstop\n" << "0. Exit\n" << "Option: ";
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
                    break;
                case 5:
                    showStandings(session);
                    break;
                case 6:
                    showMovers(session);
                    break;
                case 7:
                    showFastestPitstop(session);
                case 0:
                    return;
                default:
                    std::cout << "\nInvalid option\n";
            }
        }
    }
}

int main() {
    Loader loader("assets");
    const std::string sessionType = "race";
    const std::string event = chooseEvent(loader, sessionType);
    if (event.empty()) {
        return 0;
    }
    Session session(loader.load(event, sessionType));
    runMenu(session);

    return 0;
}
