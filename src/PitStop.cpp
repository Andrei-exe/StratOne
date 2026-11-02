//
// Created by Andrei on 01-Nov-26.
//

#include "PitStop.h"

PitStop::PitStop(int inLap_, double pitlaneDuration_, double stopDuration_) :
    inLap(inLap_), pitlaneDuration(pitlaneDuration_), stopDuration(stopDuration_) {}

double PitStop::getPitLaneDuration() const {
    return pitlaneDuration;
}

bool PitStop::hasStopTime() const {
    if (stopDuration > 0) {
        return true;
    }
    return false;
}

bool PitStop::isFasterThan(const PitStop &pitstop) const {
    if (stopDuration < pitstop.stopDuration) {
        return true;
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const PitStop &pitStop) {
    os << "Pitted on lap " << pitStop.inLap << ": time in pits " << pitStop.pitlaneDuration << "s";
    if (pitStop.stopDuration > 0) {
        os << ", stop duration " << pitStop.stopDuration << "s";
    }
    else {
        os << ", stop duration -";
    }
    return os;
}
