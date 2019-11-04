#ifndef MATRIX_H
#define MATRIX_H

#define INF std::numeric_limits<int>::max()

class Matrix {
 public:
  Matrix(std::vector<std::vector<int>> M) : M(M) {}
  Matrix() {}
  void printMatrix();
  void changeZeros();
  int substractMatrix();
  std::pair<int, std::pair<int, int>> detectRemovableEdge();

  void erase(std::vector<std::vector<int>>::iterator iter);
  auto begin();
  auto end();
  int &at(int i, int j);
  int size();

  int findMinInString(int stringNumber, int excludeNUmber = -1);
  int findMinInColunmn(int columnNumber, int excludeNumber = -1);

 private:
  std::vector<std::vector<int>> M;
};

#endif
#endif