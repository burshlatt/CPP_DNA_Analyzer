#ifndef A7_DNA_ANALYZER_1_1_MODEL_REGEX_HPP_
#define A7_DNA_ANALYZER_1_1_MODEL_REGEX_HPP_

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

namespace s21 {
class Regex {
 public:
  Regex() = default;
  ~Regex() = default;

  void SetString(std::string_view str) noexcept { this->str_ = str; }

  void SetExpression(std::string_view expr) noexcept { this->expr_ = expr; }

  void ReadFile(std::string_view path) {
    std::ifstream file(fs::path(path), std::ios::in);
    if (file.is_open()) file >> this->str_ >> this->expr_;
    file.close();
  }

  bool IsMatch() const {
    if (this->expr_.empty()) return this->str_.empty();

    int s_size = static_cast<int>(this->str_.size());
    int e_size = static_cast<int>(this->expr_.size());
    std::vector<std::vector<bool>> dp(s_size + 1,
                                      std::vector<bool>(e_size + 1));

    dp[s_size][e_size] = true;
    for (int i = s_size; i >= 0; i--) {
      for (int j = e_size - 1; j >= 0; j--) {
        bool first_match = (i < s_size && (this->expr_[j] == this->str_[i] ||
                                           this->expr_[j] == '.'));

        if (this->expr_[j] == '*') {
          dp[i][j] = dp[i][j + 1] || (i < s_size && dp[i + 1][j]);
        } else if (j + 1 < e_size && this->expr_[j + 1] == '+') {
          dp[i][j] = dp[i][j + 2] || (first_match && dp[i + 1][j]);
        } else if (this->expr_[j] == '?') {
          dp[i][j] = dp[i][j + 1] || (first_match && dp[i + 1][j + 1]);
        } else if (this->expr_[j] == '+') {
          dp[i][j] = false;
        } else {
          dp[i][j] = first_match && dp[i + 1][j + 1];
        }
      }
    }
    return dp[0][0];
  }

 private:
  std::string str_;
  std::string expr_;
};
}  // namespace s21

#endif  // A7_DNA_ANALYZER_1_1_MODEL_REGEX_HPP_