#include "cel/eng/misc_util.hpp"

#include "cel/eng/log.hpp"

#include <stdexcept>
#include <fstream>
#include <thread>

namespace cel {

    std::string read_entire_file(const std::string &filepath) {
        if (std::ifstream in{filepath, std::ios::binary | std::ios::ate}) {
            auto size = in.tellg();
            std::string str;
            str.resize(size);

            in.seekg(0);
            if (in.read(str.data(), size)) return str;
            // if this ^ is false, execution will fall through and an exception is thrown.
        }

        CEL_CRITICAL("Failed to open and read: {}", filepath);
        throw std::runtime_error{"Failed to read file"};
    }

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
    }

    uint8_t *utf8_codepoint_encode(uint8_t *in, uint32_t codepoint) {
        CEL_IGNORE(in);
        CEL_IGNORE(codepoint);
        throw std::runtime_error("utf8_codepoint_encode is unimplemented");
    }

    fps_limiter::fps_limiter(float target) : target_frametime_nano(1000000000 / target), target_fps(target) {}

    void fps_limiter::tick() {
        auto now = std::chrono::steady_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::duration<uint64_t, std::nano>>(now - prev_tick).count();

        if (diff <  target_frametime_nano) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(target_frametime_nano - diff));
            prev_tick += std::chrono::nanoseconds(target_frametime_nano);
            fps = target_fps;
        } else {
            fps = 1000000000.0f / diff;
            prev_tick = now;
        }

    }

    raw_stack::raw_stack(size_t size) : capacity(size) {
        store = new uint8_t[size];
        sp = store;
    }

    raw_stack::~raw_stack() {
        free(store);
    }

    raw_stack::raw_stack(raw_stack &&rhs) noexcept {
        *this = std::move(rhs);
    }

    raw_stack &raw_stack::operator=(raw_stack &&rhs) noexcept {
        if (this == &rhs) return *this;
        free(store);
        store = rhs.store;
        sp = rhs.sp;
        capacity = rhs.capacity;
        
        rhs.capacity = 0;
        rhs.store = nullptr;
        rhs.sp = nullptr;
        return *this;
    }

}