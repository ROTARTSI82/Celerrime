#include "cel/eng/misc_util.hpp"

#include <stdexcept>

namespace cel {

    // this should probably be implemented with simple chained if/else but whatever.
    uint32_t utf8_codepoint_decode(uint8_t **in) {
        uint8_t *p = *in;
        uint8_t byte0 = *p++;

        if ((byte0 & 0b10000000) != 0) {
            // more than 1 byte!
            uint8_t shift = 0; // keep track of where the first byte is.
            uint32_t ret = byte0;

            // skip over 10 prefix as it's the continuation.
            uint8_t mask = 0b01000000;
            uint32_t collected = 0b11000000;
            
            do {
                mask >>= 1;
                collected |= mask;

                shift += 6;
                ret <<= 6;

                // ignore first 2 bits.
                ret |= *p++ & 0b00111111;
            } while ((byte0 & mask) != 0);

            *in = p;

            // mask out however many bits we have to ignore in the first byte.
            return ret & ~(collected << shift);
        } else {
            *in = p;
            return byte0;
        }
    };

    uint8_t *utf8_codepoint_encode(uint8_t *in, uint32_t codepoint) {

    };

    raw_stack::raw_stack(size_t size) {
        store = new uint8_t[size];
        sp = store;
    }

    raw_stack::~raw_stack() {
        free(store);
    }

    raw_stack::raw_stack(raw_stack &&rhs) noexcept {
        *this = std::move(rhs);
    };

    raw_stack &raw_stack::operator=(raw_stack &&rhs) noexcept {
        if (this == &rhs) return *this;
        free(store);
        store = rhs.store;
        sp = rhs.sp;
        
        rhs.store = nullptr;
        rhs.sp = nullptr;
    };

}