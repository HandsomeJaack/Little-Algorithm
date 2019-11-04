#include <
std::pair<int, std::pair<int, int>> maxFactor = *std::max_element(
      factors.begin(), factors.end(),
      [](const auto &a, const auto &b) { return a.first < b.first; });
  // std::cout << maxFactor.first << "-";
  return maxFactor;
}

std::pair<Matrix, Matrix> sepMatrix(Matrix M,
                                    std::pair<int, int> removableEdge) {
  Matrix M1 = M, M2 = M;
  M1.erase(M1.begin() + removableEdge.first);
  for (auto &e : M1) {
    e.erase(e.begin() + removableEdge.second);
  }
  M2.at(removableEdge.first, removableEdge.second) = INF;
  return {M1, M2};
}


class Solution {
 public:
  void findSolution(Matrix M);
  void compareMatrix(Matrix M1, Matrix M2);
 private:
  std::pair<int, std::vector<int>> avliable_solution;
  int record;
};

void Solution::findSolution(Matrix M) {
  M.changeZeros();
  M.printMatrix();

  std::pair<int, std::vector<int>> solution;
  int prevFactor = INF;
  int lowestBorder = 0;
  
  Matrix M1, M2;
  while (M.size() != 2) {
    std::pair<int, std::pair<int, int>> maxFactor = M.detectRemovableEdge();
    if (prevFactor < maxFactor.first) {
      prevFactor = maxFactor.first;
      lowestBorder += maxFactor.first;
    }

    std::pair<Matrix, Matrix> M12 = sepMatrix(M, maxFactor.second);
    M1 = M12.first;
    M2 = M12.second;

    lowestBorder += M1.substractMatrix();
    M = M1;
  }

  solution = {lowestBorder, path};
  std::cout << "Shortest path: " << lowestBorder << std::endl << "Full path :";
  for (const auto &i : path) {
    std::cout << i << "->";
  }
  std::cout << std::endl;
}

int main() {
  Matrix M({{0, 4, 6, 8, 5, 7},
            {4, 0, 5, 7, 9, 3},
            {6, 5, 0, 7, 4, 5},
            {8, 7, 7, 0, 6, 4},
            {5, 9, 4, 6, 0, 9},
            {7, 3, 5, 4, 9, 0}});
  Solution S;
  S.findSolution(M);
  return 0;
}