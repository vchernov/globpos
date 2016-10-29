#include "../globpos/NmeaParser.h"

#include <iostream>
#include <iomanip>

#include "../globpos/gpgga.h"

using namespace globpos;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " NMEA_dump_file" << std::endl;
        return 1;
    }

    const char* fn = argv[1];

    FILE* f = fopen(fn, "rb");
    if (f == nullptr) {
        std::cerr << "Failed to open NMEA dump file: " << fn << std::endl;
        return 1;
    }

    std::chrono::time_point<std::chrono::system_clock> recordDate;
    parseDateTime("2016.10.29 00:00:00", recordDate);

    auto parser = std::make_unique<NmeaParser>();

    const size_t bufferLength = 1024;
    char buffer[bufferLength];
    while (!feof(f)) {
        size_t bytesRead = fread(buffer, sizeof(char), bufferLength, f);
        parser->parse(buffer, bytesRead);

        for (const auto& sentence : parser->getSentences()) {
            std::cout << sentence->address;

            // GPS coordinates
            if (sentence->address == "GPGGA") {
                GlobPosDegMin pos;
                pos.timestamp = recordDate;
                if (parseGPGGA(sentence.get(), pos)) {
                    time_t t = std::chrono::system_clock::to_time_t(pos.timestamp);
                    std::cout << " (" << std::put_time(std::gmtime(&t), "%Y.%m.%d %H:%M:%S") << ")";
                }
            }

            std::cout << std::endl;
        }
        parser->clearSentences();
    }

    fclose(f);
    return 0;
}
