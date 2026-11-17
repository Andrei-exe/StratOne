//
// Created by Andrei on 17-Nov-26.
//

#ifndef OOP_PITDUEL_H
#define OOP_PITDUEL_H
#include <string>
#include <ostream>

class PitDuel {
    std::string firstPitterAcronym;
    std::string directRivalAcronym;
    int firstPitterInLap;
    int directRivalInLap;
    double gapToRivalBeforeStop;
    double gapToRivalAfterStop;

    double attackerGap(double gap) const;

public:
    PitDuel(const std::string& firstPitterAcronym_, const std::string& directRivalAcronym_, int firstPitterInLap_, int directRivalInLap_, double gapToRivalBeforeStop_, double gapToRivalAfterStop_);

    bool isUndercut() const;
    bool worked() const;
    bool isEarlierThan(const PitDuel& duel) const;

    friend std::ostream& operator<<(std::ostream& os, const PitDuel& p);
};


#endif //OOP_PITDUEL_H
