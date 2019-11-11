#include <algorithm>
#include <iostream>
#include <memory>
#include <functional>

#include "matrix.h"


struct Node {
  Matrix M;
  std::vector<int> rows;
  std::vector<int> columns;
  std::unique_ptr<Node> left, right;
  std::vector<std::pair<int, int>> path;
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

Node& findMin(Node& root, int currBottom) { 
  Node *minNode = 0;
  forEachTreeElem(root, [&](Node &node)
    {
        if (node.left) // Есть дочерние узлы, пропускаем этот узел.
            return;
        int val = node.M.findBottom();
        //std::cout << val << "-";
        if (val < currBottom)
        {
            currBottom = val;
            minNode = &node;
            node.bypass += val;
        }
    });
    return *minNode;
}

void bypass(Node& root) {

  std::pair<int, std::pair<int, int>> removableEdge;
  std::pair<Matrix, Matrix> M12;
  Node* newNode = &root;
  int curBottom = newNode->M.substractMatrix();
  newNode->bypass = curBottom;
  //std::cout << curBottom <<"-";
  //newNode->bypass += removableEdge.first;
  
  while(newNode->M.size() > 2) {
    //std::cout << "while";
    newNode->M.printMatrix();
    for(const auto& i: newNode->rows) 
      std::cout << i << "-";
    std::cout << std::endl;

    for(const auto& i: newNode->columns) 
      std::cout << i << "-";
    std::cout << std::endl;
    
    for(const auto& i: newNode->path) 
      std::cout << i.first << ":" << i.second <<"-";
    std::cout << std::endl;
    removableEdge = newNode-> M.detectRemovableEdge();
    M12 = sepMatrix(newNode->M, removableEdge.second);
    newNode->bypass += removableEdge.first;

    newNode->left = std::make_unique<Node>(M12.first);
    newNode->right = std::make_unique<Node>(M12.second);
        
    int realRow = newNode->rows.at(removableEdge.second.first);
    int realColumn = newNode->columns.at(removableEdge.second.second);
    
    newNode->right->path = newNode->path;
    newNode->left->path = newNode->path;

    newNode->right->path.push_back(std::make_pair(realRow + 1, realColumn + 1));
    newNode->left->path.push_back(std::make_pair(realRow + 1, realColumn + 1));
    
    newNode->left->rows = newNode->rows;
    newNode->left->columns = newNode->columns;
    
    newNode->right->rows = newNode->rows;
    newNode->right->columns = newNode->columns;

    newNode->left->rows.erase(std::find(newNode->left->rows.begin(), newNode->left->rows.end(), realRow));
    newNode->left->columns.erase(
    std::find(newNode->left->columns.begin(), newNode->left->columns.end(), realColumn));

    newNode = &findMin(root, curBottom);
    newNode->M.substractMatrix();
  }
}

void findSolution(Matrix& M) {
  M.changeZeros();
  Node Tree(M);

  for (int i = 0; i < M.size(); ++i) {
    Tree.rows.push_back(i);
    Tree.columns.push_back(i);
  }

  bypass(Tree);

  for (int i = 0; i < Tree.path.size(); ++i) {
    if( i == Tree.path.size()-1) {
      std::cout << "(" << Tree.path[i].first << "," << Tree.path[0].second << "),";
      std::cout << "(" << Tree.path[i].second << "," << Tree.path[0].first << ")";
    } else {
      std::cout << "(" << Tree.path[i].first << "," << Tree.path[i].second << "),";
      std::cout << "(" << Tree.path[i].second << "," << Tree.path[i+1].first << "),";
    }
  }
  std::cout << std::endl;
  //std::cout << Tree.bypass << std::endl;
}

int main() {
  Matrix M({{0, 4, 6, 8, 5, 7},
            {4, 0, 5, 7, 9, 3},
            {6, 5, 0, 7, 4, 5},
            {8, 7, 7, 0, 6, 4},
            {5, 9, 4, 6, 0, 9},
            {7, 3, 5, 4, 9, 0}});
  Matrix S({{0, 5, 1, 4},
            {5, 0, 5, 3},
            {5, 3, 0, 6},
            {8, 6, 7, 0}});
  //findSolution(S);
  findSolution(M);
  return 0;
}