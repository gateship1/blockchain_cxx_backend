#ifndef BLOCK_HEADER_FILE
#define BLOCK_HEADER_FILE

#include <ctime>
#include <string>
#include <sstream>

/* Block
  
  Purpose: data structue to store all necessary block information
*/
struct Block {
    
    Block(const size_t&, const size_t&, const time_t&, const std::string&, const std::string&, const std::string&);
    
    // getter functions
    auto get_index() const -> size_t;
    auto get_data() const -> std::string;
    auto get_timestamp() const -> std::time_t;
    auto get_parent_hash() const -> std::string;
    auto get_hash() const -> std::string;
    auto get_nonce() const -> size_t;
    auto check_hash() const -> std::string;
    
    friend auto operator<<(std::ostream&, const Block&) -> std::ostream&;
    
    private:
        const size_t index; // unique id for the block
        const std::string data; // data stored in the block
        const std::time_t timestamp; // time stamp of block generation
        const std::string parent_hash; // hash of the parent block in the blockchain
        const size_t nonce; // "number used once"
        const std::string hash; // block signature
};

#endif // BLOCK_HEADER_FILE
