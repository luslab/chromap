#ifndef UTILS_H_
#define UTILS_H_

#include <sys/resource.h>
#include <sys/time.h>

namespace chromap {

struct uint128_t {
  uint64_t first;
  uint64_t second;
};

struct BarcodeWithQual {
  uint32_t corrected_base_index1;
  char correct_base1;
  uint32_t corrected_base_index2;
  char correct_base2;
  double score;
  bool operator>(const BarcodeWithQual &b) const {
    return std::tie(score, corrected_base_index1, correct_base1,
                    corrected_base_index2, correct_base2) >
           std::tie(b.score, b.corrected_base_index1, b.correct_base1,
                    b.corrected_base_index2, b.correct_base2);
  }
};

#define SortMappingWithoutBarcode(m)                                      \
  (((((m).fragment_start_position_ << 16) | (m).fragment_length_) << 8) | \
   (m).mapq_)
//#define SortMappingWithoutBarcode(m) (m)

KHASH_MAP_INIT_INT64(k128, uint128_t);
KHASH_MAP_INIT_INT64(k64_seq, uint64_t);
KHASH_SET_INIT_INT(k32_set);
KHASH_MAP_INIT_INT64(kmatrix, uint32_t);

struct StackCell {
  size_t x;  // node
  int k, w;  // k: level; w: 0 if left child hasn't been processed
  StackCell(){};
  StackCell(int k_, size_t x_, int w_) : x(x_), k(k_), w(w_){};
};

inline static double GetRealTime() {
  struct timeval tp;
  struct timezone tzp;
  gettimeofday(&tp, &tzp);
  return tp.tv_sec + tp.tv_usec * 1e-6;
}

inline static double GetCPUTime() {
  struct rusage r;
  getrusage(RUSAGE_SELF, &r);
  return r.ru_utime.tv_sec + r.ru_stime.tv_sec +
         1e-6 * (r.ru_utime.tv_usec + r.ru_stime.tv_usec);
}

inline static void ExitWithMessage(const std::string &message) {
  std::cerr << message << std::endl;
  exit(-1);
}

}  // namespace chromap

#endif  // UTILS_H_
