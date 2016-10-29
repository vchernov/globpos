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

bool parseDateTime(const char* str, std::chrono::time_point<std::chrono::system_clock>& timestamp);
bool parseTime(const char* str, std::chrono::time_point<std::chrono::system_clock>& timestamp);

}

#endif //GLOBPOS_GPGGA_H
