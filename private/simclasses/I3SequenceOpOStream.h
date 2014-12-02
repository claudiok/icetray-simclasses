#include <ostream>

#define I3_SEQUENCE_OP_OSTREAM(Sequence, delim) \
    std::ostream& operator<<(std::ostream& os, const Sequence& seq) { \
    os << "["; \
    if (seq.begin() != seq.end()) { \
      Sequence::const_iterator iter = seq.begin(); \
      os << *iter; \
      ++iter; \
      for (; iter != seq.end(); ++iter) \
        os << delim << *iter; \
    } \
    os << "]"; \
    return os; \
  }
