#include <algorithm>
#include <iostream>

#include "matrix.h"

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
};

void Solution::findSolution(Matrix M) {
  M.changeZeros();
  M.printMatrix();

  std::pair<int, std::pair<int,int>> removableEdge = M.detectRemovableEdge();
  std::pair<Matrix, Matrix> M12 = sepMatrix(M, removableEdge.second);
  std::vector<std::pair<int, int>> path;

  while(M12.first.size() != 2 || M12.second.size() != 2){
    std::pair<int, std::pair<int,int>> removableEdgeM1 = M12.first.detectRemovableEdge();
    std::pair<int, std::pair<int,int>> removableEdgeM2 = M12.second.detectRemovableEdge();

    if(removableEdgeM1.first < removableEdgeM2.first) {
      M12 = sepMatrix(M12.first, removableEdgeM1.second);
      path.push_back(removableEdgeM1.second);
    } else {
      M12 = sepMatrix(M12.second, removableEdgeM2.second);
      path.push_back(removableEdgeM2.second);
    }
  }

  for(const auto& i: path) {
    std::cout << i.first << "->" << i.second <<",";
  }

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