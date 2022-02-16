# ArithmeticCoding_cpp

## Overview
This is an attempt to re-create the arithmetic algorithm code written down in C in the 1987 paper **Arithmetic Coding For Data Compression** by *Ian H. Witten*, *Radford M. Neal*, and *John G. Cleary*. Most of the code comments and function names are also taken from the original paper.

The basic description of the idea is as follows, quoted from [Wikipedia](https://en.wikipedia.org/wiki/Arithmetic_coding):
> **Arithmetic Coding** (**AC**) is a form of entropy encoding used in lossless data compression. Normally, a string of characters is represented using a fixed number of bits per character, as in the ASCII code. When a string is converted to arithmetic encoding, frequently used characters will be stored with fewer bits and not-so-frequently occurring characters will be stored with more bits, resulting in fewer bits used in total.