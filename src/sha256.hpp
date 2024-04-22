#ifndef SHA256_HEADER_FILE
#define SHA256_HEADER_FILE

#include <bitset>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#if (UNITTEST)
    #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#else
    #define DOCTEST_CONFIG_DISABLE
#endif
#include "doctest.h"

struct SHA256 {   
    
    // methods
    SHA256(const std::string&);
    ~SHA256();
       
    // use to output message to terminal
    auto display_message(const size_t&) const -> void;
    
    // display the message in binary
    auto display_block_in_binary(const uint32_t&) const -> std::string;
    
    // display the block in hexidecimal
    auto display_block_in_hex(const uint32_t&) const -> std::string;
    
    auto operator[](const size_t&) -> uint64_t&;
    
    // compute the message hash
    auto compute_digest() -> std::string;   
    
    private:
        uint64_t* M; // message data block
        size_t size; // length of the message in bits bit blocks
        size_t bits; // number of bits per block

        uint32_t H[8]; // SHA-256 hash values (store in hex)
        static constexpr uint32_t K[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
            0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
            0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
            0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
            0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
            0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
            0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
            0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
            0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };
            
    protected:
        // determine the size of the block array by determining how many 512 bit
        // blocks are required to hold the message with padding
        inline auto l_calc(const size_t&) const -> size_t;
                
        // determine number of zeroes with which to pad the message
        inline auto k_calc(const size_t&) const -> size_t; 
        
        // convert the message (an ASCII string) to its bit representation
        auto msg_to_bits(const std::string&) -> size_t;
        
        // pad the message to the next 512 bit block with '0's
        auto pad(const size_t&) -> size_t;
        
        // append message length in bits to the end of the message block
        auto append_msg_length(const size_t&, const size_t&) -> size_t;
        
        // parse the message from 8 bit blocks to 32 bit blocks
        auto parse_padded_msg_block() -> void;
        
        // preprocess the message
        auto preprocess(const std::string&, const size_t&, const size_t&) -> void;
        
        // logical functions in SHA-256
        inline auto rotr(const uint32_t&, const uint32_t&) const -> uint32_t;
        inline auto Ch(const uint32_t&, const uint32_t&, const uint32_t&) const -> uint32_t;
        inline auto Maj(const uint32_t&, const uint32_t&, const uint32_t&) const -> uint32_t;
        inline auto Sigma0(const uint32_t&) const -> uint32_t;
        inline auto Sigma1(const uint32_t&) const -> uint32_t;
        inline auto sigma0(const uint32_t&) const -> uint32_t;
        inline auto sigma1(const uint32_t&) const -> uint32_t;
        
};

#endif // SHA256_HEADER_FILE
