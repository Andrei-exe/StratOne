//
// Created by Andrei on 17-Nov-26.
//

#include "../include/PitDuel.h"

PitDuel::PitDuel(const std::string &firstPitterAcronym_, const std::string &directRivalAcronym_, int firstPitterInLap_, int directRivalInLap_, double gapToRivalBeforeStop_, double gapToRivalAfterStop_) :
    firstPitterAcronym(firstPitterAcronym_), directRivalAcronym(directRivalAcronym_), firstPitterInLap(firstPitterInLap_), directRivalInLap(directRivalInLap_),
    gapToRivalBeforeStop(gapToRivalBeforeStop_), gapToRivalAfterStop(gapToRivalAfterStop_) {}

double PitDuel::attackerGap(double gap) const {
    if (isUndercut()) {
        return gap;
    }
    return -gap;
}

bool PitDuel::isUndercut() const {
    if (gapToRivalBeforeStop < 0) {
        return true;
    }
    return false;
}

bool PitDuel::worked() const {
    if ((gapToRivalBeforeStop < 0 && gapToRivalAfterStop > 0) || (gapToRivalBeforeStop > 0 && gapToRivalAfterStop < 0)) {
        return true;
    }
    return false;
}

bool PitDuel::isEarlierThan(const PitDuel &duel) const {
    return firstPitterInLap < duel.firstPitterInLap;
}

std::ostream & operator<<(std::ostream &os, const PitDuel &pitDuel) {
    os << "Lap " << pitDuel.firstPitterInLap << ": ";
    if (pitDuel.isUndercut()) {
        os << pitDuel.firstPitterAcronym << " undercut on " << pitDuel.directRivalAcronym;
    } else {
        os << pitDuel.directRivalAcronym << " overcut on " << pitDuel.firstPitterAcronym;
    }

    os << " (stops lap " << pitDuel.firstPitterInLap << " vs lap " << pitDuel.directRivalInLap << "), gap "
    << pitDuel.attackerGap(pitDuel.gapToRivalBeforeStop) << "s -> " << pitDuel.attackerGap(pitDuel.gapToRivalAfterStop) << "s, ";

    if (pitDuel.worked()) {
        os << "worked";
    } else {
        os << "failed";
    }

    return os;
}
