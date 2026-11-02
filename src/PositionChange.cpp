//
// Created by Andrei on 02-Nov-26.
//

#include "PositionChange.h"

PositionChange::PositionChange(const std::string &acronym_, int startPosition_, int finishPosition_) :
    acronym(acronym_), startPosition(startPosition_), finishPosition(finishPosition_) {}

int PositionChange::positionsGained() const {
    return startPosition - finishPosition;
}

std::ostream &operator<<(std::ostream &os, const PositionChange &positionChange) {
    const int posGain = positionChange.positionsGained();
    os << positionChange.acronym << " P" << positionChange.startPosition << " -> P" << positionChange.finishPosition << " (";
    if (posGain > 0) {
        os << "+" << posGain << ")";
    }
    else if (posGain == 0) {
        os << "=)";
    }
    else {
        os  << posGain << ")";
    }
    return os;
}
