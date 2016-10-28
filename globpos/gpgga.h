#ifndef GLOBPOS_GPGGA_H
#define GLOBPOS_GPGGA_H

#include "NmeaSentence.h"
#include "GlobalPosition.h"

namespace globpos {

// GPGGA - Global Positioning System Fix Data
bool parseGPGGA(NmeaSentence* sentence, GlobPosDegMin& pos);

// parses latitude from string
bool parseLatitude(const std::string& str, GpsCoordDegMin& coord);

// parses longitude from string
bool parseLongitude(const std::string& str, GpsCoordDegMin& coord);

}

#endif //GLOBPOS_GPGGA_H
