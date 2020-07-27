#include "Containers.h"

#include <iostream>

using namespace containers;


priorityQueue::priorityQueue(){
    first_ = nullptr;
    last_ = nullptr;
}

priorityQueue::~priorityQueue(){
    Pair * current = first_;
    Pair * next = nullptr;
    while (current != nullptr) {
        next = current->next;
        delete current;
        current = next;
    }
}

void priorityQueue::push(int x, int priority){
    Pair * npair = new Pair(x, priority);

    if (first_ == nullptr) {
        first_ = npair;
        last_ = first_;
        return;
    }
    if (first_->priority > priority) {
        npair->next = first_;
        first_ = npair;
    } else {
        Pair * current = first_;
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


int priorityQueue::pop(){
    if (first_ == nullptr) {return -1;}
    int value = first_->value;
    Pair * old = first_;
    first_ = first_->next;
    delete old;
    return value;
}