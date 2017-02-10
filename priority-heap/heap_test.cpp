#include "heap.hpp"
#include <iostream>

bool tiebreaker(int& x1, int p1, int& x2, int p2) {
  return x1 > x2;
}

int main() {
  MaxHeap<int, tiebreaker> myHeap;
  int iterations = 100;
  for (int i = 0; i < iterations; i++) {
    myHeap.push(i, i);
  }
  myHeap.push(100, 99);
  myHeap.push(98, 99);
  myHeap.decKey(50, 10000);
  Prioritized<int> popped;
  for (int i = 0; i < iterations + 2; i++) {
    popped = myHeap.pop();
    std::cout << "CONTENT: " << popped.content << " PRIORITY: " << popped.priority << std::endl;
  }
}
