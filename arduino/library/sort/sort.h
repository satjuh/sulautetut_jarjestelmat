namespace sort {
    template <class Container>    
    void insertionSort(Container& t);
}

// See https://www.geeksforgeeks.org/insertion-sort/ for more details
// just made it into a template function
template <class Container> void sort::insertionSort(Container & c) {
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
