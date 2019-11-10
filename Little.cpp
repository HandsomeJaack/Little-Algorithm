#include <algorithm>
#include <iostream>
#include <memory>
#include <functional>

#include "matrix.h"

std::vector<int> rows;
std::vector<int> columns;
std::vector<std::pair<int, int>> path;

struct Node {
  Matrix M;
  std::unique_ptr<Node> left, right;
  int bypass;
  
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

void forEachTreeElem(Node& node, std::function<void(Node &)> func) {
    if(node.left)   forEachTreeElem(*node.left, func);
    if(node.right)   forEachTreeElem(*node.right, func);

    func(node);
}

Node& findMin(Node& root) { 
  int currBottom = std::numeric_limits<int>::max();
  Node *minNode = 0;
  forEachTreeElem(root, [&](Node &node)
    {
        if (node.left) // Есть дочерние узлы, пропускаем этот узел.
            return;
        int val = node.M.findBottom();
        if (val < currBottom)
        {
            currBottom = val;
            minNode = &node;
            node.bypass = val;
        }
    });
    return *minNode;
}

void bypass(Node& root) {

  std::pair<int, std::pair<int, int>> removableEdge;
  std::pair<Matrix, Matrix> M12;
  Node* newNode = &root;
 
  while(newNode->M.size() != 2) {
    newNode = &findMin(root);
    newNode->M.substractMatrix();
    
    removableEdge = newNode->M.detectRemovableEdge();
    M12 = sepMatrix(newNode->M, removableEdge.second);

    newNode->left = std::make_unique<Node>(M12.first);
    newNode->right = std::make_unique<Node>(M12.second);
    
    int realRow = rows.at(removableEdge.second.first);
    int realColumn = columns.at(removableEdge.second.second);
    
    path.push_back(std::make_pair(realRow + 1, realColumn + 1));
    
    rows.erase(std::find(rows.begin(), rows.end(), realRow));
    columns.erase(
        std::find(columns.begin(), columns.end(), realColumn));
    
  }
}

void findSolution(Matrix& M) {
  M.changeZeros();
  Node Tree(M);

  for (int i = 0; i < M.size(); ++i) {
    rows.push_back(i);
    columns.push_back(i);
  }

  bypass(Tree);

  for (int i = 1; i < path.size() + 1; ++i) {
    if(i == path.size()) {
      std::cout << "(" << path[i-1].first << "," << path[i-1].second << ")" << std::endl;
      break;
    }
    std::cout << "(" << path[i-1].first << "," << path[i-1].second <<"), ("
              << path[i-1].second << "," << path[i].first << "), ";
  }
  std::cout << Tree.bypass << std::endl;
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