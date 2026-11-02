//
// Created by Andrei on 02-Nov-26.
//

#ifndef OOP_POSITIONCHANGE_H
#define OOP_POSITIONCHANGE_H
#include <string>
#include <ostream>

class PositionChange {
    std::string acronym;
    int startPosition;
    int finishPosition;

public:
    PositionChange(const std::string& acronym_, int startPosition_, int finishPosition_);

    int positionsGained() const;

    friend std::ostream& operator<<(std::ostream& os, const PositionChange& positionChange);
};


#endif //OOP_POSITIONCHANGE_H
