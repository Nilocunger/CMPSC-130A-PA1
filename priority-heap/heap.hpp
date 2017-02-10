#ifndef HEAP_H
#define HEAP_H

template<typename Content>
class PriorityContainer {
  public: 
    PriorityContainer() : priority(0) { }
    PriorityContainer(Content content, int priority) : content(content), priority(priority) { }
    Content content;
    long long priority;
    
    bool operator==(PriorityContainer<Content> const& rhs) {  return this->priority == rhs.priority;  }
    bool operator<(PriorityContainer<Content> const& rhs) {  return this->priority < rhs.priority;  }
    bool operator>(PriorityContainer<Content> const& rhs) {  return this->priority > rhs.priority;  }

    bool operator!=(PriorityContainer<Content> const& rhs) {  return !(*this == rhs);  }
    bool operator<=(PriorityContainer<Content> const& rhs) {  return (*this == rhs) || (*this < rhs);  }
    bool operator>=(PriorityContainer<Content> const& rhs) {  return (*this == rhs) || (*this > rhs);  }
};

enum HeapErrorCodes {OVERRIDE, NO_ELEMENT, NO_CHILD}; 

template<typename Content>
using Prioritized = PriorityContainer<Content>;

template<typename NodeContents>
class Heap {
  private:
    PriorityContainer<NodeContents> *contents;
    int size;
    int occupied;

    int getLastIndex() {  return this->occupied - 1;  }
    int getOpenIndex() {  return this->occupied;  }

    void place(PriorityContainer<NodeContents> x, int index);

    PriorityContainer<NodeContents> grab(int index);

    virtual bool moreTop(PriorityContainer<NodeContents>& x1, 
                         PriorityContainer<NodeContents>& x2) {  return true;  }
    bool compare(int index1, int index2) {
      auto node1 = this->grab(index1);
      auto node2 = this->grab(index2);
      return this->moreTop(node1, node2);
    }
    int returnTopper(int index1, int index2);

    void percolateDown(int index);
    void percolateUp(int index);

    int getRightChildIndex(int index);
    int getLeftChildIndex(int index);
    int getChildIndex(int index, bool isLeft);
    PriorityContainer<NodeContents> getRightChild(int index);
    PriorityContainer<NodeContents> getLeftChild(int index);
    PriorityContainer<NodeContents> getChild(int index, bool isLeft);
    int getParentIndex(int index);
    PriorityContainer<NodeContents> getParent(int index);
    PriorityContainer<NodeContents> getNode(int index);
    bool hasNode(int index);
  public:
    Heap();
    Heap(int initialSize);
    ~Heap();

    void push(PriorityContainer<NodeContents> x);
    void push(NodeContents x, int priority) {
      this->push(PriorityContainer<NodeContents>(x, priority));
    }
    PriorityContainer<NodeContents> pop();

    void incKey(int index, int val) {
      this->contents[index].priority += val;
      this->percolateUp(index);
    }
    
    void decKey(int index, int val) {
      this->contents[index].priority -= val;
      this->percolateDown(index);
    }

    bool isEmpty() {  return this->occupied == 0;  }
};

template<typename Content>
using Tiebreaker = bool (*)(Content& c1, int p1, Content& c2, int p2);

template<typename NodeContents, Tiebreaker<NodeContents> onTie>
class MinHeap : public Heap<NodeContents> {
  private:
    bool moreTop(PriorityContainer<NodeContents>& x1, 
                 PriorityContainer<NodeContents>& x2) { 
      if (x1 == x2) {
        return onTie(x1.content, x1.priority, x2.content, x2.priority);
      } else {
        return x1 < x2;
      }
    }
};

template<typename NodeContents, Tiebreaker<NodeContents> onTie>
class MaxHeap : public Heap<NodeContents> {
  private:
    bool moreTop(PriorityContainer<NodeContents>& x1, 
                 PriorityContainer<NodeContents>& x2) { 
      if (x1 == x2) {
        return onTie(x1.content, x1.priority, x2.content, x2.priority);
      } else {
        return x1 > x2;
      }
    }
};

// Constructors and Destructors
template<typename NodeContents>
Heap<NodeContents>::Heap(int initialSize) {
  this->contents = new PriorityContainer<NodeContents>[initialSize];
  this->size = initialSize;
  this->occupied = 0;
}

template<typename NodeContents>
Heap<NodeContents>::Heap() : Heap(20) { }

template<typename NodeContents>
Heap<NodeContents>::~Heap() {
  delete[] this->contents;
}


// Element operations
template<typename NodeContents>
void Heap<NodeContents>::push(PriorityContainer<NodeContents> x) {
  if (this->occupied >= this->size) {
    int newSize = this->occupied * 2;
    PriorityContainer<NodeContents> *newContents = new PriorityContainer<NodeContents>[newSize];
    for (int i = 0; i < this->occupied; i++) {
      newContents[i] = this->contents[i];
    }
    delete[] this->contents;
    this->contents = newContents;
    this->size = newSize;
  }

  this->place(x, this->getOpenIndex());
  this->occupied++;
  this->percolateUp(this->getLastIndex());
}

template<typename NodeContents>
PriorityContainer<NodeContents> Heap<NodeContents>::pop() {
  PriorityContainer<NodeContents> toReturn = this->grab(0);
  if (this->getLastIndex() == 0) {
    this->occupied--;
    return toReturn;
  }
  auto lastNode = this->grab(this->getLastIndex());
  this->occupied--;
  this->place(lastNode, 0);
  this->percolateDown(0);
  return toReturn;
}
 

// Percolation
template<typename NodeContents>
void Heap<NodeContents>::percolateDown(int index) {
  int leftChildIndex = this->getLeftChildIndex(index);
  int rightChildIndex = this->getRightChildIndex(index);

  try {
    int compareAgainst = this->returnTopper(leftChildIndex, rightChildIndex);
    if (!this->compare(index, compareAgainst)) {
      auto tempNode = this->grab(compareAgainst);
      this->place(this->grab(index), compareAgainst);
      this->place(tempNode, index);
      return this->percolateDown(compareAgainst);
    }
  } catch (const HeapErrorCodes&) {
    return;
  }
}

template<typename NodeContents>
int Heap<NodeContents>::returnTopper(int index1, int index2) {
  if (!this->hasNode(index1) && !this->hasNode(index2)) {
    throw NO_CHILD;  
  } else if (!this->hasNode(index1)) {
    return index2;
  } else if (!this->hasNode(index2)) {
    return index1;
  } else {
    auto node1 = this->grab(index1);
    auto node2 = this->grab(index2);
    return (this->moreTop(node1, node2)) ? index1 : index2;
  }
}

template<typename NodeContents>
void Heap<NodeContents>::percolateUp(int index) {
  int parentIndex = this->getParentIndex(index);
  if (this->hasNode(parentIndex)) {
    auto currentNode = this->grab(index);
    auto parentNode = this->grab(parentIndex);
    if (this->moreTop(currentNode, parentNode)) {
      this->place(currentNode, parentIndex);
      this->place(parentNode, index);
      this->percolateUp(parentIndex);
    }
  }
}


template<typename NodeContents>
bool Heap<NodeContents>::hasNode(int index) {
  return (index >= 0) && (index < this->occupied);
}

template<typename NodeContents>
int Heap<NodeContents>::getLeftChildIndex(int index) {
  return this->getChildIndex(index, true);
}

template<typename NodeContents>
int Heap<NodeContents>::getRightChildIndex(int index) {
  return this->getChildIndex(index, false);
}

template<typename NodeContents>
int Heap<NodeContents>::getChildIndex(int index, bool isLeft) {
  if (isLeft) {
    return 2 * index + 1;
  } else {
    return 2 * index + 2;
  }
}

template<typename NodeContents>
int Heap<NodeContents>::getParentIndex(int index) {
  if (index == 0) {  return -1;  }
  return (index - 1) / 2;
}

// Element moving
template<typename NodeContents>
void Heap<NodeContents>::place(PriorityContainer<NodeContents> x, int index) {
  this->contents[index] = x;
}

template<typename NodeContents>
PriorityContainer<NodeContents> Heap<NodeContents>::grab(int index) {
  if (!this->hasNode(index)) {  throw NO_ELEMENT;  }
  return this->contents[index];
}

#endif
