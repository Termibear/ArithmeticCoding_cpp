#include "decode.h"
#include <sstream>

// Initialize decode values and bit input.
Decode::Decode() : value{ 0 }
{
    // Buffer starts out with no bits in it.
    buffer = 0;
    bits_to_go = 0;
    garbage_bits = 0;
    end_decoding = false;

    // Full code range.
    low = 0;
    high = TOP_VALUE;
}

// Input bits to fill the code value.
void Decode::start_decoding()
{
    value = 0;
    for (int i = 1; i <= CODE_VALUE_BITS; i++)
        value = 2 * value + input_bit();
}

// Decode a symbol from the stream.
void Decode::decode(const char *in_file, const char *out_file)
{
    // Open the files for encoding.
    in.open(in_file, std::ios::binary);
    out.open(out_file, std::ios::binary);

    if (!in || !out)
    {
        std::cerr << "Error: Could not open file.\n";
        return;
    }

    start_decoding();

    // Loop through characters.
    while (true)
    {
        // Decode next symbol.
        int symbol = decode_symbol();
        // Exit loop if EOF symbol.
        if ((symbol == EOF_SYMBOL) || end_decoding)
        {
            std::cout << "Decoding finished" << std::endl;
            break;
        }
        // Translate to a character.
        int ch = index_to_char[symbol];

        // Write that character.
        out.put(static_cast<char>(ch));
        // Update the model.
        update_tables(symbol);
    }
    
    in.close();
    out.close();
}

// Decodes a symbol.
int Decode::decode_symbol()
{
    // Symbol decoded.
    int symbol;
    // Size of current code region
    CODE_VALUE range = (CODE_VALUE)(high - low) + 1;
    // Find cum freq for value.
    int cum = (((long)(value - low) + 1) * cum_freq[0] - 1) / range;

    // Then find symbol.
    for (symbol = 1; cum_freq[symbol] > cum; symbol++);

    // Narrow the code region to that allotted to this symbol.
    high = low + (range * cum_freq[symbol - 1]) / cum_freq[0] - 1;
    low = low + (range * cum_freq[symbol]) / cum_freq[0];

    // Loop to get rid of bits.
    while (true)
    {
        // Expand low half.
        if (high < HALF)
        {
            // Nothing.
        }
        // Expand high half.
        else if (low >= HALF)
        {
            // Subtract offset to top.
            value -= HALF;
            low -= HALF;
            high -= HALF;
        }
        // Expand middle half.
        else if (low >= FIRST_QUARTER && high < THIRD_QUARTER)
        {
            // Subtract offset to middle.
            value -= FIRST_QUARTER;
            low -= FIRST_QUARTER;
            high -= FIRST_QUARTER;
        }
        // Otherwise exit loop.
        else break;

        // Scale up code range.
        low = 2 * low;
        high = 2 * high + 1;
        // Move in next input bit.
        value = 2 * value + input_bit();
    }

    return symbol;
}

// Inputs a bit.
int Decode::input_bit()
{
    // Read the next byte if no bits are left in the buffer.
    if (bits_to_go == 0)
    {
        buffer = in.get();
        // Return arbitrary bits after EOF, but check for too many such.
        if (buffer == EOF)
        {
            garbage_bits++;
            if (garbage_bits > CODE_VALUE_BITS - 2)
            {
                std::cerr << "Bad input file, " << garbage_bits << " bits past EOF.\n";
                exit(-1);
            }
        }
        bits_to_go = 8;
    }

    // Return the next bit from the bottom of the byte.
    int t = buffer & 1;
    buffer >>= 1;
    bits_to_go--;
    return t;
}