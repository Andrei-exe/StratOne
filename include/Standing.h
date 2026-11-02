//
// Created by Andrei on 02-Nov-26.
//

#ifndef OOP_STANDING_H
#define OOP_STANDING_H
#include <string>
#include <ostream>


class Standing {
    std::string acronym;
    int position;
    double gapToLeader;

public:
    Standing(const std::string& acronym_, int position_, double gapToLeader_);

    friend std::ostream& operator<<(std::ostream& os, const Standing& standing);
};


#endif //OOP_STANDING_H
