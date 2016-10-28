#ifndef GLOBPOS_NMEAPARSER_H
#define GLOBPOS_NMEAPARSER_H

#include <memory>

#include "settings.h"
#include "NmeaSentence.h"

namespace globpos {

/**
 * NMEA 0183 protocol parser.
 */
class NmeaParser {
public:
    NmeaParser();
    virtual ~NmeaParser() = default;

    void reset();

    /// Set flag to verify checksum value before committing a sentence.
    void setVerifyChecksum(bool);

    void parse(char);
    void parse(const char* buffer, size_t length);

    const std::vector<std::shared_ptr<NmeaSentence>>& getSentences() const;
    void clearSentences();

private:
    enum State {
        S_SOS = 0, // start of sentence
        S_ADDRESS = 1, // talker + sentence identifiers
        S_DATA = 2,
        S_CHECKSUM = 3,
        S_END = 4
    };

    template<class T, size_t N>
    struct Buffer {
        T content[N];
        size_t length;

        void clear() {
            length = 0;
        }

        void add(T c) {
            if (length < N) {
                content[length++] = c;
            }
        }
    };

    struct Part {
        size_t offset;
        size_t size;
    };

    void clearCurrentSentence();
    bool verifyCurrentSentence();
    void commitCurrentSentence();

    std::vector<std::shared_ptr<NmeaSentence>> completeSentences;

    State state;

    Buffer<Part, DATA_BUFFER_SIZE> data;
    Part currentData;

    Buffer<char, MAX_SENTENCE_LENGTH> sentence;
    Buffer<char, CHECKSUM_LENGTH> checksum;
    size_t currentEnd;
    size_t addressLength;

    bool verifyChecksum = true;
};

}

#endif //GLOBPOS_NMEAPARSER_H
