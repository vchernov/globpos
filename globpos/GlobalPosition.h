#ifndef GLOBPOS_GLOBALPOSITION_H
#define GLOBPOS_GLOBALPOSITION_H

#include <chrono>

namespace globpos {

template<class T>
struct GlobalPosition {
    int id;
    T latitude;
    T longitude;
    std::chrono::time_point<std::chrono::system_clock> timestamp;
};

using GlobPosDeg = GlobalPosition<int>; // coordinates in fixed-point degrees

struct GpsCoordDegMin {
    bool direction; // if true then North or East else South or West
    unsigned int deg; // degrees
    unsigned int minInt; // integer part of minutes
    unsigned int minFract; // fractional part of minutes
};

using GlobPosDegMin = GlobalPosition<GpsCoordDegMin>; // coordinates with separate degrees and minutes

}

#endif //GLOBPOS_GLOBALPOSITION_H
