#include <cassert>
#include "Session.h"
#include "Statistics.h"

namespace {
    void testDriverReplacedBySameNumber() {
        Session session("test");
        session.addDriver(1, Driver(1, "VER", "Max VERSTAPPEN", "Red Bull Racing"));
        session.addDriver(1, Driver(1, "MAX", "Max VERSTAPPEN", "Red Bull Racing"));
        assert(session.findDriverByAcronym("MAX") != nullptr);
        assert(session.findDriverByAcronym("VER") == nullptr);
    }

    void testUnknownDriver() {
        Session session("test");
        session.addDriver(1, Driver(1, "VER", "Max VERSTAPPEN", "Red Bull Racing"));
        assert(session.findDriverByAcronym("XYZ") == nullptr);
    }

    void testLapComparison() {
        const Lap fast(1, 80.0, {}, {25.0, 30.0, 25.0}, 300, false);
        const Lap slow(2, 81.5, {}, {25.5, 30.5, 25.5}, 298, false);
        const Lap pitOut(3, 95.0, {}, {30.0, 35.0, 30.0}, 280, true);
        assert(fast.isFasterThan(slow) == true);
        assert(slow.isFasterThan(fast) == false);
        assert(fast.isRepresentative() == true);
        assert(pitOut.isRepresentative() == false);
    }

    void testStintBoundaries() {
        const Stint stint("MEDIUM", 0, 10, 20);
        assert(stint.isLapInStint(10) == true);
        assert(stint.isLapInStint(20) == true);
        assert(stint.isLapInStint(9) == false);
        assert(stint.isLapInStint(21) == false);
    }

    void testMedian() {
        assert(stats::median({}) == 0);
        assert(stats::median({3, 1, 2}) == 2);
        assert(stats::median({4, 1, 3, 2}) == 2.5);
    }

    void testOptimalStintLength() {
        const Strategy strategy(20.0, 50);
        assert(strategy.optimalStintLength(0.1) == 20);
        assert(strategy.optimalStintLength(0.001) == 50);
        assert(strategy.optimalStintLength(0) == 0);
    }

    void testPitStopComparison() {
        const PitStop quick(20, 22.0, 2.1);
        const PitStop slow(35, 24.0, 3.4);
        const PitStop noTime(40, 23.0, 0);
        assert(quick.isFasterThan(slow) == true);
        assert(slow.isFasterThan(quick) == false);
        assert(noTime.hasStopTime() == false);
    }

    void testPositionsGained() {
        assert(PositionChange("ALO", 9, 5).positionsGained() == 4);
        assert(PositionChange("STR", 10, 15).positionsGained() == -5);
    }
}

int main() {
    testDriverReplacedBySameNumber();
    testUnknownDriver();
    testLapComparison();
    testStintBoundaries();
    testMedian();
    testOptimalStintLength();
    testPitStopComparison();
    testPositionsGained();

    return 0;
}