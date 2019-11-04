#include <algorithm>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>
#include "matrix.h"

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
  for (int i = 0; i < M[0].size(); ++i) {
    if (min >= M[i][columnNumber] && i != excludeNumber)
      min = M[i][columnNumber];
  }
  return min;
}

int Matrix::findMinInString(int stringNumber, int excludeNumber) {
  int min = INF;
  for (int i = 0; i < M[0].size(); ++i) {
    if (min >= M[stringNumber][i] && i != excludeNumber)
      min = M[stringNumber][i];
  }
  return min;
}

int Matrix::substractMatrix() {
  int lowestBorder = 0;
  for (int i = 0; i < M[0].size(); ++i) {
    Matrix M1 = M;
    int minString = M1.findMinInString(i);
    for (int j = 0; j < M[0].size(); ++j) {
      M[i][j] -= minString;
    }
    lowestBorder += minString;
  }

  for (int i = 0; i < M[0].size(); ++i) {
    Matrix M1 = M;
    int minColumn = M1.findMinInColunmn(i);
    for (int j = 0; j < M[0].size(); ++j) {
      M[j][i] -= minColumn;
    }
    lowestBorder += minColumn;
  }
  return lowestBorder;
}

std::pair<int, std::pair<int, int>> Matrix::detectRemovableEdge() {
  int maxColumn = 0, maxString = 0, maxElement = 0;
  std::vector<std::pair<int, std::pair<int, int>>> factors{};
  Matrix M1 = M;
  for (int i = 0; i < M[0].size(); ++i) {
    for (int j = 0; j < M[0].size(); ++j) {
      if (M[i][j] == 0) {
        int f = M1.findMinInString(i, j) + M1.findMinInColunmn(j, i);
        factors.push_back({f, {i, j}});
      }
    }
  }

auto Matrix::begin() { return M.begin(); }

auto Matrix::end() { return M.end(); }

void Matrix::erase(std::vector<std::vector<int>>::iterator iter) {
  M.erase(iter);
}

int Matrix::size() { return M.size(); }

int &Matrix::at(int i, int j) { return M[i][j]; }