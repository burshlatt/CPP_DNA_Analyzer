#ifndef A7_DNA_ANALYZER_1_1_MODEL_NW_ALGORITHM_HPP_
#define A7_DNA_ANALYZER_1_1_MODEL_NW_ALGORITHM_HPP_

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

namespace s21 {
class NeedlemanWunsch {
 public:
  using Matrix = std::vector<std::vector<int>>;

  struct Sequences {
    int optimal_score{};
    std::string alignment_a;
    std::string alignment_b;
  };

  NeedlemanWunsch() = default;
  ~NeedlemanWunsch() = default;

  void SetGapScore(int gap) noexcept { this->gap_ = gap; }
  void SetMatchScore(int match) noexcept { this->match_ = match; }
  void SetMismatchScore(int mismatch) noexcept { this->mismatch_ = mismatch; }

  void SetSeq(std::string_view seq_a, std::string_view seq_b) noexcept {
    this->seq_a_ = seq_a;
    this->seq_b_ = seq_b;
  }

  void ReadFile(std::string_view path) {
    std::ifstream file(fs::path(path), std::ios::in);
    if (file.is_open()) {
      file >> this->match_ >> this->mismatch_ >> this->gap_;
      file >> this->seq_a_ >> this->seq_b_;
    }
    file.close();
  }

  inline Sequences GetOptimalAlignment() const { return this->Align(); }

 private:
  int gap_{};
  int match_{};
  int mismatch_{};
  std::string seq_a_;
  std::string seq_b_;
  mutable Matrix matrix_;

  inline int GetScore(int i, int j) const noexcept {
    return this->seq_a_[i] == this->seq_b_[j] ? this->match_ : this->mismatch_;
  }

  Matrix CreateMatrix() const {
    std::size_t rows = this->seq_a_.size() + 1;
    std::size_t cols = this->seq_b_.size() + 1;
    Matrix new_matrix(rows, std::vector<int>(cols));

    for (std::size_t i = 0; i < rows; i++) new_matrix[i][0] = this->gap_ * i;

    for (std::size_t j = 0; j < cols; j++) new_matrix[0][j] = this->gap_ * j;

    for (std::size_t i = 1; i < rows; i++) {
      for (std::size_t j = 1; j < cols; j++) {
        int score_a = new_matrix[i][j - 1] + this->gap_;
        int score_b = new_matrix[i - 1][j] + this->gap_;
        int score_c = new_matrix[i - 1][j - 1] + GetScore(i - 1, j - 1);
        new_matrix[i][j] = std::max({score_a, score_b, score_c});
      }
    }
    return new_matrix;
  }

  Sequences Align() const {
    this->matrix_ = this->CreateMatrix();
    std::string alignment_a;
    std::string alignment_b;
    int i = this->seq_a_.size();
    int j = this->seq_b_.size();
    while (i > 0 && j > 0) {
      int score_up = this->matrix_[i][j - 1] + this->gap_;
      int score_left = this->matrix_[i - 1][j] + this->gap_;
      int score_diag = this->matrix_[i - 1][j - 1] + GetScore(i - 1, j - 1);

      std::vector<int> same_score;
      if (this->matrix_[i][j] == score_up)
        same_score.push_back(this->matrix_[i][j - 1]);
      if (this->matrix_[i][j] == score_left)
        same_score.push_back(this->matrix_[i - 1][j]);
      if (this->matrix_[i][j] == score_diag)
        same_score.push_back(this->matrix_[i - 1][j - 1]);
      auto choice = std::max_element(same_score.begin(), same_score.end());

      if (*choice == this->matrix_[i - 1][j - 1]) {
        alignment_a = this->seq_a_[i - 1] + alignment_a;
        alignment_b = this->seq_b_[j - 1] + alignment_b;
        i--;
        j--;
      } else if (*choice == this->matrix_[i - 1][j]) {
        alignment_a = this->seq_a_[i - 1] + alignment_a;
        alignment_b = "-" + alignment_b;
        i--;
      } else if (*choice == this->matrix_[i][j - 1]) {
        alignment_a = "-" + alignment_a;
        alignment_b = this->seq_b_[j - 1] + alignment_b;
        j--;
      }
    }
    return {matrix_.back().back(), alignment_a, alignment_b};
  }
};
}  // namespace s21

#endif  // A7_DNA_ANALYZER_1_1_MODEL_NW_ALGORITHM_HPP_