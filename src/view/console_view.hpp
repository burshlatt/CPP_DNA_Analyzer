#ifndef A7_DNA_ANALYZER_1_1_VIEW_CONSOLE_VIEW_HPP_
#define A7_DNA_ANALYZER_1_1_VIEW_CONSOLE_VIEW_HPP_

#include <filesystem>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <string>

#include "../controller/controller.hpp"

#define BOLD "\033[1m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

namespace fs = std::filesystem;

namespace s21 {
class ConsoleView {
 public:
  ConsoleView() {
    menu_[0] = [this]() { this->DrawGoodbye(); };
    menu_[1] = [this]() { this->AlgorithmRK(); };
    menu_[2] = [this]() { this->AlgorithmNW(); };
    menu_[3] = [this]() { this->RegularExpressions(); };
    menu_[4] = [this]() { this->KStrings(); };
    menu_[5] = [this]() { this->MinimumWindowSubstring(); };
  }

  ~ConsoleView() = default;

  void RunApp() {
    int option = -1;
    while (option != 0) {
      this->ClearOutput();
      this->DrawLogo();
      this->DrawMenu();
      option = GetCorrectInt();
      if (menu_.find(option) != menu_.end()) {
        this->ClearOutput();
        menu_[option]();
      }
      if (option == 1 || option == 2 || option == 3 || option == 4 ||
          option == 5) {
        int back_to_menu = -1;
        while (back_to_menu != 1) {
          std::cout << RED << "1. " << BLUE << "Return to main menu" << RESET
                    << std::endl;
          std::cout << RED << "2. " << BLUE << "Repeat input" << RESET
                    << std::endl;
          std::cout << GREEN << "\nSelect menu item: " << RESET;
          back_to_menu = GetCorrectInt();
          if (back_to_menu == 2) {
            this->ClearOutput();
            menu_[option]();
          } else if (back_to_menu != 1) {
            std::cout << BOLD << RED
                      << "\nERROR: This item is not on the menu!\n"
                      << RESET << std::endl;
          }
        }
      }
    }
  }

 private:
  Controller controller_;
  std::map<int, std::function<void()>> menu_;

  inline void ClearOutput() const noexcept { std::cout << "\x1B[2J\x1B[H"; }

  inline void ClearInput() const {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  inline void DrawLogo() const noexcept {
    std::string dna_analyzer =
        "  _____    _   _                                           _          "
        "                             \n"
        " |  __ \\  | \\ | |     /\\           /\\                     | |     "
        "                             \n"
        " | |  | | |  \\| |    /  \\         /  \\     _ __     __ _  | |  _   "
        "_   ____   ___   _ __        \n"
        " | |  | | | . ` |   / /\\ \\       / /\\ \\   | '_ \\   / _` | | | | "
        "| | | |_  /  / _ \\ | '__|    \n"
        " | |__| | | |\\  |  / ____ \\     / ____ \\  | | | | | (_| | | | | "
        "|_| |  / /  |  __/ | |          \n"
        " |_____/  |_| \\_| /_/    \\_\\   /_/    \\_\\ |_| |_|  \\__,_| |_|  "
        "\\__, | /___|  \\___| |_|     \n"
        "                                                                __/ | "
        "                             \n"
        "                                                               |___/  "
        "                             \n";
    std::cout << BOLD << BLUE << dna_analyzer << RESET;
  }

  inline void DrawGoodbye() const noexcept {
    std::string goodbye =
        "   _____                       _   _                            \n"
        "  / ____|                     | | | |                           \n"
        " | |  __    ___     ___     __| | | |__    _   _    ___         \n"
        " | | |_ |  / _ \\   / _ \\   / _` | | '_ \\  | | | |  / _ \\    \n"
        " | |__| | | (_) | | (_) | | (_| | | |_) | | |_| | |  __/        \n"
        "  \\_____|  \\___/   \\___/   \\__,_| |_.__/   \\__, |  \\___|  \n"
        "                                            __/ |               \n"
        "                                           |___/                \n";

    std::cout << BOLD << BLUE << goodbye << RESET;
  }

  inline void DrawMenu() const noexcept {
    std::cout << RED << "1. " << BLUE << "Rabin-Karp algorithm" << RESET
              << std::endl;
    std::cout << RED << "2. " << BLUE << "Needleman-Wunsch algorithm" << RESET
              << std::endl;
    std::cout << RED << "3. " << BLUE << "Matching regular expressions" << RESET
              << std::endl;
    std::cout << RED << "4. " << BLUE
              << "Find the minimum number of changes for k-strings" << RESET
              << std::endl;
    std::cout << RED << "5. " << BLUE << "Find minimum window substring"
              << RESET << std::endl;
    std::cout << RED << "0. " << BLUE << "Exit" << RESET << std::endl;
    std::cout << GREEN << "\nSelect menu item: " << RESET;
  }

  int GetCorrectInt() const {
    bool is_correct = false;
    int result = 0;
    while (!is_correct) {
      try {
        std::string input_value;
        std::cin >> input_value;
        if (input_value == "b") return -1;
        result = std::stoi(input_value);
        is_correct = true;
      } catch (...) {
        std::cout << BOLD << RED << "\nERROR: Invalid input!" << RESET
                  << std::endl;
        std::cout << GREEN << "Try again: " << RESET;
        ClearInput();
      }
    }
    return result;
  }

  std::string GetFilePath() const {
    int opt = -1;
    fs::path path(fs::current_path());
    while (opt != 0) {
      this->ClearOutput();
      int num = 1;
      std::map<int, std::pair<bool, std::string>> dirs;
      std::cout << BOLD << BLUE << "DIRS / FILES:\n" << RESET << std::endl;

      for (const auto& entry : fs::directory_iterator(path.generic_string())) {
        if (entry.is_directory()) {
          std::cout << RED << num << "." << BOLD << BLUE << " (Dir)\t" << RESET;
          dirs[num] = {true, entry.path().filename().generic_string()};
        } else {
          std::cout << RED << num << "." << BOLD << GREEN << " (File)\t"
                    << RESET;
          dirs[num] = {false, entry.path().filename().generic_string()};
        }
        std::cout << entry.path().filename().generic_string() << std::endl;
        num++;
      }
      std::cout << BOLD << RED << "\nCURRENT_DIR: " << path.generic_string()
                << RESET << std::endl;
      std::cout << RED << "\nb." << BOLD << " BACK " << RESET << std::endl;
      std::cout << RED << "0." << BOLD << " EXIT " << RESET << std::endl;
      std::cout << GREEN << "\nSelect menu item: " << RESET;

      opt = GetCorrectInt();
      if (opt == -1) {
        path = path.parent_path();
      } else if (opt != 0 && dirs.find(opt) != dirs.end()) {
        auto [is_dir, name] = dirs[opt];
        path = path / name;
        if (!is_dir) return path.generic_string();
      }
    }
    return "";
  }

  void AlgorithmRK() {
    std::string text, pattern;
    int opt = -1;
    while (opt != 0 && opt != 1 && opt != 2) {
      std::cout << BOLD << BLUE << "TEXT:\n" << RESET << std::endl;
      std::cout << RED << "1. " << BLUE << "Enter text" << RESET << std::endl;
      std::cout << RED << "2. " << BLUE << "Select the path to the text file"
                << RESET << std::endl;
      std::cout << RED << "0. " << BLUE << "Cancel" << RESET << std::endl;
      std::cout << GREEN << "\nSelect menu item: " << RESET;
      opt = GetCorrectInt();
      if (opt == 1) {
        std::cout << GREEN << "\nEnter text: " << RESET;
        ClearInput();
        std::getline(std::cin, text);
      } else if (opt == 2) {
        text = GetFilePath();
      } else if (opt != 0) {
        std::cout << BOLD << RED << "\nERROR: This item is not on the menu!\n"
                  << RESET << std::endl;
      }
    }

    if (opt != 0 && !text.empty()) {
      opt = -1;
      this->ClearOutput();
      while (opt != 0 && opt != 1 && opt != 2) {
        std::cout << BOLD << BLUE << "PATTERN:\n" << RESET << std::endl;
        std::cout << RED << "1. " << BLUE << "Enter a search pattern" << RESET
                  << std::endl;
        std::cout << RED << "2. " << BLUE
                  << "Select the path to the pattern file" << RESET
                  << std::endl;
        std::cout << RED << "0. " << BLUE << "Cancel" << RESET << std::endl;
        std::cout << GREEN << "\nSelect menu item: " << RESET;
        opt = GetCorrectInt();
        if (opt == 1) {
          std::cout << GREEN << "\nEnter pattern: " << RESET;
          ClearInput();
          std::getline(std::cin, pattern);
        } else if (opt == 2) {
          pattern = GetFilePath();
        } else if (opt != 0) {
          std::cout << BOLD << RED << "\nERROR: This item is not on the menu!\n"
                    << RESET << std::endl;
        }
      }
      if (opt != 0 && !pattern.empty()) {
        this->ClearOutput();
        std::cout << RED << "Text: " << BLUE << text << std::endl;
        std::cout << RED << "Pattern: " << BLUE << pattern << std::endl;

        std::list<int> res = controller_.AlgorithmRK(text, pattern);
        std::cout << RED << "Positions: " << RESET;
        for (auto pos : res) std::cout << GREEN << pos << " ";
        std::cout << RESET << std::endl << std::endl;
      }
    }
  }

  void AlgorithmNW() {
    int gap = 0, match = 0, mismatch = 0;
    std::string seq_a, seq_b, path;
    int opt = -1;
    while (opt != 0 && opt != 1 && opt != 2) {
      std::cout << BOLD << BLUE << "MENU:\n" << RESET << std::endl;
      std::cout << RED << "1. " << BLUE << "Enter subsequences" << RESET
                << std::endl;
      std::cout << RED << "2. " << BLUE
                << "Select the path to the file with subsequences" << RESET
                << std::endl;
      std::cout << RED << "0. " << BLUE << "Cancel" << RESET << std::endl;
      std::cout << GREEN << "\nSelect menu item: " << RESET;
      opt = GetCorrectInt();
      if (opt == 1) {
        std::cout << GREEN << "\nEnter gap: " << RESET;
        gap = GetCorrectInt();
        std::cout << GREEN << "\nEnter match: " << RESET;
        match = GetCorrectInt();
        std::cout << GREEN << "\nEnter mismatch: " << RESET;
        mismatch = GetCorrectInt();
        std::cout << GREEN << "\nEnter subsequence #1: " << RESET;
        std::cin >> seq_a;
        std::cout << GREEN << "\nEnter subsequence #2: " << RESET;
        std::cin >> seq_b;
      } else if (opt == 2) {
        path = GetFilePath();
      } else if (opt != 0) {
        std::cout << BOLD << RED << "\nERROR: This item is not on the menu!\n"
                  << RESET << std::endl;
      }
    }
    if (opt != 0) {
      this->ClearOutput();
      Controller::Sequences res;
      if (opt == 1 && !seq_a.empty() && !seq_b.empty())
        res = controller_.AlgorithmNW(gap, match, mismatch, seq_a, seq_b);
      else if (opt == 2 && !path.empty())
        res = controller_.AlgorithmNW(path);

      auto [score, str_a, str_b] = res;
      std::cout << std::endl
                << RED << "Optimal score:\t" << GREEN << score << std::endl;
      std::cout << RED << "Optimal align:\t";
      std::cout << GREEN << str_a << BLUE << std::endl << "\t\t";
      for (std::size_t i = 0; i < str_a.size(); i++)
        std::cout << ((str_a[i] == str_b[i]) ? "|" : " ");
      std::cout << std::endl
                << GREEN << "\t\t" << str_b << RESET << std::endl
                << std::endl;
    }
  }

  void RegularExpressions() {
    std::string str, expr, path;
    int opt = -1;
    while (opt != 0 && opt != 1 && opt != 2) {
      std::cout << BOLD << BLUE << "REGULAR EXPRESSIONS:\n"
                << RESET << std::endl;
      std::cout << RED << "1. " << BLUE << "Enter string and expression"
                << RESET << std::endl;
      std::cout << RED << "2. " << BLUE
                << "Select the path to the file with the string and expression"
                << RESET << std::endl;
      std::cout << RED << "0. " << BLUE << "Cancel" << RESET << std::endl;
      std::cout << GREEN << "\nSelect menu item: " << RESET;
      opt = GetCorrectInt();
      if (opt == 1) {
        std::cout << GREEN << "\nString: " << RESET;
        std::cin >> str;
        std::cout << GREEN << "\nExpression: " << RESET;
        std::cin >> expr;
      } else if (opt == 2) {
        path = GetFilePath();
      } else if (opt != 0) {
        std::cout << BOLD << RED << "\nERROR: This item is not on the menu!\n"
                  << RESET << std::endl;
      }
    }
    if (opt != 0) {
      this->ClearOutput();
      bool is_match = false;
      if (opt == 1 && !str.empty() && !expr.empty())
        is_match = controller_.RegularExpressions(str, expr);
      else if (opt == 2 && !path.empty())
        is_match = controller_.RegularExpressions(path);

      std::cout << BOLD << BLUE << "\nRESULT:\n" << RESET << std::endl;
      if (is_match)
        std::cout << RED << "Match: " << GREEN << "True" << RESET << std::endl
                  << std::endl;
      else
        std::cout << RED << "Match: " << GREEN << "False" << RESET << std::endl
                  << std::endl;
    }
  }

  void KStrings() {
    std::string str_a, str_b, path;
    int opt = -1;
    while (opt != 0 && opt != 1 && opt != 2) {
      std::cout << BOLD << BLUE << "K-SIMILAR STRINGS:\n" << RESET << std::endl;
      std::cout << RED << "1. " << BLUE << "Enter k-strings" << RESET
                << std::endl;
      std::cout << RED << "2. " << BLUE
                << "Select the path to the file with k-strings" << RESET
                << std::endl;
      std::cout << RED << "0. " << BLUE << "Cancel" << RESET << std::endl;
      std::cout << GREEN << "\nSelect menu item: " << RESET;
      opt = GetCorrectInt();
      if (opt == 1) {
        std::cout << GREEN << "\nString #1: " << RESET;
        std::cin >> str_a;
        std::cout << GREEN << "\nString #2: " << RESET;
        std::cin >> str_b;
      } else if (opt == 2) {
        path = GetFilePath();
      } else if (opt != 0) {
        std::cout << BOLD << RED << "\nERROR: This item is not on the menu!\n"
                  << RESET << std::endl;
      }
    }
    if (opt != 0) {
      this->ClearOutput();
      int count = 0;
      if (opt == 1 && !str_a.empty() && !str_b.empty())
        count = controller_.KStrings(str_a, str_b);
      else if (opt == 2 && !path.empty())
        count = controller_.KStrings(path);
      std::cout << BOLD << BLUE << "RESULT:\n" << RESET << std::endl;
      std::cout << RED << "Number of changes: " << GREEN << count << RESET
                << std::endl
                << std::endl;
    }
  }

  void MinimumWindowSubstring() {
    std::string str, pattern, path;
    int opt = -1;
    while (opt != 0 && opt != 1 && opt != 2) {
      std::cout << BOLD << BLUE << "MINIMUM WINDOW SUBSTRING:\n"
                << RESET << std::endl;
      std::cout << RED << "1. " << BLUE << "Enter string and pattern" << RESET
                << std::endl;
      std::cout << RED << "2. " << BLUE
                << "Select the path to the file with the string and pattern"
                << RESET << std::endl;
      std::cout << RED << "0. " << BLUE << "Cancel" << RESET << std::endl;
      std::cout << GREEN << "\nSelect menu item: " << RESET;
      opt = GetCorrectInt();
      if (opt == 1) {
        std::cout << GREEN << "\nString: " << RESET;
        std::cin >> str;
        std::cout << GREEN << "\nPattern: " << RESET;
        std::cin >> pattern;
      } else if (opt == 2) {
        path = GetFilePath();
      } else if (opt != 0) {
        std::cout << BOLD << RED << "\nERROR: This item is not on the menu!\n"
                  << RESET << std::endl;
      }
    }
    if (opt != 0) {
      this->ClearOutput();
      std::string substr;
      if (opt == 1 && !str.empty() && !pattern.empty())
        substr = controller_.MinimumWindowSubstring(str, pattern);
      else if (opt == 2 && !path.empty())
        substr = controller_.MinimumWindowSubstring(path);
      std::cout << BOLD << BLUE << "RESULT:\n" << RESET << std::endl;
      std::cout << RED << "Minimum window substring: " << GREEN << substr
                << RESET << std::endl
                << std::endl;
    }
  }
};
}  // namespace s21

#endif  // A7_DNA_ANALYZER_1_1_VIEW_CONSOLE_VIEW_HPP_
