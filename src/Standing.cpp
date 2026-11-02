//
// Created by Andrei on 02-Nov-26.
//

#include "Standing.h"

Standing::Standing(const std::string &acronym_, int position_, double gapToLeader_) :
    acronym(acronym_), position(position_), gapToLeader(gapToLeader_) {}

std::ostream &operator<<(std::ostream &os, const Standing &standing) {
    os << "P" << standing.position  << " " << standing.acronym;
    if (standing.position == 1) {
        os << " Leader";
    }
    else {
        os << " +" << standing.gapToLeader << "s";
    }
    return os;
}
