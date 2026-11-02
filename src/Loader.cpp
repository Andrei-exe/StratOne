//
// Created by Andrei on 30-Oct-26.
//

#include "Loader.h"
#include <csv.hpp>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <date.h>
#include <sstream>

namespace {
    double columnValueOrZero(const csv::CSVRow &row, const std::string &column) {
        if (row[column].is_null()) {
            return 0.0;
        }
        return row[column].get<double>();
    }

    std::chrono::sys_time<std::chrono::microseconds> columnTimestamp(const csv::CSVRow &row, const std::string &column) {
        const std::string text = row[column].get<std::string>();
        if (text.empty()) {
            return {};
        }
        std::istringstream input(text);
        std::chrono::sys_time<std::chrono::microseconds> timestamp{};
        date::from_stream(input, "%F %T%Ez", timestamp);
        if (input.fail()) {
            return {};
        }
        return timestamp;
    }
}

Loader::Loader(const std::string &rootFolder_) :
    rootFolder(rootFolder_) {}

std::string Loader::sessionFolder(const std::string &event, const std::string &sessionType) const {
    return rootFolder + "/" + event + "/" + sessionType;
}

void Loader::readDrivers(Session &session, const std::string& folder) const {
    std::string driversFile = folder + "/drivers.csv";
    csv::CSVReader reader(driversFile);
    for (const auto& row : reader) {
        int number = row["driver_number"].get<int>();
        std::string acronym = row["name_acronym"].get<std::string>();
        std::string fullName = row["full_name"].get<std::string>();
        std::string team = row["team_name"].get<std::string>();
        session.addDriver(number, Driver(number, acronym, fullName, team));
    }
}

void Loader::readStints(Session &session, const std::string& folder) const {
    std::string stintsFile = folder + "/stints.csv";
    csv::CSVReader reader(stintsFile);
    for (const auto& row : reader) {
        int driverNumber = row["driver_number"].get<int>();
        std::string compound = row["compound"].get<std::string>();
        int tyreAgeAtStart = row["tyre_age_at_start"].get<int>();
        int lapStart = row["lap_start"].get<int>();
        int lapEnd = row["lap_end"].get<int>();
        session.addStint(driverNumber, Stint(compound, tyreAgeAtStart, lapStart, lapEnd));
    }
}

void Loader::readLaps(Session &session, const std::string& folder) const {
    std::string lapsFile = folder + "/laps.csv";
    csv::CSVReader reader(lapsFile);
    for (const auto& row : reader) {
        double lapDuration = columnValueOrZero(row, "lap_duration");
        if (lapDuration == 0.0) {
            continue;
        }
        int driverNumber = row["driver_number"].get<int>();
        int lapNumber = row["lap_number"].get<int>();
        std::chrono::sys_time<std::chrono::microseconds> startTime = columnTimestamp(row, "date_start");
        std::array<double, 3> sectors = {
            columnValueOrZero(row, "duration_sector_1"),
            columnValueOrZero(row, "duration_sector_2"),
            columnValueOrZero(row, "duration_sector_3")
        };
        int speedTrap = static_cast<int>(columnValueOrZero(row, "st_speed"));
        bool isPitOutLap = row["is_pit_out_lap"].get<bool>();
        session.addLap(driverNumber, Lap(lapNumber, lapDuration, startTime, sectors, speedTrap, isPitOutLap));
    }
}

Fuel Loader::readFuel(const std::string& folder) const {
    std::string fuelFile = folder + "/fuel.csv";
    csv::CSVReader reader(fuelFile);
    csv::CSVRow row;
    if (!reader.read_row(row)) {
        return Fuel{0, 0};
    }
    double startFuel = row["start_fuel_kg"].get<double>();
    double secondsPerKg = row["seconds_per_kg"].get<double>();
    return Fuel{startFuel, secondsPerKg};
}

void Loader::readPitStops(Session &session, const std::string& folder) const {
    std::string pitStopsFile = folder + "/pit.csv";
    csv::CSVReader reader(pitStopsFile);
    for (const auto& row : reader) {
        int driverNumber = row["driver_number"].get<int>();
        int lapNumber = row["lap_number"].get<int>();
        double pitlaneDuration = row["lane_duration"].get<double>();
        double stopDuration = columnValueOrZero(row, "stop_duration");
        session.addPitStop(driverNumber, PitStop(lapNumber, pitlaneDuration, stopDuration));
    }
}

Session Loader::load(const std::string &event, const std::string& sessionType) const {
    const std::string folder = sessionFolder(event, sessionType);
    Session session(event + " " + sessionType);
    readDrivers(session, folder);
    readStints(session, folder);
    readLaps(session, folder);
    readPitStops(session, folder);
    session.applyFuelCorrection(readFuel(folder));
    return session;
}

std::vector<std::string> Loader::availableEvents(const std::string &sessionType) const {
    std::vector<std::string> events;
    for (const auto& entry : std::filesystem::directory_iterator(rootFolder)) {
        if (!entry.is_directory()) {
            continue;
        }
        const auto sessionPath = entry.path() / sessionType;
        if (!std::filesystem::exists(sessionPath)) {
            continue;
        }
        events.push_back(entry.path().filename().string());
    }
    std::ranges::sort(events);
    return events;
}

std::ostream& operator<<(std::ostream &os, const Loader &loader) {
    os << "Data root: " << loader.rootFolder << '\n';
    return os;
}
