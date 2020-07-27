#ifndef Containers_h 
#define Containers_h 

#include <iostream>

namespace containers{
    // First is the priority
    // Second is the value
    struct Pair {
        int value;
        int priority;
        Pair * next;
        // Simple copy constructor
        Pair() = default;
        Pair(int y, int x) {value= y; priority = x;}
        Pair(const Pair& p){ value = p.value; priority = p.priority; }
    };

    class priorityQueue{
        public:
            priorityQueue();
            ~priorityQueue();
            // Pop the first element in queue
            int pop();
            // Push element to the queue
            void push(int x, int priority);
        private:
            // class variables
            Pair * first_;
            Pair * last_;
    };

    template <class Type> class ownArray{
        public:
            ownArray(unsigned int size = 8);
            ~ownArray();
            // Return the size of the array
            unsigned int size();
            // [] access operator
            Type & operator [](unsigned int i);
            // Push element to the end of the array
            void push_back(Type v);
            // Remove element from the end of the array and return the element
            Type pop_back();
            // Last element in the array
            Type back();
            // First element in the array
            Type front();
        private:
            void doubleSize();
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

template <class Type> containers::ownArray<Type>::~ownArray(){
    delete arr_;
}

template <class Type> unsigned int containers::ownArray<Type>::size(){
    return currentSize_;
}

template <class Type> Type & containers::ownArray<Type>::operator[](unsigned int i) {
    return arr_[i];
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

template <class Type> Type containers::ownArray<Type>::pop_back(){
    if (currentSize_ < 0) {
        throw "Can't pop empty array!";
    }
    --currentSize_;
    return arr_[currentSize_]; 
}

template <class Type> Type containers::ownArray<Type>::back(){ 
    return currentSize_ > 0 ? arr_[currentSize_ - 1] : throw "Can't index empty array!";
}

template <class Type> Type containers::ownArray<Type>::front(){ 
    return currentSize_ > 0 ? arr_[0] : throw "Can't index empty array!";
}

template <class Type> void containers::ownArray<Type>::doubleSize(){
    Type * newArr = new Type[ 2* maxSize_];
    for (unsigned int i = 0; i < maxSize_; ++i) {
        newArr[i] = arr_[i];
    }
    delete arr_;
    maxSize_ = 2 * maxSize_;
    arr_ = newArr;
    return;
}


#endif