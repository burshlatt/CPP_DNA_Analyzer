#ifndef A7_DNA_ANALYZER_1_1_CONTROLLER_CONTROLLER_HPP_
#define A7_DNA_ANALYZER_1_1_CONTROLLER_CONTROLLER_HPP_

#include <list>
#include <string_view>

#include "../model/dna_search.hpp"
#include "../model/k_strings.hpp"
#include "../model/regex.hpp"
#include "../model/sequence_alignment.hpp"
#include "../model/window_substring.hpp"

namespace s21 {
class Controller {
 public:
  using Sequences = NeedlemanWunsch::Sequences;

  Controller() = default;
  ~Controller() = default;

  std::list<int> AlgorithmRK(std::string_view text, std::string_view pattern) {
    rk_.SetText(text);
    rk_.SetPattern(pattern);
    return rk_.GetPositions();
  }

  Sequences AlgorithmNW(std::string_view path) {
    nw_.ReadFile(path);
    return nw_.GetOptimalAlignment();
  }

  Sequences AlgorithmNW(int gap, int match, int mismatch,
                        std::string_view subseq_a, std::string_view subseq_b) {
    nw_.SetGapScore(gap);
    nw_.SetMatchScore(match);
    nw_.SetMismatchScore(mismatch);
    nw_.SetSeq(subseq_a, subseq_b);
    return nw_.GetOptimalAlignment();
  }

  bool RegularExpressions(std::string_view path) {
    rg_.ReadFile(path);
    return rg_.IsMatch();
  }

  bool RegularExpressions(std::string_view str, std::string_view expr) {
    rg_.SetString(str);
    rg_.SetExpression(expr);
    return rg_.IsMatch();
  }

  int KStrings(std::string_view path) {
    ks_.ReadFile(path);
    return ks_.GetDiffCount();
  }

  int KStrings(std::string_view str_a, std::string_view str_b) {
    ks_.SetStrings(str_a, str_b);
    return ks_.GetDiffCount();
  }

  std::string MinimumWindowSubstring(std::string_view path) {
    ws_.ReadFile(path);
    return ws_.GetMinimumWindowSubstring();
  }

  std::string MinimumWindowSubstring(std::string_view str,
                                     std::string_view pattern) {
    ws_.SetString(str);
    ws_.SetPattern(pattern);
    return ws_.GetMinimumWindowSubstring();
  }

 private:
  Regex rg_;
  KString ks_;
  RabinKarp rk_;
  NeedlemanWunsch nw_;
  WindowSubstring ws_;
};
}  // namespace s21

#endif  // A7_DNA_ANALYZER_1_1_CONTROLLER_CONTROLLER_HPP_
