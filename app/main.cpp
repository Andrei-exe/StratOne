#include "Session.h"
#include "Loader.h"
#include <iostream>

int main() {
    Loader loader("assets/netherlands_2023");
    Session session(loader.load("netherlands_2023"));
    std::cout<<session.idealFastestDriver()->idealLapTime()<<std::endl;

    return 0;
}
