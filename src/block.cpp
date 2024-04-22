#include <cstring>
#include <iostream>
#include <string>

#include "block.hpp"
#include "sha256.hpp"

Block::Block(const size_t& block_nonce, const size_t& id, const time_t& block_time, const std::string& parent, 
             const std::string& block_data, const std::string& block_hash) :
   index(id), data(block_data), timestamp(block_time), parent_hash(parent), nonce(block_nonce), hash(block_hash) {
}

auto Block::get_index() const -> size_t {
    return this->index;
}

auto Block::get_data() const -> std::string {
    return this->data;
}

auto Block::get_timestamp() const -> std::time_t {
    return this->timestamp;
}

auto Block::get_parent_hash() const -> std::string {
    return this->parent_hash;
}

auto Block::get_hash() const -> std::string {
    return this->hash;
}

auto Block::get_nonce() const -> size_t {
    return this->nonce;
}

// calculate the block fingerprint with SHA-256
auto Block::check_hash() const -> std::string {
    std::stringstream ss;
    ss << this->nonce << this->index << this->timestamp << this->parent_hash << this->data;
    return SHA256(ss.str()).compute_digest();
}

auto operator<<(std::ostream& os, const Block& block) -> std::ostream& {
    os << "Block id: " << block.get_index() << "\n"
       << "Block timestamp: " << block.get_timestamp() << "\n"
       << "Block parent: " << block.get_parent_hash() << "\n"
       << "Block data: " << block.get_data() << "\n"
       << "Block nonce: " << block.get_nonce() << "\n"
       << "Block signature: " << block.get_hash() << "\n";
    return os;
}
