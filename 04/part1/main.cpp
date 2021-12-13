#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace fs = std::filesystem;

auto read_extracted_numbers(std::ifstream &fstream)
{
  std::string line;
  std::getline(fstream, line);
  std::vector<int> extracted_numbers;
  auto start = 0;
  auto end = line.find(",");
  while (end != std::string::npos) {
    extracted_numbers.push_back(stoi(line.substr(start, end)));
    start = end + 1;
    end = line.find(",", start);
  }
  extracted_numbers.push_back(stoi(line.substr(start, line.size() - 1)));
  return extracted_numbers;
}

struct BingoResult
{
  int turns;
  int score;
};

class Board
{
 public:
  Board(std::ifstream &fstream)
  {
    for (int i = 0; i < 5; i++) {
      std::vector<int> row;
      std::vector<bool> m;
      std::string line;
      std::getline(fstream, line);
      auto start = find_first_digit(line, 0);
      auto end = line.find(" ", start);
      while (end != std::string::npos) {
        row.push_back(stoi(line.substr(start, end)));
        m.push_back(false);
        start = find_first_digit(line, end + 1);
        end = line.find(" ", start);
      }
      row.push_back(stoi(line.substr(start, line.size() - 1)));
      m.push_back(false);
      rows.push_back(row);
      marked.push_back(m);
    }
  }

  struct BingoResult play_bingo(const std::vector<int> &numbers)
  {
    for (int turn = 0; turn < numbers.size(); turn++) {
      last_number = numbers[turn];
      for (int i = 0; i < rows.size(); i++) {
        for (int j = 0; j < rows[i].size(); j++) {
          if (rows[i][j] == numbers[turn]) {
            marked[i][j] = true;
            if (bingo()) {
              return {turn, compute_score()};
            }
          }
        }
      }
    }
    return {std::numeric_limits<int>::max(), 0};
  }

  void print_board()
  {
    std::cout << "-------------------\n";
    for (auto row : rows) {
      for (auto v : row) {
        std::cout << v << ' ';
      }
      std::cout << '\n';
    }
    std::cout << "-------------------\n";
  }
  void print_marked()
  {
    std::cout << "-------------------\n";
    for (auto row : marked) {
      for (auto v : row) {
        std::cout << v << ' ';
      }
      std::cout << '\n';
    }
    std::cout << "-------------------\n";
  }

 private:
  std::vector<std::vector<int>> rows;
  std::vector<std::vector<bool>> marked;
  int last_number;

  std::size_t find_first_digit(const std::string &s, std::size_t pos = 0)
  {
    for (auto i = pos; i < s.size(); i++) {
      if (isdigit(s[i])) {
        return i;
      }
    }
    return std::string::npos;
  }

  bool bingo()
  {
    // Check rows
    for (auto row : marked) {
      bool all_marked = true;
      for (auto v : row) {
        if (!v) {
          all_marked = false;
          break;
        }
      }
      if (all_marked) {
        return true;
      }
    }

    // Check columns
    for (int j = 0; j < marked[0].size(); j++) {
      bool all_marked = true;
      for (int i = 0; i < marked.size(); i++) {
        if (!marked[i][j]) {
          all_marked = false;
          break;
        }
      }
      if (all_marked) {
        return true;
      }
    }

    return false;
  }

  int compute_score()
  {
    int score = 0;
    for (int i = 0; i < marked.size(); i++) {
      for (int j = 0; j < marked[i].size(); j++) {
        if (!marked[i][j]) {
          score += rows[i][j];
        }
      }
    }
    return score * last_number;
  }
};

int main(int argc, char **argv)
{
  std::ifstream input_file(argv[1]);
  auto extracted_numbers = read_extracted_numbers(input_file);

  struct BingoResult best_rst
  {
    std::numeric_limits<int>::max(),  // turns
        0,                            // score
  };
  for (std::string skip_line; std::getline(input_file, skip_line);) {
    Board board{input_file};
    auto rst = board.play_bingo(extracted_numbers);
    if (rst.turns < best_rst.turns) {
      best_rst = rst;
    }
  }
  std::cout << best_rst.score << '\n';
}
