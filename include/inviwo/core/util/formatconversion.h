#ifndef IVW_FORMATCONVERSION_H
#define IVW_FORMATCONVERSION_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <string>
#include <sstream>

namespace inviwo {

    #define BYTE_SWAP 1024

    static const uint64_t BYTE_SIZE = 1;
    static const uint64_t KILO_BYTE_SIZE = BYTE_SWAP * BYTE_SIZE;
    static const uint64_t MEGA_BYTE_SIZE = BYTE_SWAP * KILO_BYTE_SIZE;
    static const uint64_t GIGA_BYTE_SIZE = BYTE_SWAP * MEGA_BYTE_SIZE;
    static const uint64_t TERA_BYTE_SIZE = BYTE_SWAP * GIGA_BYTE_SIZE;
    static const float BYTE_DIV = 1.f/BYTE_SWAP;

    #define BYTES_TO_KILOBYTES(bytes) (bytes/BYTE_SWAP);
    #define BYTES_TO_MEGABYTES(bytes) (bytes/(BYTE_SWAP*BYTE_SWAP));
    #define KILOBYTES_TO_BYTES(bytes) (bytes*BYTE_SWAP);
    #define MEGABYTES_TO_BYTES(bytes) (bytes*BYTE_SWAP*BYTE_SWAP);

    static IVW_CORE_API inline std::string formatBytes(uint64_t bytes) {
        std::ostringstream stream;
        stream.precision(2);
        stream.setf(std::ios::fixed, std::ios::floatfield);

        if(bytes > TERA_BYTE_SIZE)
            stream << static_cast<float>(bytes/GIGA_BYTE_SIZE)*BYTE_DIV << " TB";
        else if(bytes > GIGA_BYTE_SIZE)
            stream << static_cast<float>(bytes/MEGA_BYTE_SIZE)*BYTE_DIV << " GB";
        else if(bytes > MEGA_BYTE_SIZE)
            stream << static_cast<float>(bytes/KILO_BYTE_SIZE)*BYTE_DIV << " MB";
        else if(bytes > KILO_BYTE_SIZE)
            stream << static_cast<float>(bytes)*BYTE_DIV << " KB";
        else
            stream << static_cast<float>(bytes) << " B";

        return stream.str();
    };

} // namespace

#endif // IVW_FORMATCONVERSION_H
