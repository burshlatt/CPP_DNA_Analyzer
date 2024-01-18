#ifndef A7_DNA_ANALYZER_1_1_MODEL_K_STRINGS_HPP_
#define A7_DNA_ANALYZER_1_1_MODEL_K_STRINGS_HPP_

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

namespace s21 {
class KString {
 public:
  KString() = default;
  ~KString() = default;

  void SetStrings(std::string_view str_a, std::string_view str_b) noexcept {
    this->str_a_ = str_a;
    this->str_b_ = str_b;
  }

  void ReadFile(std::string_view path) {
    std::ifstream file(fs::path(path), std::ios::in);
    if (file.is_open()) file >> this->str_a_ >> this->str_b_;
    file.close();
  }

  int GetDiffCount() const {
    if (!this->IsAnagrams()) return -1;

    return KSimilarity(this->str_a_, this->str_b_);
  }

 private:
  std::string str_a_;
  std::string str_b_;

  bool IsAnagrams() const {
    if (this->str_a_.size() != this->str_b_.size()) return false;

    std::vector<int> letters_a(128);
    std::vector<int> letters_b(128);
    for (std::size_t i = 0; i < this->str_a_.size(); i++) {
      letters_a[this->str_a_[i]] += 1;
      letters_b[this->str_b_[i]] += 1;
    }

    if (letters_a != letters_b) return false;

    return true;
  }

  int KSimilarity(std::string_view str_a, std::string_view str_b) const {
    std::string str_a_copy(str_a);
    std::string str_b_copy(str_b);
    for (std::size_t i = 0; i < str_a_copy.size(); i++) {
      if (str_a_copy[i] == str_b_copy[i]) continue;

      std::vector<int> matches;
      for (std::size_t j = i + 1; j < str_a_copy.size(); j++) {
        if (str_a_copy[j] == str_b_copy[i] && str_a_copy[j] != str_b_copy[j]) {
          matches.push_back(j);
          if (str_a_copy[i] == str_b_copy[j]) {
            std::swap(str_a_copy[i], str_a_copy[j]);
            return 1 + KSimilarity(str_a_copy.substr(i + 1),
                                   str_b_copy.substr(i + 1));
          }
        }
      }

      int best = str_a_copy.size() - 1;
      for (int j : matches) {
        std::swap(str_a_copy[i], str_a_copy[j]);
        best = std::min(best, 1 + KSimilarity(str_a_copy.substr(i + 1),
                                              str_b_copy.substr(i + 1)));
        std::swap(str_a_copy[i], str_a_copy[j]);
      }
      return best;
    }
    return 0;
  }
};
}  // namespace s21

#endif  // A7_DNA_ANALYZER_1_1_MODEL_K_STRINGS_HPP_
