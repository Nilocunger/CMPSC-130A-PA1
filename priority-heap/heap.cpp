template<typename Content>
class PriorityContainer {
  public: 
    Content c;
    int priority;
    
    bool operator==(PriorityContainer<Content> const& rhs) {  return this->priority == rhs.priority;  }
    bool operator<(PriorityContainer<Content> const& rhs) {  return this->priority < rhs.priority;  }
    bool operator>(PriorityContainer<Content> const& rhs) {  return this->priority > rhs.priority;  }

    bool operator!=(PriorityContainer<Content> const& rhs) {  return !(*this == rhs);  }
    bool operator<=(PriorityContainer<Content> const& rhs) {  return (*this == rhs) || (*this < rhs);  }
    bool operator>=(PriorityContainer<Content> const& rhs) {  return (*this == rhs) || (*this > rhs);  }
};

enum HeapErrorCodes {OVERRIDE, NO_ELEMENT}; 
template<typename NodeContents>
class Heap {
  private:
    PriorityContainer<NodeContents> *contents;
    int size;
    int numElems;

    void reheap();

    void place(PriorityContainer<NodeContents> x, int index);
    void placeLast(PriorityContainer<NodeContents> x, int index) {
      this->place(x, index);
    }

    PriorityContainer<NodeContents> grab(int index);
    PriorityContainer<NodeContents> grabLast(int index) { 
      return this->grab(this->numElems - 1);
    };

    void swap(int index1, int index2);
    void swapEnds();

    virtual bool moreTop(PriorityContainer<NodeContents>& x1, 
                         PriorityContainer<NodeContents>& x2);

    int percolateDown(int index);
    int percolateUp(int index);

    int getRightChildIndex(int index);
    int getLeftChildIndex(int index);
    int getChildIndex(int index, bool isLeft);
    PriorityContainer<NodeContents> getRightChild(int index);
    PriorityContainer<NodeContents> getLeftChild(int index);
    PriorityContainer<NodeContents> getChild(int index, bool isLeft);
    int getParentIndex(int index);
    PriorityContainer<NodeContents> getParent(int index);
  public:
    Heap();
    Heap(int initialSize);
    ~Heap();

    void push(PriorityContainer<NodeContents> x);
    PriorityContainer<NodeContents> pop();

    void change(int index, int val);
};

template<typename NodeContents>
class MinHeap : public Heap<NodeContents> {
  private:
    bool moreTop(PriorityContainer<NodeContents>& x1, 
                    PriorityContainer<NodeContents>& x2) { 
      return x1 > x2;
    }
};

template<typename NodeContents>
class MaxHeap : public Heap<NodeContents> {
  private:
    bool moreTop(PriorityContainer<NodeContents>& x1, 
                    PriorityContainer<NodeContents>& x2) { 
      return x1 < x2;
    }
};

// Constructors and Destructors
template<typename NodeContents>
Heap<NodeContents>::Heap(int initialSize) {
  this->contents = new PriorityContainer<NodeContents>[initialSize];
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
  if (this->numElems >= this->size) {
    int newSize = this->numElems * 2;
    PriorityContainer<NodeContents> *newContents = new PriorityContainer<NodeContents>[newSize];
    for (int i = 0; i < this->numElems; i++) {
      newContents[i] = this->contents[i];
    }
    delete[] this->contents;
    this->contents = newContents;
    this->size = newSize;
  }

  this->placeLast(x);
  this->percolateUp();
}

template<typename NodeContents>
PriorityContainer<NodeContents> Heap<NodeContents>::pop() {
  PriorityContainer<NodeContents> toReturn = this->grab(0);
  this->place(this->grabLast(), 0);
  this->percolateDown();
  return toReturn;
}
 

// Percolation
template<typename NodeContents>
int Heap<NodeContents>::percolateDown(int index) {
  auto leftChild = this->getLeftChild(index);
  auto rightChild = this->getRightChild(index);
  if (!leftChild) {  return index;  }
  if (this->moreTop(leftChild, rightChild)) {
    this->place(leftChild, index);
    return this->percolateDown(this->getLeftChildIndex(index));
  } else {
    this->place(rightChild, index);
    return this->percolateDown(this->getRightChildIndex(index));
  }
}

template<typename NodeContents>
int Heap<NodeContents>::percolateUp(int index) {
  if (!this->getParent(index)) {  return index;  }
  if (this->getParent(index) > this->contents[index]) {
    int parentIndex = this->getParentIndex();
    this->swap(parentIndex, index);
    this->percolateUp(parentIndex);
  }
}


// Child and Parent methods
template<typename NodeContents>
PriorityContainer<NodeContents> Heap<NodeContents>::getLeftChild(int index) {
  return this->getChild(index, true);
}

template<typename NodeContents>
PriorityContainer<NodeContents> Heap<NodeContents>::getRightChild(int index) {
  return this->getChild(index, false);
}

template<typename NodeContents>
PriorityContainer<NodeContents> Heap<NodeContents>::getChild(int index, bool isLeft) {
  return this->contents[getChildIndex(index, isLeft)];
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
  return (index - 1) / 2;
}

template<typename NodeContents>
PriorityContainer<NodeContents> Heap<NodeContents>::getParent(int index) {
  return this->contents[this->getParentIndex(index)];
}


// Element moving
template<typename NodeContents>
void Heap<NodeContents>::place(PriorityContainer<NodeContents> x, int index) {
  if (this->contents[index]) {
    throw OVERRIDE;
  }

  this->contents[index] = x;
  this->numElems++;
}

template<typename NodeContents>
PriorityContainer<NodeContents> Heap<NodeContents>::grab(int index) {
  if (!this->contents[index]) {
    throw NO_ELEMENT;
  }

  PriorityContainer<NodeContents> toReturn = this->contents[index];
  delete this->contents[index];
  this->numElems--;
  return toReturn;
}

template<typename NodeContents>
void Heap<NodeContents>::swap(int index1, int index2) {
  PriorityContainer<NodeContents> temp = this->grab(index1);
  this->place(this->grab(index2), index1);
  this->place(temp, index2);
}

template<typename NodeContents>
void Heap<NodeContents>::swapEnds() {
  this->swap(this->numElems, 0);
}
