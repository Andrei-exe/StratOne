#include <cassert>
#include "Driver.h"

int main() {
    Driver testDriver(1, "VER", "Red Bull Racing");
    assert(testDriver.hasNumber(1) == true);
    assert(testDriver.hasNumber(3) == false);

    return 0;
}