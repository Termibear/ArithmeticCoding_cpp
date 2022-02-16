#pragma once
#include "arithmetic_coding.h"

// Class for Arithmetic Encoding.
class Encode : public Arithmetic_Coding
{
    CODE_VALUE low, high;                                   // Ends of current code region.

    CODE_VALUE bits_to_follow;                              // Number of opposite bits to output after the next bit.
    int buffer;                                             // Bits buffered for output.
    int bits_to_go;                                         // Number of bits free in buffer.

    std::ifstream in;                                       // Original file to be encoded.
    std::ofstream out;                                      // Encoded file.

public:
    Encode();                                               // Start encoding a stream of symbols.
    ~Encode() = default;

    void output_bit(int bit);                               // Outputs a bit.
    void bit_plus_follow(int bit);                          // Output bits plus following opposite bits.
    void done_encoding();                                   // Finish encoding the stream.
    void encode_symbol(int symbol);                         // Encodes a character.
    void encode(const char *in_file, const char *out_file); // Encodes characters from the file, and creates a new encoded file.
};