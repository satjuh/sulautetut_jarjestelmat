#ifndef Containers_h 
#define Containers_h 


namespace containers{
    // First is the priority
    // Second is the value
    template <class Type>
    struct Pair {
        Type value;
        int priority;
        Pair * next;
        // Simple copy constructor
        Pair() = default;
        Pair(Type v, int x) {value= v; priority = x;}
        Pair(const Pair& p){ value = p.value; priority = p.priority; }
    };

    template <class Type>
    class priorityQueue{
        public:
            priorityQueue();
            ~priorityQueue();
            // Pop the first element in queue
            Type pop();
            // Push element to the queue
            void push(Type x, int priority);
        private:
            // class variables
            Pair<Type> * first_;
            Pair<Type> * last_;
    };

    template <class Type> class ownArray{
        public:
            // Default constructor
            ownArray(unsigned int size = 8);
            // Multiple inital values constructor
            ownArray(unsigned int size, Type value);
            // Copy constructor to make deep copies of objects
            ownArray(const ownArray &obj);
            // Destructor
            ~ownArray();
            // Return the size of the array
            unsigned int size();
            // [] access operator
            Type & operator [](unsigned int i);
            // const [] access operator
            const Type & operator [](unsigned int i) const;
            // =-operator for correct copying of objects
            void operator = (const ownArray & obj); 
            // Push element to the end of the array
            void push_back(Type v);
            // Push element to the front of the array. Not very efficient
            void push_front(Type v);
            // Remove element from the end of the array and return the element
            void pop_back();
            // Add an element to the front of the array. Not very efficient!
            void pop_front();
            // Last element in the array
            Type back();
            // First element in the array
            Type front();
        private:
            void doubleSize();
            void clearFirst();
            Type * arr_;
            unsigned int maxSize_;
            unsigned int currentSize_;
    };

}


template <class Type> containers::ownArray<Type>::ownArray(unsigned int size){
    arr_ = new Type [size];
    maxSize_ = size;
    currentSize_ = 0;
}
template <class Type> containers::ownArray<Type>::ownArray(unsigned int size, Type value){
    arr_ = new Type [size];
    maxSize_ = size;
    for (unsigned int i = 0; i < maxSize_; ++i){
        arr_[i] = value;
    }
    currentSize_ = maxSize_;
}


template <class Type> containers::ownArray<Type>::ownArray(const ownArray &obj){
    arr_ = new Type [obj.maxSize_];

    for (unsigned int i = 0; i < obj.currentSize_; ++i){
        arr_[i] = obj.arr_[i];
    }
    maxSize_ = obj.maxSize_;
    currentSize_ = obj.currentSize_;
}

template <class Type> containers::ownArray<Type>::~ownArray(){
    delete [] arr_;
}

template <class Type> unsigned int containers::ownArray<Type>::size(){
    return currentSize_;
}

template <class Type> Type & containers::ownArray<Type>::operator[](unsigned int i) {
    return arr_[i];
}

template <class Type> const Type & containers::ownArray<Type>::operator[](unsigned int i) const{
    return arr_[i];
}

template <class Type> void containers::ownArray<Type>::operator= (const ownArray & obj){
    if (currentSize_ <= obj.currentSize_){
        delete [] arr_;
        arr_ = new Type[obj.maxSize_];
        maxSize_ = obj.maxSize_;
    }
    currentSize_ = obj.currentSize_;

    for (unsigned int i = 0; i < obj.currentSize_; ++i){
        arr_[i] = obj.arr_[i];
    }
    return;
}

template <class Type> void containers::ownArray<Type>::push_back(Type v){
    if (currentSize_ < maxSize_){
        arr_[currentSize_] = v;
        ++currentSize_;
    } else {
        doubleSize();
        push_back(v);
    }
}

template <class Type> void containers::ownArray<Type>::push_front(Type v){
    if (currentSize_ < maxSize_){
        clearFirst();
        arr_[0] = v;
        ++currentSize_;
    } else {
        doubleSize();
        push_front(v);
    }
}

template <class Type> void containers::ownArray<Type>::pop_back(){
    if (currentSize_ <= 0) {
        return;
    }
    --currentSize_;
    return;
}

template<class Type> void containers::ownArray<Type>::pop_front(){
    if (currentSize_ <= 0){
        return;
    }

    for (unsigned int i = 0; i < currentSize_ - 1; ++i){
        arr_[i] = arr_[i+1];
    }

    --currentSize_;
    return;
}

template <class Type> Type containers::ownArray<Type>::back(){ 
    // orginal version, Arduino doesn't support exceptions
    // return currentSize_ > 0 ? arr_[currentSize_ - 1] : throw "Can't index empty array!";
    // dangerous
    return arr_[currentSize_ - 1];
}

template <class Type> Type containers::ownArray<Type>::front(){ 
    // orginal version, Arduino doesn't support exceptions
    // return currentSize_ > 0 ? arr_[0] : throw "Can't index empty array!";
    // dangerous will break if the array is empty
    return arr_[0];
}

template <class Type> void containers::ownArray<Type>::doubleSize(){
    Type * newArr = new Type[ 2* maxSize_];
    for (unsigned int i = 0; i < maxSize_; ++i) {
        newArr[i] = arr_[i];
    }
    delete [] arr_;
    maxSize_ = 2 * maxSize_;
    arr_ = newArr;
    return;
}


template <class Type> void containers::ownArray<Type>::clearFirst(){
    if (currentSize_ == 0) {
        return;
    }

    for (unsigned int i = currentSize_; i -->0;){
        arr_[i + 1] = arr_[i];
    }
}


template <class Type> containers::priorityQueue<Type>::priorityQueue(){
    first_ = nullptr;
    last_ = nullptr;
}

template <class Type> containers::priorityQueue<Type>::~priorityQueue(){
    Pair<Type> * current = first_;
    Pair<Type> * next = nullptr;
    while (current != nullptr) {
        next = current->next;
        delete current;
        current = next;
    }
}

template <class Type> void containers::priorityQueue<Type>::push(Type x, int priority){
    Pair<Type> * npair = new Pair<Type>(x, priority);

    if (first_ == nullptr) {
        first_ = npair;
        last_ = first_;
        return;
    }
    if (first_->priority > priority) {
        npair->next = first_;
        first_ = npair;
    } else {
        Pair<Type> * current = first_;
        npair->next = nullptr;
        
        while (current->next != nullptr) {
            if (current->priority == priority){
                npair->next = current->next;
                current->next = npair;
                return;
            }
            if (current->priority < priority && priority < current->next->priority) {
                npair->next = current->next;
                current->next = npair;
                return;
            }
            current = current->next;
        }

        // Insert to end
        last_->next = npair;
        last_ = npair;
    }
}

template <class Type> Type containers::priorityQueue<Type>::pop(){
    if (first_ == nullptr) {
        return;
    }
    Type value = first_->value;
    Pair<Type> * old = first_;
    first_ = first_->next;
    delete old;
    return value;
}

#endif
