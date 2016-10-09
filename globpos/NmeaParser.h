#ifndef GLOBPOS_NMEAPARSER_H
#define GLOBPOS_NMEAPARSER_H

#include <cstdlib>

namespace globpos {

class NmeaParser {
public:
    void parse(char c);
    void parse(const char* buffer, size_t len);

private:
    enum State {
        S_SOS = 0, // start of sentence
        S_ADDRESS = 1, // talker + sentence identifiers
        S_DATA = 2,
        S_CHECKSUM = 3,
        S_END = 4,
    };

    State state;
};

}

#endif //GLOBPOS_NMEAPARSER_H
