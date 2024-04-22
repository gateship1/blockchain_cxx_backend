#ifndef BLOCKCHAIN_HEADER_FILE
#define BLOCKCHAIN_HEADER_FILE

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "block.hpp"

struct Blockchain {

    Blockchain();

    auto get_end_of_chain() const -> Block;
    auto set_difficulty(const size_t&) -> void;
    auto set_max_iterations(const size_t&) -> void;
    auto get_difficulty() const -> size_t;
    auto get_max_iterations() const -> size_t;
    auto mine(const std::string&) -> bool;
    auto get_chain_length() const -> size_t;
    auto get_block(const size_t&) const -> Block;
    auto check_parent(const std::string&) const -> bool;

    // calculate the block fingerprint with SHA-256
    static auto calc_hash(const size_t &, const size_t &, const time_t &, const std::string &, const std::string &) -> std::string;

    private:
        std::vector<Block> blockchain;
        // difficulty is the preferred chain difficulty, sdifficulty is the difficulty set for the last successful mine
        size_t difficulty, sdifficulty;
        size_t max_iterations;
        auto genesis_block_generation() -> void;
        auto add_block(Block&, const std::string&) -> bool;
        auto check_proof(const Block&, const std::string&) const -> bool;
        auto proof_of_work(size_t&, const size_t&, const time_t&, const std::string&, const std::string&) -> std::string;
        auto gen_solution_str() const -> std::string;

};

#endif // BLOCKCHAIN_HEADER_FILE
