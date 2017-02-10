#include <iostream>
#include <random>

int main() {
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::uniform_int_distribution<int> distribution(1,100);
  std::mt19937 mt(seed);
  for (int i = 0; i < 10; i++) {
    std::cout << distribution(mt) << std::endl;
  }
}
