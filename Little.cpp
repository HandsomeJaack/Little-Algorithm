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
  Node(){}
};

std::pair<Node, Node> sepMatrix(Node* N,
                                    std::pair<int, int> removableEdge) {
  Matrix M1 = N->M, M2 = N->M;
  Node N1(M1), N2(M2);
  N1.rows = N->rows; N1.columns = N->columns;
  N2.rows = N->rows; N2.columns = N->columns;
  N1.path = N->path; N2.path = N->path;

  int realRow = N1.rows.at(removableEdge.first);
  int realColumn = N1.columns.at(removableEdge.second);
  std::cout << "Matrix row/column: " << removableEdge.first << "|" << removableEdge.second << std::endl;
  std::cout << "Real edge: " << realRow << "|" << realColumn << std::endl;


  if(std::find(N1.rows.begin(), N1.rows.end(), realColumn) != N1.rows.end()
    && std::find(N1.columns.begin(), N1.columns.end(), realRow) != N1.columns.end())
    N1.M.at(removableEdge.second, removableEdge.first) = INF;

  auto rowPos = std::find(N1.rows.begin(), N1.rows.end(), realColumn);
  std::cout << N1.path.size() << std::endl;
  for(const auto& p: N1.path) {
    auto columnPos = std::find(N1.columns.begin(), N1.columns.end(), p.first);
    if(rowPos != N1.rows.end() && columnPos != N1.columns.end()) {
      int matrixPos = std::distance(N1.columns.begin(), columnPos);
      std::cout << "Forbidden edge: <" << realColumn << ", " << p.first << ">" << std::endl;
      N1.M.at(removableEdge.second, matrixPos) = INF;
    } else {
      std::cout << "Nessesary edge not found." << std::endl;
    }
  }

  N1.rows.erase(std::find(N1.rows.begin(), N1.rows.end(), realRow));
  N1.columns.erase(std::find(N1.columns.begin(), N1.columns.end(), realColumn));

  N1.M.erase(N1.M.begin() + removableEdge.first);
  for (auto& e : N1.M) {
    e.erase(e.begin() + removableEdge.second);
  }

  N2.M.at(removableEdge.first, removableEdge.second) = INF;
  return {std::move(N1), std::move(N2)};
}

void forEachTreeElem(Node& node, std::function<void(Node &)> func) {
    if(node.left)   forEachTreeElem(*node.left, func);
    if(node.right)   forEachTreeElem(*node.right, func);

    func(node);
}

Node& findMin(Node& root) { 
  Node *minNode = 0;
  int minBypass = 2000;
  forEachTreeElem(root, [&](Node &node)
    {   
        if (node.left) // Есть дочерние узлы, пропускаем этот узел.
            return;
            
        if (node.bypass < minBypass){
            minNode = &node;
            minBypass = node.bypass;
        }
        std::cout << node.bypass <<", ";
    });
    std::cout << std::endl;
    return *minNode;
}

Node& bypass(Node& root) {

  std::pair<int, std::pair<int, int>> removableEdge;
  std::pair<Matrix, Matrix> M12;
  std::pair<Node,Node> N12;
  Node* newNode = &root;
  int curBottom = newNode->M.substractMatrix();
  newNode->bypass = curBottom;
  newNode->path = {};
  // /std::cout << curBottom <<"<";
  //newNode->bypass += removableEdge.first;
  
  while(newNode->M.size() > 2) {
    removableEdge = newNode-> M.detectRemovableEdge();
    N12 = sepMatrix(newNode, removableEdge.second);
    Node &N1 = N12.first, &N2 = N12.second;
    newNode->left = std::make_unique<Node>(N1.M);
    newNode->right = std::make_unique<Node>(N2.M);

    newNode->left->rows = N1.rows;
    newNode->right->rows = N2.rows;
    
    newNode->left->columns = N1.columns;
    newNode->right->columns = N2.columns;

    newNode->left->path = N1.path;
    newNode->right->path = N2.path;

    int leftCoef = newNode->left->M.substractMatrix();
    int rightCoef = newNode->right->M.substractMatrix();
    
    for(const auto& i: newNode->left->rows) 
      std::cout << i <<"-";
    std::cout << std::endl;

    for(const auto& i: newNode->left->columns) 
      std::cout << i <<"-";
    std::cout << std::endl;

    std::cout << "Left Matrix: " << std::endl;
    N1.M.printMatrix();

    for(const auto& i: newNode->right->rows) 
      std::cout << i <<"-";
    std::cout << std::endl;

    for(const auto& i: newNode->right->columns) 
      std::cout << i <<"-";
    std::cout << std::endl;

    std::cout << "Right Matrix: " << std::endl;
    N2.M.printMatrix();

    std::cout << "Left coef: " << leftCoef << std::endl 
              << "Right coef :" << rightCoef << std::endl;

    newNode->left->bypass = newNode->bypass + leftCoef;
    newNode->right->bypass = newNode->bypass + rightCoef;
    
    int realRow = newNode->rows.at(removableEdge.second.first);
    int realColumn = newNode->columns.at(removableEdge.second.second);
    
    std::cout << "Edge to remove: <"<< realRow << ","
              << realColumn << ">" << std::endl;
    
    newNode = &findMin(root);
    newNode->path.push_back(std::make_pair(realRow, realColumn));

    for(const auto& i: newNode->path) 
      std::cout << i.first << ":" << i.second <<"-";

    std::cout << std::endl << "New matrix: " << std::endl;
    newNode->M.printMatrix(); 
  }
  int nextVal = 0, commonVal = 0, lastVal = 0;
  for(size_t i = 0; i < 2; ++i){
    if(newNode->rows[i] == newNode->path[newNode->path.size()-1].second)
      nextVal = newNode->rows[i];
    if(newNode->columns[i] == newNode->path[newNode->path.size()-1].second)
      nextVal = newNode->columns[i];
    if(newNode->rows[i] == newNode->columns[i])
      commonVal = newNode->rows[i];
  }
  std::vector<int> c(newNode->rows.size() + newNode->columns.size());
  c.insert(c.end(), newNode->rows.begin(),  newNode->rows.end());
  c.insert(c.end(), newNode->columns.begin(),  newNode->columns.end());
  for(const auto& i: c){
    if(i != nextVal && i != commonVal)
      lastVal = i;
  }
  newNode->path.push_back({nextVal,commonVal});
  newNode->path.push_back({commonVal,lastVal});
  return *newNode;
}

void findSolution(Matrix& M) {
  M.changeZeros();
  Node Tree(M);

  for (int i = 0; i < M.size(); ++i) {
    Tree.rows.push_back(i+1);
    Tree.columns.push_back(i+1);
  }
  Node& R= bypass(Tree);

  for (size_t i = 0; i < R.path.size(); ++i) {
    if( i == R.path.size()-1) {
      std::cout << "(" << R.path[i].first << "," << R.path[i].second << ")";
    } else {
      std::cout << "(" << R.path[i].first << "," << R.path[i].second << "),";
    }
  }
  std::cout << std::endl;
  //std::cout << Tree.bypass << std::endl;
}

int main() {
  Matrix M({{0, 4, 6, 4, 5, 7},
            {4, 0, 5, 7, 9, 3},
            {6, 5, 0, 7, 4, 5},
            {8, 7, 7, 0, 6, 4},
            {5, 9, 4, 6, 0, 9},
            {7, 3, 5, 4, 9, 0}});
  findSolution(M);
  return 0;
}