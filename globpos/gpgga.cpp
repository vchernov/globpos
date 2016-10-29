#include "gpgga.h"

#include <cstring>

namespace globpos {

bool parseGPGGA(NmeaSentence* sentence, GlobPosDegMin& pos) {
    // quality
    std::string quality = sentence->safeGetData(5);
    if (!quality.empty()) {
        int qualityVal = 0;
        sscanf(quality.c_str(), "%d", &qualityVal);
        if (qualityVal < 1) {
            return false;
        }
    } else {
        return false;
    }

    // time
    std::string time = sentence->safeGetData(0);
    if (!time.empty()) {
        if (!parseTime(time.c_str(), pos.timestamp)) {
            return false;
        }
    } else {
        return false;
    }

    // latitude
    std::string latitude = sentence->safeGetData(1);
    if (!latitude.empty()) {
        if (!parseLatitude(latitude, pos.latitude)) {
            return false;
        }
    } else {
        return false;
    }
    std::string latitudeDir = sentence->safeGetData(2);
    if (!latitudeDir.empty()) {
        if (latitudeDir == "N")
            pos.latitude.direction = true;
        else if (latitudeDir == "S")
            pos.latitude.direction = false;
        else {
            return false;
        }
    } else {
        return false;
    }

    // longitude
    std::string longitude = sentence->safeGetData(3);
    if (!longitude.empty()) {
        if (!parseLongitude(longitude, pos.longitude)) {
            return false;
        }
    } else {
        return false;
    }
    std::string longitudeDir = sentence->safeGetData(4);
    if (!longitudeDir.empty()) {
        if (longitudeDir == "E")
            pos.longitude.direction = true;
        else if (longitudeDir == "W")
            pos.longitude.direction = false;
        else {
            return false;
        }
    } else {
        return false;
    }

    return true;
}

bool parseLatitude(const std::string& str, GpsCoordDegMin& coord) {
    return sscanf(str.c_str(), "%02d%02d.%d", &coord.deg, &coord.minInt, &coord.minFract) == 3;
}

bool parseLongitude(const std::string& str, GpsCoordDegMin& coord) {
    return sscanf(str.c_str(), "%03d%02d.%d", &coord.deg, &coord.minInt, &coord.minFract) == 3;
}

bool parseDateTime(const char* str, std::chrono::time_point<std::chrono::system_clock>& timestamp) {
    std::tm tm;
    memset(&tm, 0, sizeof(tm));

    long millis;
    int filled = sscanf(str, "%04d%*c%02d%*c%02d%*c%02d%*c%02d%*c%02d%*c%06ld",
                        &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
                        &tm.tm_hour, &tm.tm_min, &tm.tm_sec,
                        &millis);
    if (filled < 6) {
        return false;
    }

    tm.tm_mon -= 1;
    tm.tm_year -= 1900;
    if (tm.tm_year < 70) {
        tm.tm_year = 70;
    }

    time_t sec;
    if (tm.tm_year == 70 && tm.tm_mon == 0 && tm.tm_mday == 1) {
        sec = tm.tm_hour * 60 * 60 + tm.tm_min * 60 + tm.tm_sec;
    } else {
        sec = timegm(&tm);
    }

    timestamp = std::chrono::system_clock::from_time_t(sec);

    if (filled == 7) {
        timestamp += std::chrono::milliseconds(millis);
    }
    return true;
}

/// Searches for decimal point and returns number of fractional digits.
int fractDigitsCnt(const std::string& numStr) {
    size_t p = numStr.rfind('.');
    if (p == std::string::npos) {
        return 0;
    }
    return (int)(numStr.length() - p - 1);
}

/// Compile-time power computation.
template<int N, int P> struct Power
{
    enum
    {
        val = N * Power<N, P - 1>::val
    };
};
template<int N> struct Power<N, 0>
{
    enum
    {
        val = 1
    };
};

/// Run-time power of 10 computation.
long pow10(int p) {
    return (p == 0) ? 1 : 10 * pow10(p - 1);
}

bool parseTime(const char* str, std::chrono::time_point<std::chrono::system_clock>& timestamp) {
    int hours, min, sec;
    long millis;
    int filled = sscanf(str, "%02d%02d%02d.%ld", &hours, &min, &sec, &millis);
    if (filled < 3) {
        return false;
    }

    timestamp += std::chrono::hours(hours);
    timestamp += std::chrono::minutes(min);
    timestamp += std::chrono::seconds(sec);

    if (filled == 4) {
        int fractDigits = fractDigitsCnt(str);
        long order = Power<10, 6>::val / pow10(fractDigits);
        timestamp += std::chrono::milliseconds(millis * order);
    }
    return true;
}

}
