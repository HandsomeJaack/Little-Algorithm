#include "matrix.h"
#include <algorithm>
#include <iostream>

void Matrix::printMatrix() {
  for (const auto &v : M) {
    for (const auto &e : v) {
      if (e >= INF / 2) {
        std::cout << "inf ";
      } else {
        std::cout << e << " ";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void Matrix::changeZeros() {
  for (auto &v : M) {
    for (auto &e : v) {
      if (e == 0) e = INF;
    }
  }
}

int Matrix::findMinInColunmn(int columnNumber, int excludeNumber) {
  int min = INF;
  for (size_t i = 0; i < M[0].size(); ++i) {
    if (min >= M[i][columnNumber] && i != excludeNumber)
      min = M[i][columnNumber];
  }
  return min;
}

int Matrix::findMinInString(int stringNumber, int excludeNumber) {
  int min = INF;
  for (size_t i = 0; i < M[0].size(); ++i) {
    if (min >= M[stringNumber][i] && i != excludeNumber)
      min = M[stringNumber][i];
  }
  return min;
}

int Matrix::substractMatrix() {
  int lowestBorder = 0;
  for (size_t i = 0; i < M[0].size(); ++i) {
    Matrix M1 = M;
    int minString = M1.findMinInString(i);
    for (size_t j = 0; j < M[0].size(); ++j) {
      M[i][j] -= minString;
    }
    lowestBorder += minString;
  }

  for (size_t i = 0; i < M[0].size(); ++i) {
    Matrix M1 = M;
    int minColumn = M1.findMinInColunmn(i);
    for (size_t j = 0; j < M[0].size(); ++j) {
      M[j][i] -= minColumn;
    }
    lowestBorder += minColumn;
  }
  return lowestBorder;
}

std::pair<int, std::pair<int, int>> Matrix::detectRemovableEdge() {
  std::vector<std::pair<int, std::pair<int, int>>> factors{};

  Matrix M1 = M;
  for (size_t i = 0; i < M1.size(); ++i) {
    for (size_t j = 0; j < M1.size(); ++j) {
      if (M1.at(i, j) == 0) {
        int f = M1.findMinInString(i, j) + M1.findMinInColunmn(j, i);
        factors.push_back({f, {i, j}});
      }
    }
  }

  std::pair<int, std::pair<int, int>> maxFactor{};

  if (!factors.empty()) {
    maxFactor = *std::max_element(
        factors.begin(), factors.end(),
        [](const auto &a, const auto &b) { return a.first < b.first; });
  } else {
    std::cout << "empty vector.";
  }
  // std::cout << maxFactor.first << "-";
  return maxFactor;
}

std::vector<std::vector<int>>::iterator Matrix::begin() { return M.begin(); }

std::vector<std::vector<int>>::iterator Matrix::end() { return M.end(); }

void Matrix::erase(std::vector<std::vector<int>>::iterator iter) {
  M.erase(iter);
}

int Matrix::size() { return M.size(); }

int &Matrix::at(int i, int j) { return M[i][j]; }