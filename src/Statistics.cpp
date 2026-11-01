//
// Created by Andrei on 01-Nov-26.
//

#include "Statistics.h"
#include <vector>
#include <algorithm>

double stats::median(std::vector<double> v) {
    if (v.empty()) {
        return 0;
    }
    std::ranges::sort(v);
    if (v.size() % 2 != 0) {
        return v[v.size() / 2];
    }
    return (v[v.size() / 2 - 1] + v[v.size() / 2]) / 2;
}
