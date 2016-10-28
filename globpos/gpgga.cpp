#include "gpgga.h"

#include <iostream>

namespace globpos {

bool parseGPGGA(NmeaSentence* sentence, GlobPosDegMin& pos) {
    // quality
    std::string quality = sentence->safeGetData(5);
    if (!quality.empty()) {
        int qualityVal = 0;
        sscanf(quality.c_str(), "%d", &qualityVal);
        if (qualityVal < 1) {
#ifdef GLOBPOS_PRINT_ERRORS
            fprintf(stdout, "Low signal quality: %d, skipping sentence\n", qualityVal);
#endif
            return false;
        }
    } else {
#ifdef GLOBPOS_PRINT_ERRORS
        fprintf(stderr, "Unknown signal quality\n");
#endif
        return false;
    }

    // time
    std::string time = sentence->safeGetData(0);
    if (!time.empty()) {
        //TODO: parse timestamp
    } else {
#ifdef GLOBPOS_PRINT_ERRORS
        fprintf(stderr, "Unknown time\n");
#endif
        return false;
    }

    // latitude
    std::string latitude = sentence->safeGetData(1);
    if (!latitude.empty()) {
        parseLatitude(latitude, pos.latitude);
    } else {
#ifdef GLOBPOS_PRINT_ERRORS
        fprintf(stderr, "Unknown latitude\n");
#endif
        return false;
    }
    std::string latitudeDir = sentence->safeGetData(2);
    if (!latitudeDir.empty()) {
        if (latitudeDir == "N")
            pos.latitude.direction = true;
        else if (latitudeDir == "S")
            pos.latitude.direction = false;
        else {
#ifdef GLOBPOS_PRINT_ERRORS
            fprintf(stderr, "Unknown latitude direction\n");
#endif
            return false;
        }
    } else {
#ifdef GLOBPOS_PRINT_ERRORS
        fprintf(stderr, "Unknown latitude direction\n");
#endif
        return false;
    }

    // longitude
    std::string longitude = sentence->safeGetData(3);
    if (!longitude.empty()) {
        parseLongitude(longitude, pos.longitude);
    } else {
#ifdef GLOBPOS_PRINT_ERRORS
        fprintf(stderr, "Unknown longitude\n");
#endif
        return false;
    }
    std::string longitudeDir = sentence->safeGetData(4);
    if (!longitudeDir.empty()) {
        if (longitudeDir == "E")
            pos.longitude.direction = true;
        else if (longitudeDir == "W")
            pos.longitude.direction = false;
        else {
#ifdef GLOBPOS_PRINT_ERRORS
            fprintf(stderr, "Unknown longitude direction\n");
#endif
            return false;
        }
    } else {
#ifdef GLOBPOS_PRINT_ERRORS
        fprintf(stderr, "Unknown longitude direction\n");
#endif
        return false;
    }

    return true;
}

bool parseCoordinate(const std::string& str, const char* format, GpsCoordDegMin& coord) {
    if (sscanf(str.c_str(), format, &coord.deg, &coord.minInt, &coord.minFract) == 3)
        return true;
    else {
#ifdef GLOBPOS_PRINT_ERRORS
        fprintf(stderr, "wrong coordinate string: %s\n", str.c_str());
#endif
        return false;
    }
}

bool parseLatitude(const std::string& str, GpsCoordDegMin& coord) {
    return parseCoordinate(str, "%02d%02d.%d", coord);
}

bool parseLongitude(const std::string& str, GpsCoordDegMin& coord) {
    return parseCoordinate(str, "%03d%02d.%d", coord);
}

}
