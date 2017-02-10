#include "pqueue.h"

template<typename Contents>
class PriorityQueue {
  private:
    MinHeap<Contents> heap;
  public:
    PriorityQueue() : heap() { }
    PriorityQueue(int size) : heap(size) { }

    void push(Content& c) {  this->heap.push(c);  }
    Content pop() {  return this->heap.pop();  }
    bool isEmpty() {  return this->heap.isEmpty();  }
}
