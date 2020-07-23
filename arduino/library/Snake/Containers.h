#ifndef Containers_h 
#define Containers_h 

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

    class ownArray{
        public:
            ownArray(unsigned int size);
            ~ownArray();
            int size();
            int & operator [](unsigned int i){arr_[i];};
            void push_back(int v);
            void back();
        private:
            int * arr_;
            int size_;
            int currentSize;
    };

}


#endif