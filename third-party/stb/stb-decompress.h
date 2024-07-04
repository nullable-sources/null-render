#pragma once
#include <vector>
#include <stdexcept>

#define stb_impl_in2(x) ((i[x] << 8) + i[(x)+1])
#define stb_impl_in3(x) ((i[x] << 16) + stb_impl_in2((x)+1))
#define stb_impl_in4(x) ((i[x] << 24) + stb_impl_in3((x)+1))

namespace stb {
    inline std::uint8_t* barrier_out_e{ }, * barrier_out_b{ };
    inline const std::uint8_t* barrier_in_b{ };
    inline std::uint8_t* dout{ };

    static std::uint32_t decompress_length(const std::uint8_t* input) { return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11]; }
    static void match(const std::uint8_t* data, std::uint32_t length) {
        if(dout + length > barrier_out_e) throw std::runtime_error{ "dout + length > barrier_out_e" };
        if(data < barrier_out_b) { dout = barrier_out_e + 1; return; }
        while(length--) *dout++ = *data++;
    }

    static void lit(const std::uint8_t* data, std::uint32_t length) {
        if(dout + length > barrier_out_e) throw std::runtime_error{ "dout + length > barrier_out_e" };
        if(data < barrier_in_b) { dout = barrier_out_e + 1; return; }
        memcpy(dout, data, length);
        dout += length;
    }

    static const std::uint8_t* decompress_token(const std::uint8_t* i) {
        if(*i >= 0x20) {
            if(*i >= 0x80)       match(dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
            else if(*i >= 0x40)  match(dout - (stb_impl_in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
            else /* *i >= 0x20 */ lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
        } else {
            if(*i >= 0x18)       match(dout - (stb_impl_in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
            else if(*i >= 0x10)  match(dout - (stb_impl_in3(0) - 0x100000 + 1), stb_impl_in2(3) + 1), i += 5;
            else if(*i >= 0x08)  lit(i + 2, stb_impl_in2(0) - 0x0800 + 1), i += 2 + (stb_impl_in2(0) - 0x0800 + 1);
            else if(*i == 0x07)  lit(i + 3, stb_impl_in2(1) + 1), i += 3 + (stb_impl_in2(1) + 1);
            else if(*i == 0x06)  match(dout - (stb_impl_in3(1) + 1), i[4] + 1), i += 5;
            else if(*i == 0x04)  match(dout - (stb_impl_in3(1) + 1), stb_impl_in2(4) + 1), i += 6;
        }
        return i;
    }

    static std::uint32_t adler32(std::uint32_t adler32, std::uint8_t* buffer, std::uint32_t buflen) {
        constexpr std::uint64_t ADLER_MOD = 65521;
        std::uint64_t s1{ adler32 & 0xffff }, s2{ adler32 >> 16 };
        std::uint64_t blocklen{ buflen % 5552 };

        std::uint64_t i{ };
        while(buflen) {
            for(i = 0; i + 7 < blocklen; i += 8) {
                s1 += buffer[0], s2 += s1;
                s1 += buffer[1], s2 += s1;
                s1 += buffer[2], s2 += s1;
                s1 += buffer[3], s2 += s1;
                s1 += buffer[4], s2 += s1;
                s1 += buffer[5], s2 += s1;
                s1 += buffer[6], s2 += s1;
                s1 += buffer[7], s2 += s1;

                buffer += 8;
            }

            for(; i < blocklen; ++i)
                s1 += *buffer++, s2 += s1;

            s1 %= ADLER_MOD, s2 %= ADLER_MOD;
            buflen -= blocklen;
            blocklen = 5552;
        }
        return (std::uint32_t)(s2 << 16) + (std::uint32_t)s1;
    }

    static std::uint32_t decompress(std::uint8_t* output, const std::uint8_t* i) {
        if(stb_impl_in4(0) != 0x57bC0000) return 0;
        if(stb_impl_in4(4) != 0) return 0;
        const std::uint32_t olen{ decompress_length(i) };
        barrier_in_b = i;
        barrier_out_e = output + olen;
        barrier_out_b = output;
        i += 16;

        dout = output;
        for(;;) {
            const std::uint8_t* old_i{ i };
            i = decompress_token(i);
            if(i == old_i) {
                if(*i == 0x05 && i[1] == 0xfa) {
                    if(dout != output + olen) throw std::runtime_error{ "dout != output + olen" };
                    if(adler32(1, output, olen) != (std::uint32_t)stb_impl_in4(2))
                        return 0;
                    return olen;
                } else throw std::runtime_error{ "decompress error" };
            }

            if(dout > output + olen) throw std::runtime_error{ "dout > output + olen" };
        }
    }
}