#include "sort.h"
#include <deque>

// See https://www.geeksforgeeks.org/insertion-sort/ for more details
// just made to it use deque
void sort::insertionSort(std::deque<int> & c) {
    int i, key, j;
    for (int i = 1; i < c.size(); ++i){
        key = c[i];
        j = i - 1;

        while ( j >= 0 && c[j] > key){
            c[j + 1] = c[j];
            j = j - 1;
        }
        c[j + 1] = key;
    }
}
