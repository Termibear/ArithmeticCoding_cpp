#include "arithmetic_coding.h"

// Initialize the model.
Arithmetic_Coding::Arithmetic_Coding()
{
    int i;
    // Set up tables that translate between symbol indexes and characters.
    for (i = 0; i < NO_OF_CHARS; i++)
    {
        char_to_index[i] = i + 1;
        index_to_char[i + 1] = static_cast<unsigned char>(i);
    }

    // Set up initial frequency counts to be 1 for all symbols.
    for (i = 0; i <= NO_OF_SYMBOLS; i++)
    {
        freq[i] = 1;
        cum_freq[i] = NO_OF_SYMBOLS - i;
    }

    // freq[0] must not be the same as freq[1].
    freq[0] = 0;
}

// Update the model to account for a new symbol.
void Arithmetic_Coding::update_tables(int symbol)
{
    int i;
    // See if frequency counts are at their maximum.
    if (cum_freq[0] == MAX_FREQUENCY)
    {
        int cum = 0;
        // If so, halve all the counts (keeping them non-zero).
        for (i = NO_OF_SYMBOLS; i >= 0; i--)
        {
            freq[i] = (freq[i] + 1) / 2;
            cum_freq[i] = cum;
            cum += freq[i];
        }
    }

    // Find symbol's new index.
    for (i = symbol; freq[i] == freq[i - 1]; i--);

    if (i < symbol)
    {
        int ch_i = index_to_char[i], ch_symbol = index_to_char[symbol];
        // Update the translation tables if the symbol has moved.
        index_to_char[i] = static_cast<unsigned char>(ch_symbol);
        index_to_char[symbol] = static_cast<unsigned char>(ch_i);
        char_to_index[ch_i] = symbol;
        char_to_index[ch_symbol] = i;
    }

    // Increment the frequency count for the symbol and update the cumulative frequencies.
    freq[i]++;
    while (i > 0)
    {
        i--;
        cum_freq[i]++;
    }
}