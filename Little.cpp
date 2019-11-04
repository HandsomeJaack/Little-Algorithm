#include <algorithm>
#include <iostream>
#include <memory>

#include "matrix.h"

struct Node {
  Matrix M;
  std::unique_ptr<Node> left, right;
  std::vector<std::pair<int, int>> path;
  int bypass;
  std::vector<int> rows;
  std::vector<int> columns;
  Node(Matrix M) : M(M) {}
};

std::pair<Matrix, Matrix> sepMatrix(Matrix M,
                                    std::pair<int, int> removableEdge) {
  Matrix M1 = M, M2 = M;
  M1.erase(M1.begin() + removableEdge.first);
  for (auto& e : M1) {
    e.erase(e.begin() + removableEdge.second);
  }
  M2.at(removableEdge.first, removableEdge.second) = INF;
  return {M1, M2};
}

void bypass(std::pair<Matrix, Matrix> M12, Node& node) {
  if (M12.first.size() == 2 || M12.second.size() == 2) return;

  int bottomM1 = M12.first.substractMatrix();
  int bottomM2 = M12.second.substractMatrix();

  std::cout << bottomM1 << ";" << bottomM2 << std::endl;
  node.left = std::make_unique<Node>(M12.first);
  node.right = std::make_unique<Node>(M12.second);

  if (bottomM1 < bottomM2) {
    node.bypass += bottomM1;
    std::pair<int, std::pair<int, int>> removableEdgeM1 =
        M12.first.detectRemovableEdge();
    M12 = sepMatrix(M12.first, removableEdgeM1.second);

    int realRow = node.rows.at(removableEdgeM1.second.first);
    int realColumn = node.columns.at(removableEdgeM1.second.second);

    node.path.push_back(std::make_pair(realRow + 1, realColumn + 1));

    node.rows.erase(std::find(node.rows.begin(), node.rows.end(), realRow));
    node.columns.erase(
        std::find(node.columns.begin(), node.columns.end(), realColumn));

    bypass(M12, *node.left);
  } else {
    node.bypass += bottomM2;
    std::pair<int, std::pair<int, int>> removableEdgeM2 =
        M12.second.detectRemovableEdge();
    M12 = sepMatrix(M12.second, removableEdgeM2.second);

    int realRow = node.rows.at(removableEdgeM2.second.first);
    int realColumn = node.columns.at(removableEdgeM2.second.second);

    node.path.push_back(std::make_pair(realRow + 1, realColumn + 1));

    node.rows.erase(std::find(node.rows.begin(), node.rows.end(), realRow));
    node.columns.erase(
        std::find(node.columns.begin(), node.columns.end(), realRow));

    bypass(M12, *node.right);
  }
}

void findSolution(Matrix& M) {
  Node Tree(M);
  M.changeZeros();
  Tree.bypass = M.substractMatrix();

  for (int i = 0; i < M.size(); ++i) {
    Tree.rows.push_back(i);
    Tree.columns.push_back(i);
  }

  std::pair<int, std::pair<int, int>> removableEdge = M.detectRemovableEdge();
  Tree.path.push_back(removableEdge.second);
  std::pair<Matrix, Matrix> M12 = sepMatrix(M, removableEdge.second);

  Tree.rows.erase(std::find(Tree.rows.begin(), Tree.rows.end(),
                            removableEdge.second.first));
  Tree.columns.erase(std::find(Tree.columns.begin(), Tree.columns.end(),
                               removableEdge.second.second));
  bypass(M12, Tree);

  for (const auto& i : Tree.path) {
    if (&i != &Tree.path.back()) {
      std::cout << i.first << "->" << i.second << ",";
    } else {
      std::cout << i.first << "->" << i.second << std::endl;
    }
  }
}

int main() {
  Matrix M({{0, 4, 6, 8, 5, 7},
            {4, 0, 5, 7, 9, 3},
            {6, 5, 0, 7, 4, 5},
            {8, 7, 7, 0, 6, 4},
            {5, 9, 4, 6, 0, 9},
            {7, 3, 5, 4, 9, 0}});
  findSolution(M);
  return 0;
}