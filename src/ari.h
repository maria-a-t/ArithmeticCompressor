#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <utility>


class BitStream {
private:
  unsigned char in_buff[8192] = {0};
  unsigned char out_buff[8192] = {0};

  size_t input_char_ptr = 0;
  size_t input_bit_ptr = 0;
  size_t in_buff_size = 0;

  size_t output_char_ptr = 0;
  size_t output_bit_ptr = 0;
  size_t out_buff_size = 8192;



  void read_buff();

  void write_buff();

public:
  FILE *ifp = nullptr;
  FILE *ofp = nullptr;
  bool is_EOF = false;

  BitStream() = default;
  BitStream(char *ifile, char *ofile);

  bool read_bit();

  void write_bit(bool bit);

  unsigned char read_byte();

  void write_byte(unsigned char byte);

  int get_file_size();

  void close();
};

class FrequencyTable {
private:
  static int cmp(const void * x1, const void * x2);
public:
  std::pair<long long, unsigned char> table[256];// {weight, symbol}
  long long agressive = 1000;
  FrequencyTable();
  long long text_size = 256;

  void update(unsigned char byte);

  int get_index(unsigned char byte);

  std::pair<long long, long long> get_prefsum(unsigned char byte);

  long long get_sum(int idx);

  unsigned char get_symbol(long long value, long long l, long long h);
};

class ArithmeticCompressor {
private:
  FrequencyTable frequency_table;

public:
  int bits_to_follow = 0;

  void bits_plus_follow(int bit);

  BitStream bitstream;
  long long value = 0;
  long long l0, h0;
  long long First_qtr, Half, Third_qtr;

  ArithmeticCompressor(char *ifile, char *ofile);

  void encode_byte(unsigned char byte);

  unsigned char decode_byte();
};

void compress_ari(char *ifile, char *ofile);

void decompress_ari(char *ifile, char *ofile);