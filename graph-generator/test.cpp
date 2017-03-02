#include <iostream>

#include "generator.hpp"

int main() {
  Graph g(10, 5);
  const int * const* adjMatrix = g.getAdjMatrix();
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      std::cout << adjMatrix[i][j];
      std::cout << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}

