#ifndef A7_DNA_ANALYZER_1_1_MODEL_WINDOW_SUBSTRING_HPP_
#define A7_DNA_ANALYZER_1_1_MODEL_WINDOW_SUBSTRING_HPP_

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

namespace s21 {
class WindowSubstring {
 public:
  WindowSubstring() = default;
  ~WindowSubstring() = default;

  void SetString(std::string_view str) noexcept { this->str_ = str; }

  void SetPattern(std::string_view pattern) noexcept {
    this->pattern_ = pattern;
  }

  void ReadFile(std::string_view path) {
    std::ifstream file(fs::path(path), std::ios::in);
    if (file.is_open()) file >> this->str_ >> this->pattern_;
    file.close();
  }

  std::string GetMinimumWindowSubstring() const {
    if (this->pattern_.size() > this->str_.size()) return "";

    std::vector<int> let_count(128, 0);
    for (auto ch : this->pattern_) let_count[ch]++;

    int min_len = __INT_MAX__;
    int begin = 0, end = 0, head = 0;
    int counter = this->pattern_.size();

    while (static_cast<std::size_t>(end) < this->str_.size()) {
      if (let_count[this->str_[end++]]-- > 0) counter--;

      while (counter == 0) {
        if (end - begin < min_len) {
          head = begin;
          min_len = end - begin;
        }

        if (let_count[this->str_[begin++]]++ == 0) counter++;
      }
    }
    return min_len == __INT_MAX__
               ? ""
               : std::string(this->str_.substr(head, min_len));
  }

 private:
  std::string str_;
  std::string pattern_;
};
}  // namespace s21

#endif  // A7_DNA_ANALYZER_1_1_MODEL_WINDOW_SUBSTRING_HPP_