#include "encode.h"
#include "decode.h"
#include <sstream>
#include <filesystem>

int main(int argc, char** argv)
{
    // Check for correct number of arguments.
    if (argc > 4 || argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << "[e/d/v] <in_file> <out_file>" << std::endl;
        return -1;
    }

    // Encoding.
    if (argv[1][0] == 'e')
    {
        Encode obj;
        obj.encode(argv[2], argv[3]);

        // Check compression ratio
        std::filesystem::path in_file = argv[2], out_file = argv[3];

        auto in_size = std::filesystem::file_size(in_file);
        auto out_size = std::filesystem::file_size(out_file);

        std::cout << argv[2] << " size = " << in_size << " bytes" << std::endl;
        std::cout << argv[3] << " size = " << out_size << " bytes" << std::endl;
        std::cout << "File size is currently " << (float)out_size / (float)in_size * 100.0f << "% of original file." << std::endl;
    }

    // Decoding.
    if (argv[1][0] == 'd')
    {
        Decode obj;
        obj.decode(argv[2], argv[3]);
    }

    // Verifying.
    if (argv[1][0] == 'v')
    {
        std::ifstream in1(argv[2], std::ios::binary), in2(argv[3], std::ios::binary);

        if (!in1 || !in2)
        {
            std::cerr << "Files could not open.\n";
            return -1;
        }

        std::ostringstream temp1, temp2;

        temp1 << in1.rdbuf();
        temp2 << in2.rdbuf();

        if (temp1.str() == temp2.str())
        {
            std::cout << "The files are the same" << std::endl;
            return 0;
        }
        else
        {
            std::cout << "The files are NOT the same" << std::endl;
            return 1;
        }
    }

    return 0;
}