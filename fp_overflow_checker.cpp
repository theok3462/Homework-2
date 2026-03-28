// Theo Kliewer
// CS3339 Spring 2026
// HW2 - floating point overflow checker

#include <bitset>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

// class to hold a float and get its bits
class FPBits {
 public:
  FPBits(float val) {
    num = val;
    uint32_t tmp = 0;
    std::memcpy(&tmp, &num, sizeof(tmp));
    bits = std::bitset<32>(tmp);
  }

  uint32_t getRaw() {
    return (uint32_t)bits.to_ulong();
  }

  // get the biased exponent (bits 30-23)
  int getExp() {
    return (int)((getRaw() >> 23) & 0xFF);
  }

  // subtract bias to get real exponent
  int getTrueExp() {
    return getExp() - 127;
  }

  // print in format: s eeeeeeee mmmmmmmmmmmmmmmmmmmmmmm
  void printBits(std::ostream& out) {
    uint32_t r = getRaw();

    // sign
    out << ((r >> 31) & 1) << " ";

    // exponent
    for (int i = 30; i >= 23; i--)
      out << ((r >> i) & 1);
    out << " ";

    // mantissa
    for (int i = 22; i >= 0; i--)
      out << ((r >> i) & 1);
  }

  float num;

 private:
  std::bitset<32> bits;
};

// checks if overflow will happen and finds the threshold
// the idea is: a float has 23 mantissa bits, so adding counter to bound
// gets lost when the exponent of bound is big enough that counter < ulp
// that happens when bound's exponent >= counter's exponent + 24
bool checkOverflow(float bound, float counter, float* thresh) {
  FPBits fc(counter);
  int cexp = fc.getTrueExp();

  // smallest exponent where counter gets lost
  int minExp = cexp + 24;
  int biased = minExp + 127;

  // make sure it fits in a normal float
  if (biased <= 0 || biased >= 255)
    return false;

  // build threshold float from its exponent bits, mantissa stays 0
  uint32_t raw = (uint32_t)biased << 23;
  float t = 0.0f;
  std::memcpy(&t, &raw, sizeof(t));

  *thresh = t;

  return t <= bound;
}

void printUsage(const char* name) {
  std::cerr << "usage:\n";
  std::cerr << "        " << name << " loop_bound loop_counter\n\n";
  std::cerr << "        loop_bound is a positive floating-point value\n";
  std::cerr << "        loop_counter is a positive floating-point value\n";
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printUsage(argv[0]);
    return 1;
  }

  float bound = std::stof(std::string(argv[1]));
  float counter = std::stof(std::string(argv[2]));

  FPBits fb(bound);
  FPBits fc(counter);

  std::cout << "Loop bound:   ";
  fb.printBits(std::cout);
  std::cout << "\n";

  std::cout << "Loop counter: ";
  fc.printBits(std::cout);
  std::cout << "\n\n";

  float thresh = 0.0f;
  if (checkOverflow(bound, counter, &thresh)) {
    FPBits ft(thresh);
    std::cout << "Warning: Possible overflow!\n";
    std::cout << "Overflow threshold:\n";
    std::cout << "        " << thresh << "\n";
    std::cout << "        ";
    ft.printBits(std::cout);
    std::cout << "\n";
  } else {
    std::cout << "There is no overflow!\n";
  }

  return 0;
}
