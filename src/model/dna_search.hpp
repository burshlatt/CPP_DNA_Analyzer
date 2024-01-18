#ifndef A7_DNA_ANALYZER_1_1_MODEL_RK_ALGORITHM_HPP_
#define A7_DNA_ANALYZER_1_1_MODEL_RK_ALGORITHM_HPP_

#include <filesystem>
#include <fstream>
#include <list>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

namespace s21 {
class RabinKarp {
 public:
  RabinKarp() = default;
  ~RabinKarp() = default;

  void SetText(std::string_view text) {
    if (fs::exists(text))
      this->text_ = ReadFile(text);
    else
      this->text_ = text;
  }

  void SetPattern(std::string_view pattern) {
    if (fs::exists(pattern))
      this->pattern_ = ReadFile(pattern);
    else
      this->pattern_ = pattern;
  }

  std::list<int> GetPositions() const { return this->Search(); }

 private:
  std::string text_;
  std::string pattern_;

  bool CompareStrings(std::size_t pos) const noexcept {
    const std::size_t pattern_size = this->pattern_.size();
    for (std::size_t j = 0; j < pattern_size; ++j)
      if (this->text_[pos + j] != this->pattern_[j]) return false;
    return true;
  }

  std::string ReadFile(std::string_view path) const {
    std::string result;
    std::ifstream file(fs::path(path), std::ios::binary | std::ios::in);
    if (file.is_open()) {
      file.seekg(0, std::ios::end);
      int file_size = file.tellg();
      file.seekg(0, std::ios::beg);
      char *buffer = new char[file_size]{};
      file.read(buffer, file_size);
      for (int i = 0; i < file_size; i++) result += buffer[i];
      delete buffer;
    }
    file.close();
    return result;
  }

  std::list<int> Search() const {
    std::list<int> positions;

    if (pattern_.size() > text_.size()) return positions;

    const int mod = 9973;
    const int abc_size = 256;

    int first_symbol_hash = 1;
    int substring_hash = static_cast<int>(this->text_[0]) % mod;
    int pattern_hash = static_cast<int>(this->pattern_[0]) % mod;

    std::size_t pattern_size = this->pattern_.size();
    for (std::size_t i = 1; i < pattern_size; i++) {
      pattern_hash *= abc_size;
      pattern_hash += static_cast<int>(this->pattern_[i]);
      pattern_hash %= mod;

      substring_hash *= abc_size;
      substring_hash += static_cast<int>(this->text_[i]);
      substring_hash %= mod;

      first_symbol_hash *= abc_size;
      first_symbol_hash %= mod;
    }

    std::size_t text_size = this->text_.size();
    std::size_t size_offset = text_size - pattern_size;
    for (std::size_t pos = 0; pos <= size_offset; pos++) {
      if (pattern_hash == substring_hash && CompareStrings(pos))
        positions.push_back(pos);
      if (pos == size_offset) break;

      substring_hash -=
          (static_cast<int>(this->text_[pos]) * first_symbol_hash) % mod;
      substring_hash += mod;
      substring_hash *= abc_size;
      substring_hash += static_cast<int>(this->text_[pos + pattern_size]);
      substring_hash %= mod;
    }
    return positions;
  }
};
}  // namespace s21

#endif  // A7_DNA_ANALYZER_1_1_MODEL_RK_ALGORITHM_HPP_
