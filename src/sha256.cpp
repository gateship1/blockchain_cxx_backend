#include "sha256.hpp"

SHA256::SHA256(const std::string& msg) {

    // determine message length in bits (8 bits per char)
    auto l{this->l_calc(msg.length())};
    // determine number of '0's needed to pad the message
    auto k{this->k_calc(l-((l+1+64)/512)*512)};
    // determine number of 8 bit blocks needed to store the message (must be multiple of 512 bits)
    auto N{msg.length()+1+(k-7)/8+8};
    // initialize data structure to hold message
    this->M = new uint64_t[N];
    this->size = N;
    this->bits = 8;

    this->preprocess(msg, l, k);
}

SHA256::~SHA256() {
    delete[] this->M;
}

auto SHA256::display_message(const size_t& end) const -> void {
    std::cout << "\n";
    std::cout << std::setw(4) << "Block  "
              << std::setw(8) << "Int" << "   "
              << std::setw(8) << "Binary" << "   "
              << std::setw(8) << "Hex" << "\n";
    for (size_t i{0}; i < end; i++) {
        std::cout << std::setw(4) << i+1 << "   " 
                  << std::setw(8) << this->M[i] << "   "
                  << std::setw(8) << this->display_block_in_binary(this->M[i]) << "   "
                  << std::setw(8) << this->display_block_in_hex(this->M[i]) << "\n";
    }
    return;
}

auto SHA256::display_block_in_binary(const uint32_t& char_block) const -> std::string {
    std::bitset<8> block_string(char_block);
    return block_string.to_string();
}

auto SHA256::display_block_in_hex(const uint32_t& char_block) const -> std::string {
    std::bitset<32> block_string(char_block);
    std::stringstream block_stream;
    block_stream << std::hex << std::setw(8) << std::setfill('0') << block_string.to_ulong();
    std::string hex_rep;
    block_stream >> hex_rep;
    return hex_rep;
}

auto SHA256::operator[](const size_t& index) -> uint64_t& {
      return M[index];
}

/* l_calc

  Purpose: determine message length (8 bits per ASCII character)

  Parameters: no_of_chars, number of characters in the message

  Return: length of the message in bits
*/
auto SHA256::l_calc(const size_t& no_of_chars) const -> size_t {
    return 8 * no_of_chars;
}

/* k_calc

  Purpose: determine number of "0" bits from l + 1 + k = 448 % 512

  Parameters: l, the size of the message in bits

  Return: number of "0" bits for padding 
*/
auto SHA256::k_calc(const size_t& l) const -> size_t {
    return 447 - l;
}

/* msg_to_bits

  Purpose: Convert the message into an array of bit representation
           (Section 5.1.1 preprocessing)

  Parameters: msg, message to convert

  Return: the length of the message

  Side effects: stores bits representing the original message in the 
                hash data structure
*/
auto SHA256::msg_to_bits(const std::string& msg) -> size_t {

    // convert each ASCII character to its binary representation
    size_t i{0}; size_t len{msg.length()};
    for (; i < len; ++i) {
        std::bitset<8> binary_rep(msg.c_str()[i]);
        this->M[i] = binary_rep.to_ulong();
    }
    return i;
}

/* pad

  Purpose: Pad the message with '0's to end of the last 512 bit block
           (Section 5.1.1 preprocessing)

  Parameters: current_index, the current index in the array storing the message

  Return: the updated index in the array storing the message

  Side effects: the binary representation of the message with padding is 
                stored in the hash data structure
*/
auto SHA256::pad(const size_t& current_index) -> size_t {

    auto i{current_index};
    auto l{this->l_calc(i-1)};     // determine message length in bits (8 bits per char)

    size_t nblocks{(l+1+64)/512};  // number of 512 bit blocks over 1 required

    auto k{this->k_calc(l-nblocks*512)}; // determine padding from message length that is < 512 bits

    // pad msg with zeroes
    for (size_t j{0}; j < (k-7)/8; ++j) {// zeroes are added 8 bits at a time
       this->M[i++] = 0x00000000;
    }

    return i;
}

/* append_msg_length

  Purpose: append length of message in bits to end of message hash data structure
           (Section 5.1.1 preprocessing)

  Parameters: current_index, the current index in the array storing the message
              l, the length of the message in bits

  Return: the updated index in the array storing the message

  Side effects: the binary representation of the message with padding is 
                stored in the hash data structure
*/
auto SHA256::append_msg_length(const size_t& current_index, const size_t& l) -> size_t {

    auto i{current_index};
    // append 64-bit block equal to the number l in binary to the end of the message
    std::bitset<64> l_in_64bits(l);
    std::string l_str{l_in_64bits.to_string()}; // need to break l into 8 bit chunks
    for (size_t j{0}; j < 64; j += 8) {
        std::bitset<8> l_blocks(l_str.substr(j, 8));
        this->M[i++] = l_blocks.to_ulong();
    }

    return i;
}

/* parse_padded_msg_block

  Purpose: parse the padded message into N 32-bit blocks
           (preprocessing Section 5.2.1)

  Parameters: none

  Return: none

  Side effects: the binary representation of the message with padding is 
                stored in the hash data structure in 32 bit blocks
*/
auto SHA256::parse_padded_msg_block() -> void {
    // assign the data to each 32 bit block
    for (size_t i{0}; i < size; i +=4) {
        std::bitset<32> bitblock;
        // shift bits and OR with original bits for combining
        bitblock = static_cast<uint32_t>(this->M[i] << 24);
        bitblock |= static_cast<uint32_t>(this->M[i+1] << 16);
        bitblock |= static_cast<uint32_t>(this->M[i+2] << 8);
        bitblock |= static_cast<uint32_t>(this->M[i+3]);
        // store new 32 bit word in M
        this->M[i/4] = bitblock.to_ulong();
    }
    this->size /= 4; // resize hash array length
    this->bits = 32; // resize bits per block
    return;
}

/* preprocess

  Purpose: preprocess the message

  Parameters: msg, the message to preprocess
              l, length of the message in bits
              k, number of '0's in the padded message

  Return: none

  Side effects: the binary representation of the message with padding is 
                stored in the hash data structure
*/
auto SHA256::preprocess(const std::string& msg, const size_t& l, const size_t& k) -> void {

    // convert the msg to its binary representation and return the message length
    // the length is stored as the block_index
    size_t i{this->msg_to_bits(msg)};

    // append a "1" bit to the end of the message
    this->M[i++] = 0x80;

    // pad the message with "0s"
    i = pad(i);

    // append 64-bit block equal to the number l in binary to the end of the message
    i = append_msg_length(i, l);

    // parse the padded message into 32-bit blocks
    parse_padded_msg_block();

    // set initial hash value (Section 5.3.2)
    this->H[0] = 0x6a09e667; // a <- H_1^(i-1) where i == 1
    this->H[1] = 0xbb67ae85; // b <- H_2^(i-1)
    this->H[2] = 0x3c6ef372; // c <- H_3^(i-1)
    this->H[3] = 0xa54ff53a; // d <- H_4^(i-1)
    this->H[4] = 0x510e527f; // e <- H_5^(i-1)
    this->H[5] = 0x9b05688c; // f <- H_6^(i-1)
    this->H[6] = 0x1f83d9ab; // g <- H_7^(i-1)
    this->H[7] = 0x5be0cd19; // h <- H_8^(i-1)

    return;
}

auto SHA256::rotr(const uint32_t& x, const uint32_t& n) const -> uint32_t {
    return (x >> n) | (x << (32 - n));
}

auto SHA256::Ch(const uint32_t &x, const uint32_t &y, const uint32_t &z) const ->uint32_t {
    return (x & y) ^ (~(x) & z);
}

auto SHA256::Maj(const uint32_t& x, const uint32_t& y, const uint32_t& z) const ->uint32_t {
    return (x & (y | z)) | (y & z);
}

auto SHA256::Sigma0(const uint32_t& x) const -> uint32_t {
    return this->rotr(x, 2) ^ this->rotr(x, 13) ^ this->rotr(x, 22);
}

auto SHA256::Sigma1(const uint32_t& x) const-> uint32_t {
    return this->rotr(x, 6) ^ this->rotr(x, 11) ^ this->rotr(x, 25);
}

auto SHA256::sigma0(const uint32_t& x) const-> uint32_t {
    return this->rotr(x, 7) ^ this->rotr(x, 18) ^ (x >> 3);
}

auto SHA256::sigma1(const uint32_t& x) const-> uint32_t {
    return this->rotr(x, 17) ^ this->rotr(x, 19) ^ (x >> 10);
}

/* compute_digest

  compute the message hash

  Note: anywhere 0xFFFFFFFF is used is to ensure 32-bit
*/
auto SHA256::compute_digest() -> std::string {

    auto N{size};
    size_t k{0}; // continuously count through M
    for (size_t i{1}; i <= N*bits/512; ++i) {
        // prepare the message schedule
        uint32_t W[64];
        for (size_t t{0}; t < 16; ++t, ++k) {
            W[t] = this->M[k] & 0xFFFFFFFF;
        }
        for (size_t t{16}; t < 64; ++t) {
            W[t] = (this->sigma1(W[t-2])+W[t-7]+this->sigma0(W[t-15])+W[t-16]) & 0xFFFFFFFF;
        }
        auto a{this->H[0]};
        auto b{this->H[1]};
        auto c{this->H[2]};
        auto d{this->H[3]};
        auto e{this->H[4]};
        auto f{this->H[5]};
        auto g{this->H[6]};
        auto h{this->H[7]};

        for (size_t t{0}; t < 64; ++t) {

            auto T1{h+this->Sigma1(e)+this->Ch(e,f,g)+this->K[t]+W[t]};
            auto T2{this->Sigma0(a)+this->Maj(a,b,c)};
            h = g;
            g = f;
            f = e;
            e = (d+T1) & 0xFFFFFFFF; 
            d = c;
            c = b;
            b = a;
            a = (T1+T2) & 0xFFFFFFFF;
        }

        // compute the ith intermedate hash value H^(i) (ensure 32-bits)
        this->H[0] = (this->H[0]+a) & 0xFFFFFFFF;
        this->H[1] = (this->H[1]+b) & 0xFFFFFFFF;
        this->H[2] = (this->H[2]+c) & 0xFFFFFFFF;
        this->H[3] = (this->H[3]+d) & 0xFFFFFFFF;
        this->H[4] = (this->H[4]+e) & 0xFFFFFFFF;
        this->H[5] = (this->H[5]+f) & 0xFFFFFFFF;
        this->H[6] = (this->H[6]+g) & 0xFFFFFFFF;
        this->H[7] = (this->H[7]+h) & 0xFFFFFFFF;
    }

    return this->display_block_in_hex(H[0]) +
           this->display_block_in_hex(H[1]) +
           this->display_block_in_hex(H[2]) +
           this->display_block_in_hex(H[3]) +
           this->display_block_in_hex(H[4]) +
           this->display_block_in_hex(H[5]) +
           this->display_block_in_hex(H[6]) +
           this->display_block_in_hex(H[7]);
}

/******************************************************************************
 UNIT TESTING WITH DOCTEST
******************************************************************************/
// specification file can be found: https://csrc.nist.gov/publications/fips#fips180-4
TEST_CASE("Secure Hash Standard Test") {
    SUBCASE("abc") { // test found in 180-4
        std::string message{"abc"};
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    }
    SUBCASE("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq") { // test found in 180-4
        std::string message{"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
    }
    SUBCASE("1 million 'a's") { // test found in 180-2
        std::string message;
        for (size_t i{0}; i < 1000000; ++i) message+= "a";
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0");
    }
    // following tests found: https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA2_Additional.pdf
    SUBCASE("1 byte 0xbd") {
        uint8_t imessage{0xbd};
        auto convert_message_to_hex = [](auto n){
            std::stringstream ss;
            ss << n;
            return ss.str();
        };
        auto message{convert_message_to_hex(imessage)};
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "68325720aabd7c82f30f554b313d0570c95accbb7dc4b5aae11204c08ffe732b");
    }
    SUBCASE("4 bytes 0xc98c8e55") {
        uint8_t imessage[]{0xc9, 0x8c, 0x8e, 0x55};
        std::string message;
        message.assign(imessage, imessage + 4);
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "7abc22c0ae5af26ce93dbb94433a0e0b2e119d014f8e7f65bd56c61ccccd9504");
    }
    SUBCASE("55 bytes of zeros") {
        size_t array_size{55};
        uint8_t* imessage = new uint8_t[array_size];
        std::memset(imessage, 0x00, array_size);
        std::string message;
        message.assign(imessage, imessage + array_size);
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "02779466cdec163811d078815c633f21901413081449002f24aa3e80f0b88ef7");
        delete[] imessage;
    }
    SUBCASE("56 bytes of zeros") {
        size_t array_size{56};
        uint8_t* imessage = new uint8_t[array_size];
        std::memset(imessage, 0x00, array_size);
        std::string message;
        message.assign(imessage, imessage + array_size);
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "d4817aa5497628e7c77e6b606107042bbba3130888c5f47a375e6179be789fbb");
        delete[] imessage;
    }
    SUBCASE("57 bytes of zeros") {
        size_t array_size{57};
        uint8_t* imessage = new uint8_t[array_size];
        std::memset(imessage, 0x00, array_size);
        std::string message;
        message.assign(imessage, imessage + array_size);
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "65a16cb7861335d5ace3c60718b5052e44660726da4cd13bb745381b235a1785");
        delete[] imessage;
    }
    SUBCASE("64 bytes of zeros") {
        size_t array_size{64};
        uint8_t* imessage = new uint8_t[array_size];
        std::memset(imessage, 0x00, array_size);
        std::string message;
        message.assign(imessage, imessage + array_size);
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "f5a5fd42d16a20302798ef6ed309979b43003d2320d9f0e8ea9831a92759fb4b");
        delete[] imessage;
    }
    SUBCASE("1000 bytes of zeros") {
        size_t array_size{1000};
        uint8_t* imessage = new uint8_t[array_size];
        std::memset(imessage, 0x00, array_size);
        std::string message;
        message.assign(imessage, imessage + array_size);
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "541b3e9daa09b20bf85fa273e5cbd3e80185aa4ec298e765db87742b70138a53");
        delete[] imessage;
    }
    SUBCASE("1000 bytes of 0x41 A") {
        size_t array_size{1000};
        uint8_t* imessage = new uint8_t[array_size];
        std::memset(imessage, 0x41, array_size);
        std::string message;
        message.assign(imessage, imessage + array_size);
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "c2e686823489ced2017f6059b8b239318b6364f6dcd835d0a519105a1eadd6e4");
        delete[] imessage;
    }
    SUBCASE("1005 bytes of 0x55 U") {
        size_t array_size{1005};
        uint8_t* imessage = new uint8_t[array_size];
        std::memset(imessage, 0x55, array_size);
        std::string message;
        message.assign(imessage, imessage + array_size);
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "f4d62ddec0f3dd90ea1380fa16a5ff8dc4c54b21740650f24afc4120903552b0");
        delete[] imessage;
    }
    SUBCASE("1000000 bytes of zeros") {
        size_t array_size{1000000};
        uint8_t* imessage = new uint8_t[array_size];
        std::memset(imessage, 0x00, array_size);
        std::string message;
        message.assign(imessage, imessage + array_size);
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "d29751f2649b32ff572b5e0a9f541ea660a50f94ff0beedfb0b692b924cc8025");
        delete[] imessage;
    }
#if ENABLE_LONG_TESTS
    // these are the tests that have very large strings and take some time to run
    SUBCASE("0x20000000 (536870912) bytes of 0x5a `Z`") {
        size_t array_size{536870912};
        uint8_t* imessage = new uint8_t[array_size];
        std::memset(imessage, 0x5a, array_size);
        std::string message;
        message.assign(imessage, imessage + array_size);
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "15a1868c12cc53951e182344277447cd0979536badcc512ad24c67e9b2d4f3dd");
        delete[] imessage;
    }
    SUBCASE("0x41000000 (1090519040) bytes of zeros") {
        size_t array_size{1090519040};
        uint8_t* imessage = new uint8_t[array_size];
        std::memset(imessage, 0x00, array_size);
        std::string message;
        message.assign(imessage, imessage + array_size);
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "461c19a93bd4344f9215f5ec64357090342bc66b15a148317d276e31cbc20b53");
        delete[] imessage;
    }
    SUBCASE("0x6000003e (1610612798) bytes of 0x42 `B`") {
        size_t array_size{1610612798};
        uint8_t* imessage = new uint8_t[array_size];
        std::memset(imessage, 0x42, array_size);
        std::string message;
        message.assign(imessage, imessage + array_size);
        auto digest{SHA256(message).compute_digest()};
        CHECK(digest == "c23ce8a7895f4b21ec0daf37920ac0a262a220045a03eb2dfed48ef9b05aabea");
        delete[] imessage;
    }
#endif // ENABLE_LONG_TESTS
}
