#pragma once
#include "arithmetic_coding.h"

// Class for Arithmetic Decoding.
class Decode : public Arithmetic_Coding
{
    CODE_VALUE low, high;                                   // Ends of current code region.
    CODE_VALUE value;                                       // Currently-seen code value.

    int buffer;                                             // Bits waiting to be input.
    int bits_to_go;                                         // Number of bits still in buffer.
    int garbage_bits;                                       // Number of bits past EOF.

    bool end_decoding;                                      // Check if we are done decoding.

    std::ifstream in;                                       // File to be decoded.
    std::ofstream out;                                      // The decoded file.
public:
    Decode();                                               // Initialize decode values and bit input.
    ~Decode() = default;                                    // Default destructor.

    void start_decoding();                                  // Input bits to fill the code value.
    void decode(const char *in_file, const char *out_file); // Decodes symbols from the file, and creates a new decoded file.
    int decode_symbol();                                    // Decodes a symbol.
    int input_bit();                                        // Inputs a bit.
};