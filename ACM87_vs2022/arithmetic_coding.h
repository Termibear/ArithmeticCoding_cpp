#pragma once
#include <iostream>
#include <fstream>
// Declarations used for arithmetic encoding and decoding.

// Size of arithmetic code values.

constexpr auto CODE_VALUE_BITS = 16;                                    // Number of bits in a code value
using CODE_VALUE = long;                                                // Type of an arithmetic code value

constexpr CODE_VALUE TOP_VALUE = (((long)1 << CODE_VALUE_BITS) - 1);    // Largest code value

// Half and quarter points in the code value range.
constexpr CODE_VALUE FIRST_QUARTER = (TOP_VALUE / 4 + 1);               // Point after first quarter
constexpr CODE_VALUE HALF = (2 * FIRST_QUARTER);                        // Point after first half
constexpr CODE_VALUE THIRD_QUARTER = (3 * FIRST_QUARTER);               // Point after third quarter

constexpr auto MAX_FREQUENCY = 16383;                                   // Maximum allowed frequency count

constexpr auto NO_OF_CHARS = 256;                                       // Number of character symbols
constexpr auto EOF_SYMBOL = (NO_OF_CHARS + 1);                          // Index of EOF symbol
constexpr auto NO_OF_SYMBOLS = (NO_OF_CHARS + 1);                       // Total number of symbols

// The adaptive source model.
struct Arithmetic_Coding
{
    unsigned char index_to_char[NO_OF_SYMBOLS];                         // To character from index
    int char_to_index[NO_OF_CHARS];                                     // To index from character
    int cum_freq[NO_OF_SYMBOLS + 1];                                    // Cumulative symbol frequencies
    int freq[NO_OF_SYMBOLS + 1];                                        // Symbol frequencies

    Arithmetic_Coding();                                                // Initialize the model.
    ~Arithmetic_Coding() = default;
    void update_tables(int symbol);                                     // Update the model to account for a new symbol.
};