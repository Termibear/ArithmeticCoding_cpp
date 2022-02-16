#include "encode.h"

// Start encoding a stream of symbols.
Encode::Encode()
{
    buffer = 0;
    bits_to_go = 8;

    low = 0;
    high = TOP_VALUE;
    bits_to_follow = 0;
}

// Encodes characters from the file, and creates a new encoded file.
void Encode::encode(const char *in_file, const char *out_file)
{
    // Open the files for encoding.
    in.open(in_file, std::ios::binary);
    out.open(out_file, std::ios::binary);

    if (!in || !out)
    {
        std::cerr << "Error: Could not open file.\n";
        return;
    }

    // Loop through characters.
    while (true)
    {
        // Read the next character.
        int ch = in.get();
        // Exit loop on EOF.
        if (ch == EOF)
        {
            std::cout << "Encoding finished" << std::endl;
            break;
        }

        // Translate to an index.
        int symbol = char_to_index[ch];
        // Encode that symbol.
        encode_symbol(symbol);
        // Update the model.
        update_tables(symbol);
    }

    // Encode the EOF symbol.
    encode_symbol(EOF_SYMBOL);
    // Send the last few bits.
    done_encoding();

    in.close();
    out.close();
}

// Encodes a character.
void Encode::encode_symbol(int symbol)
{
    // Narrow the code region to that allotted to this symbol.
    CODE_VALUE range = (CODE_VALUE)(high - low) + 1;
    high = low + (range * cum_freq[symbol - 1]) / cum_freq[0] - 1;
    low = low + (range * cum_freq[symbol]) / cum_freq[0];

    // Loop to output bits.
    while (true)
    {
        // Output 0 if in low half.
        if (high < HALF)
            bit_plus_follow(0);
        // Output 1 if in high half.
        else if (low >= HALF)
        {
            bit_plus_follow(1);
            low -= HALF;
            high -= HALF;
        }
        // Output an opposite bit later if in middle half.
        else if (low >= FIRST_QUARTER && high < THIRD_QUARTER)
        {
            bits_to_follow++;
            // Subtract offset to middle.
            low -= FIRST_QUARTER;
            high -= FIRST_QUARTER;
        }
        // Otherwise exit loop.
        else break;

        // Scale up code range.
        low = 2 * low;
        high = 2 * high + 1;
    }
}

// Finish encoding the stream.
void Encode::done_encoding()
{
    // Output 2 bits that select the quarter that the current code range contains.
    bits_to_follow++;
    if (low < FIRST_QUARTER)
        bit_plus_follow(0);
    else
        bit_plus_follow(1);

    // Flush out the last bits.
    out.put(static_cast<char>(buffer >> bits_to_go));
}

// Output bits plus following opposite bits.
void Encode::bit_plus_follow(int bit)
{
    // Output the bit.
    output_bit(bit);
    // Output bits_to_follow opposite bits. Set bits_to_follow to 0.
    while (bits_to_follow > 0)
    {
        // Same as output_bit(!bit)
        output_bit(bit ? 0 : 1);
        bits_to_follow--;
    }
}

// Outputs a bit.
void Encode::output_bit(int bit)
{
    // Put bit at the top of the buffer.
    buffer >>= 1;
    if (bit) buffer |= 0x80;
    bits_to_go--;

    // Output buffer if it is now full.
    if (bits_to_go == 0)
    {
        out.put(static_cast<char>(buffer));
        bits_to_go = 8;
    }
}