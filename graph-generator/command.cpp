#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "generator.hpp"

char *lpad(char *text, int length, char padWith) {
  char *padded = new char[length+1];
  int textLength = strlen(text);
  for (int i=0; i < length; i++) { 
    if (i >= length - textLength) {
      padded[i] = text[i - (length - textLength)];
    } else {
      padded[i] = padWith;
    }
  }
  padded[length] = '\0';
  return padded;
}

int getRandomSeed() {
  int fd = open("/dev/urandom", O_RDONLY);
  if (fd < 0) {
    return time(NULL);
  } else {
     unsigned int seed;
     if (read(fd, &seed, sizeof(seed))) {
       return seed;
     } else {
       return time(NULL);
     }
  }
}

int main(int argc, char **argv) {
  int seed;
  if (argc == 2) {
    seed = getRandomSeed();
  } else if (argc == 3) {
    seed = atoi(argv[2]);
  } else {
    std::cout << "Usage: generate <num_nodes> [<seed>]" << std::endl;
    exit(1);
  }

  int numNodes = atoi(argv[1]);
  Graph g(numNodes, seed);
  const int * const* adjMatrix = g.getAdjMatrix();
  for (int i = 0; i < numNodes; i++) {
    for (int j = 0; j < numNodes; j++) {
      char element[4];
      sprintf(element, "%d", adjMatrix[i][j]);
      char *padded = lpad(element, 3, ' ');
      std::cout << padded << " "; 
      delete[] padded;
    }
    std::cout << std::endl;
  }

  return 0;
}

