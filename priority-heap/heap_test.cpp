#include "heap.hpp"
#include <iostream>

int main() {
  MaxHeap<int> myHeap;
  int iterations = 100;
  for (int i = 0; i < iterations; i++) {
    myHeap.push(i, i);
  }
  for (int i = 0; i < iterations; i++) {
    std::cout << myHeap.pop().content << std::endl;
  }
}
