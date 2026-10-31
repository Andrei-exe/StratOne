//
// Created by Andrei on 30-Oct-26.
//

#include "Loader.h"
#include <csv.hpp>

namespace {
    double columnValueOrZero(const csv::CSVRow &row, const std::string &column) {
        if (row[column].is_null()) {
            return 0.0;
        }
        return row[column].get<double>();
    }
}

Loader::Loader(const std::string &folderPath_) :
    folderPath(folderPath_) {}

void Loader::readDrivers(Session &session) const {
    std::string driversFile = folderPath + "/drivers.csv";
    csv::CSVReader reader(driversFile);
    for (const auto& row : reader) {
        int number = row["driver_number"].get<int>();
        std::string acronym = row["name_acronym"].get<std::string>();
        std::string team = row["team_name"].get<std::string>();
        session.addDriver(Driver(number, acronym, team));
    }
}

Session Loader::load(const std::string &sessionName) const {
    Session session(sessionName);
    readDrivers(session);
    readStints(session);
    readLaps(session);
    return session;
}

void Loader::readStints(Session &session) const {
    std::string stintsFile = folderPath + "/stints.csv";
    csv::CSVReader reader(stintsFile);
    for (const auto& row : reader) {
        int driverNumber = row["driver_number"].get<int>();
        std::string compound = row["compound"].get<std::string>();
        int lapStart = row["lap_start"].get<int>();
        int lapEnd = row["lap_end"].get<int>();
        session.addStint(driverNumber, Stint(compound, lapStart, lapEnd));
    }
}

void Loader::readLaps(Session &session) const {
    std::string lapsFile = folderPath + "/laps.csv";
    csv::CSVReader reader(lapsFile);
    for (const auto& row : reader) {
        double lapDuration = columnValueOrZero(row, "lap_duration");
        if (lapDuration == 0.0) {
            continue;
        }
        int driverNumber = row["driver_number"].get<int>();
        int lapNumber = row["lap_number"].get<int>();
        std::array<double, 3> sectors = {
            columnValueOrZero(row, "duration_sector_1"),
            columnValueOrZero(row, "duration_sector_2"),
            columnValueOrZero(row, "duration_sector_3")
        };
        int speedTrap = static_cast<int>(columnValueOrZero(row, "st_speed"));
        bool isPitOutLap = row["is_pit_out_lap"].get<bool>();
        session.addLap(driverNumber, Lap(lapNumber, lapDuration, sectors, speedTrap, isPitOutLap));
    }
}

std::ostream& operator<<(std::ostream &os, const Loader &loader) {
    os << "Folder: " << loader.folderPath << '\n';
    return os;
}
