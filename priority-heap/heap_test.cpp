#include "heap.h"
#include <iostream>

int main() {
  MinHeap<int> myHeap;
  myHeap.push(3);
  std::cout << myHeap.pop().content << std::endl; 
}
