#include "wav_encoder.h"
#include "string.h"
#include "fstream"
#include "bitset"
#include "iostream"

#define BUFSIZE 16

wav_encoder::wav_encoder()
    : base_encoder()
{
}


wav_encoder::~wav_encoder()
{
}


wav_encoder::wav_encoder(const std::string& in_filename, const std::string& out_filename, const std::string& msg)
    : base_encoder(in_filename, out_filename, msg)
{
}


const bool wav_encoder::encode()
{
    std::fstream in_stream(input_file, std::ios::in | std::ios::binary);
    std::fstream out_stream(output_file, std::ios::out | std::ios::binary);

    unsigned long block_count = 0;
    std::string coding_str = message;
    while (!in_stream.eof())
    {
        char buff16[BUFSIZE];
        try {
            in_stream.read(buff16, BUFSIZE);
        }
        catch (std::istream::failure&) {
            std::cerr << "Reading troubles" << std::endl;
            throw;
        }
        if (block_count == 3)
        {
            std::bitset<8> msg = coding_str.length();
            std::bitset<8> buffer = buff16[1];
            for (int i = 0; i < 8; i++)
            {
                buffer[i] = msg[i];
            }
            buff16[2] = buffer.to_ulong();
        }
        if (block_count < coding_str.length() + 5 && block_count > 4)
        {
            std::bitset<8> msg = coding_str[block_count - 5];
            std::bitset<8> buffer = buff16[1];
            for (int i = 0; i < 8; i++)
            {
                buffer[i] = msg[i];
            }
            buff16[2] = buffer.to_ulong();
        }
        try {
            out_stream.write(buff16, BUFSIZE);
        }
        catch (std::istream::failure&) {
            std::cerr << "Writing troubles" << std::endl;
            throw;
        }
        block_count++;
    }
    return 0;
}
