#include "blockchain.hpp"
#include "sha256.hpp"

Blockchain::Blockchain() : difficulty(0), sdifficulty(0), max_iterations(10000) {
    this->genesis_block_generation();
}

/* genesis_block_generation

  Purpose: generate the first block in the chain (the genesis block)

  Parameters: None

  Return: None

  Side effects: genesis block is created and added to the chain
*/
auto Blockchain::genesis_block_generation() -> void {
    // initialize all block info
    const size_t index{0}; 
    const size_t nonce{0};  
    const time_t timestamp{time(nullptr)};
    const std::string parent{NULL}; // genesis block has no parent, hence NULL
    const std::string data{"Genesis"};
    // genesis block has hash of 64 0s
    const auto hash{"0000000000000000000000000000000000000000000000000000000000000000"};
    // create the genesis block
    auto block{Block(nonce, index, timestamp, parent, data, hash)};
    // add the genesis block to the chain
    this->blockchain.push_back(block);
    return;
}

/* add_block

  Purpose: add a block to the end of the chain

  Parameters: block, the block to add
              proof_hash, the proof of work hash for the block to add

  Return: True if the block is valid (and is added)
          False if the block is not valid (and is not added)

  Side effects: valid block is added to the chain
*/
auto Blockchain::add_block(Block& block, const std::string& proof_hash) -> bool {
  
    auto last_hash{this->get_end_of_chain().get_hash()};
  
    // check to make sure the parent hash for the block is the same
    // as the last block in the chain hash
    if (last_hash != block.get_parent_hash()) return false;
  
    // check the proof
    if (!(this->check_proof(block, proof_hash))) return false;
    this->blockchain.push_back(block);
  
    this->sdifficulty = this->difficulty; // set the successful difficulty = the difficulty
  
    return true;
}

auto Blockchain::set_difficulty(const size_t& ndifficult) -> void {
    this->difficulty = ndifficult;
}

auto Blockchain::set_max_iterations(const size_t& nmax_iterations) -> void {
    this->max_iterations = nmax_iterations;
}

auto Blockchain::get_difficulty() const -> size_t {
    return this->sdifficulty;
}

auto Blockchain::get_max_iterations() const -> size_t {
    return this->max_iterations;
}

/* gen_solution_str

  Purpose: generate the string of leading '0's from the difficulty,
           used to check proofs of work

  Parameters: None

  Return: the solution string of leading '0's

  Side effects: none
*/
auto Blockchain::gen_solution_str() const -> std::string {
    char* solution0s = new char[this->difficulty];
    std::memset(solution0s, '0', this->difficulty);
    std::string solution_str;
    solution_str.assign(solution0s, solution0s + this->difficulty);
    delete[] solution0s; 
    return solution_str;
}

/* check_proof

  Purpose: check the proof of work for validity

  Parameters: block, the block to check the proof of work against
              proof, the proof of work hash

  Return: true if the proof checks out,
          false otherwise

  Side effects: none
*/
auto Blockchain::check_proof(const Block& block, const std::string& proof) const -> bool {
    // initialize "solution string" (string of 0s matching the difficulty)  
    return ((proof == Blockchain::calc_hash(block.get_nonce(), block.get_index(), block.get_timestamp(), block.get_parent_hash(), block.get_data())) && 
            (proof.substr(0,this->difficulty) == gen_solution_str())) ? true : false;
}

/* proof_of_work

  Purpose: find a hash that meets the required difficulty

  Parameters: nonce, number used once
              index, the block index,
              timestamp, block mining timestamp
              parent_hash, the block's parent hash,
              data, teh data in the block

  Return: the hash meeting the difficulty

  Side effects: nonce is incremented once for each for loop cycle
*/
auto Blockchain::proof_of_work(size_t& nonce, const size_t& index, const time_t& timestamp, 
                               const std::string& parent, const std::string& data) -> std::string {
  
    auto proof_hash{Blockchain::calc_hash(nonce, index, timestamp, parent, data)}; // initial hash based on nonce of 0
    // initialize "solution string" (string of 0s matching the difficulty)
    auto solution_str{gen_solution_str()};
    for (;;) {
        // check to see if the hash meets the difficulty, if it does, break
        if (proof_hash.substr(0,this->difficulty) == solution_str) break;
        nonce++;
        // if the number of attempts exceeds the max number of iterations, break
        if (nonce > this->max_iterations) break;
        proof_hash = Blockchain::calc_hash(nonce, index, timestamp, parent, data);
    } 
    return proof_hash;
}

/* mine

  Purpose: mine a new block by determining the proof of work and adding it to the
           block chain

  Parameters: block, the block to be mined

  Return: true is mine is successful,
          false otherwise

  Side effects: the new block is added to the chain (if mine is successful)
*/
auto Blockchain::mine(const std::string& new_data) -> bool {
    // get the last block on the chain
    auto last_block{this->get_end_of_chain()};
  
    // potential new block info with the next index and input data
    const auto index{last_block.get_index()+1};
    size_t nonce{0};
    const time_t timestamp{time(nullptr)};
    const auto parent{last_block.get_hash()};
  
    // determine the proof of work for the new block
    auto proof_hash{this->proof_of_work(nonce, index, timestamp, parent, new_data)};
  
    // add the block to the chain
    auto new_block{Block(nonce, index, timestamp, parent, new_data, proof_hash)};
    return (this->add_block(new_block, proof_hash)) ? true : false;
}

auto Blockchain::get_end_of_chain() const -> Block {
    return this->blockchain.back();
}

/* calc_hash

  Purpose: to determine the block signature with SHA-256

  Parameters: nonce, number used once
              index, the block index,
              timestamp, block mining timestamp
              parent_hash, the block's parent hash,
              data, teh data in the block

  Return: the SHA-256 digest (signature) of the block data

  Side effects: None
*/
auto Blockchain::calc_hash(const size_t& nonce, const size_t& index, const time_t& timestamp, const std::string& parent_hash, const std::string& data) -> std::string {
    std::stringstream ss;
    ss << nonce << index << timestamp << parent_hash << data;
    return SHA256(ss.str()).compute_digest();
}


auto Blockchain::get_chain_length() const -> size_t {
    return this->blockchain.size();
}

auto Blockchain::get_block(const size_t& i) const -> Block {
    return this->blockchain[i];
}

/* check_parent

  Purpose: to determine if the parent hash of a block to be mined matches the 
           hash of the last chain on the block.
           Used to help prevent tampering.

  Parameters: parent_hash, the potential block's parent hash,

  Return: true if the parent_hash of the block to be mined matches the hash 
               of the last block on the chain
          false otherwise

  Side effects: None
*/
auto Blockchain::check_parent(const std::string& parent_hash) const -> bool {
    return (parent_hash == this->get_end_of_chain().get_hash()) ? true : false;
}
