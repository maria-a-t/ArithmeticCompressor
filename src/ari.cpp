#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <utility>

class BitStream {
private:
  int buf_sz = 8192;
  unsigned char in_buff[8192] = {0};
  unsigned char out_buff[8192] = {0};

  size_t input_char_ptr = 0;
  size_t input_bit_ptr = 0;
  size_t in_buff_size = 0;

  size_t output_char_ptr = 0;
  size_t output_bit_ptr = 0;
  size_t out_buff_size = buf_sz;


  void read_buff() {
    if (!is_EOF && input_char_ptr >= in_buff_size) {
      in_buff_size = fread(in_buff, sizeof(char), buf_sz, ifp);

      if (in_buff_size == 0) {
        is_EOF = true;
      }
      input_char_ptr = input_bit_ptr = 0;
    }
  }

  void write_buff() {
    if (output_bit_ptr != 0) {
      output_char_ptr++;
    }
    size_t sz = fwrite(out_buff, sizeof(char), output_char_ptr, ofp);
    if (sz != output_char_ptr) {
      exit(1);
    }
    output_char_ptr = output_bit_ptr = 0;
  }

public:
  FILE *ifp = nullptr;
  FILE *ofp = nullptr;
  bool is_EOF = false;

  BitStream() = default;
  BitStream(char *ifile, char *ofile) {
    ifp = fopen(ifile, "rb");
    ofp = fopen(ofile, "wb");
  }

  bool read_bit() {
    read_buff();

    if (is_EOF) {
      return 0;
    }

    bool bit = in_buff[input_char_ptr] & (1 << (7 - input_bit_ptr));

    input_bit_ptr++;
    if (input_bit_ptr == 8) {
      input_bit_ptr = 0;
      input_char_ptr++;
    }

    return bit;
  }

  void write_bit(bool bit) {
    if (output_char_ptr >= out_buff_size) {
      write_buff();
    }
    out_buff[output_char_ptr] |= (bit << (7 - output_bit_ptr));

    if (output_bit_ptr == 7) {
      output_bit_ptr = 0;
      output_char_ptr++;
    } else {
      output_bit_ptr++;
    }
  }

  unsigned char read_byte() {
    read_buff();

    if (is_EOF) {
      return 0;
    }

    unsigned char byte = in_buff[input_char_ptr++];

    return byte;
  }

  void write_byte(unsigned char byte) {
    if (output_char_ptr >= out_buff_size) {
      write_buff();
    }

    out_buff[output_char_ptr++] = byte;
  }

  int get_file_size() {
    assert(!fseek(ifp, 0, SEEK_END));
    int sz = ftell(ifp) / sizeof(char);
    assert(!fseek(ifp, 0, SEEK_SET));
    return sz;
  }

  void close() {
    write_buff();
    fclose(ifp);
    fclose(ofp);
  }
};

class FrequencyTable {
private:
public:
  std::pair<unsigned long long, unsigned char> table[257];// {weight, symbol}
  unsigned long long prefsum[257];
  unsigned long long agressive = 500;

  FrequencyTable() {
    table[0] = {0, 0};

    for (int i = 1; i < 257; ++i) {
      table[i] = {1, i - 1};
    }

    upd_prefsum();
  }

  unsigned long long text_size = 256;

  void upd_prefsum() {
    prefsum[0] = 0;
    for (int i = 1; i < 257; ++i) {
      prefsum[i] = prefsum[i - 1] + table[i].first;
    }
  }

  void update(unsigned char byte) {
    int i = get_index(byte);

    text_size += agressive;
    table[i].first += agressive;

    while (text_size > (1ull << 24)) {
      text_size = 0;
      for (int j = 1; j < 257; ++j) {
        table[j].first /= 2;
        if (table[j].first == 0) {
          table[j].first = 1;
        }

        text_size += table[j].first;
      }
    }

    upd_prefsum();
  }

  int get_index(unsigned char byte) {
    return byte + 1;
  }

  std::pair<unsigned long long, unsigned long long> get_prefsum(unsigned char byte) {
    return {prefsum[byte], prefsum[byte + 1]};
  }

  unsigned long long get_sum(int idx) {
    return prefsum[idx + 1];
  }

  unsigned char get_symbol(unsigned long long value, unsigned long long l, unsigned long long h) {
    unsigned long long b = ((value - l + 1) * text_size - 1) / (h - l + 1);
    unsigned long long sum = 0;

    for (int j = 1; j < 257; ++j) {
      sum += table[j].first;
      if (sum > b) {
        return table[j].second;
      }
    }

    return 0;
  }
};

class ArithmeticCompressor {
private:
  FrequencyTable frequency_table;

public:
  int bits_to_follow = 0;

  void bits_plus_follow(int bit) {
    bitstream.write_bit(bit);
    for (; bits_to_follow > 0; bits_to_follow--) {
      bitstream.write_bit(!bit);
    }
  }

  BitStream bitstream;
  unsigned long long value = 0;
  unsigned long long l0, h0;
  unsigned long long First_qtr, Half, Third_qtr;

  ArithmeticCompressor(char *ifile, char *ofile) {
    this->bitstream = BitStream(ifile, ofile);
    this->frequency_table = FrequencyTable();

    l0 = 0, h0 = (1ull << 40) - 1;
    First_qtr = (h0 + 1) / 4;
    Half = First_qtr * 2;
    Third_qtr = First_qtr * 3;
  }


  void encode_byte(unsigned char byte) {

    auto [a, b] = frequency_table.get_prefsum(byte);

    unsigned long long l1 = l0 + a * (h0 - l0 + 1) / frequency_table.text_size;
    unsigned long long h1 = l0 + b * (h0 - l0 + 1) / frequency_table.text_size - 1;

    for (;;) {
      if (h1 < Half) {
        bits_plus_follow(0);
      } else if (l1 >= Half) {
        bits_plus_follow(1);
        l1 -= Half;
        h1 -= Half;
      } else if ((l1 >= First_qtr) && (h1 < Third_qtr)) {
        bits_to_follow++;
        l1 -= First_qtr;
        h1 -= First_qtr;
      } else {
        break;
      }
      l1 += l1;
      h1 += h1 + 1;
    }

    h0 = h1;
    l0 = l1;
    frequency_table.update(byte);
  }

  unsigned char decode_byte() {
    unsigned char byte = frequency_table.get_symbol(value, l0, h0);

    auto [a, b] = frequency_table.get_prefsum(byte);
    unsigned long long l = l0 + a * (h0 - l0 + 1) / frequency_table.text_size;
    unsigned long long h = l0 + b * (h0 - l0 + 1) / frequency_table.text_size - 1;
    for (;;) {
      if (h < Half) {
      } else if (l >= Half) {
        value -= Half;
        l -= Half;
        h -= Half;
      } else if ((l >= First_qtr) && (h < Third_qtr)) {
        value -= First_qtr;
        l -= First_qtr;
        h -= First_qtr;
      } else {
        break;
      }
      l = 2 * l;
      h = 2 * h + 1;
      value = 2 * value + bitstream.read_bit();
    }

    bitstream.write_byte(byte);
    frequency_table.update(byte);

    h0 = h;
    l0 = l;

    return byte;
  }
};

void compress_ari(char *ifile, char *ofile) {
  ArithmeticCompressor compressor(ifile, ofile);

  int sz = compressor.bitstream.get_file_size();
//  std::cout << sz << '\n';

  assert(fwrite(&sz, sizeof(int), 1, compressor.bitstream.ofp) == 1);

  unsigned char c;
  for (int i = 0; i < sz; ++i) {
    c = compressor.bitstream.read_byte();
    compressor.encode_byte(c);
  }
  compressor.bits_to_follow++;
  compressor.bits_plus_follow(compressor.l0 >= compressor.First_qtr);

  compressor.bitstream.close();
}


void decompress_ari(char *ifile, char *ofile) {
  ArithmeticCompressor compressor(ifile, ofile);
  int sz = 0;
  int res = fread(&sz, sizeof(sz), 1, compressor.bitstream.ifp);
  assert(res == 1);
  compressor.value = (compressor.bitstream.read_byte() << 8) + compressor.bitstream.read_byte();
  compressor.value <<= 8;
  compressor.value += compressor.bitstream.read_byte();
  compressor.value <<= 8;
  compressor.value += compressor.bitstream.read_byte();
  compressor.value <<= 8;
  compressor.value += compressor.bitstream.read_byte();

  for (int i = 0; i < sz; ++i) {
    compressor.decode_byte();
  }

  compressor.bitstream.close();
}
