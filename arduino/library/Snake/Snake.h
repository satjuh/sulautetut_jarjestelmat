#ifndef Snake_h 
#define Snake_h 


#include "Containers.h" 

using namespace containers;

// Up = 0, down = 1, left = 2, right = 3, last = 4
enum direction {UP, DOWN, LEFT, RIGHT, LAST};

// Playfield for the snake game
// food = food position
// height = height of the playfield
// width = width of the playfield
struct Playfield {
    ownArray<int> snake;
    int food;
    int height;
    int width;
};

// Parent child required to track the path when finding path to the food with 
// path finding
struct ParChild{
    int value;
    // value -1 means no parent
    int parent;
    // direction last means that this parchild was the starting point
    direction d;
    // Default builder required for ownArray initalization
    ParChild(){
        value = -1;
        d = LAST;
        parent = -1;
    }
    ParChild(int v, int p, direction dir) {
        parent = p;
        value = v;
        d = dir;
    }
    ParChild( const ParChild & obj){
        // Copy builder
        value = obj.value;
        d = obj.d;
        parent = obj.parent;
    }
    bool operator<(const ParChild& obj){
        return value < obj.value;
    }
};


class Snake {
    public:
        // Height and width of the playfield
        Snake(int height, int width, int size = 3);
        // Destructor
        ~Snake();
        // Snake moves towards goal
        void nextFrame(void (*f)(byte arr [] ));
        // get board as byte array requires board size
        // void getBoard(byte * bytes, int size);
        void getBoard();
        // Might not be needed anymore.
        // Get board based on a snake that is in a different position
        ownArray<int> getBoard(const ownArray<int> & snake);
        // Start a snake game
        bool startGame();
        // Return play field for debugging
        Playfield getPlayfield();
        // get if game is still going
        bool onGoing();
    private:
        // Moves snake in a given direction does not care wheter you 
        // can or cannot move to that direction. 
        void move(direction d, ownArray<int> & snake, bool forward = true);
        // Check wheter moving to a given direction is possible
        // head parameter tells wheter we are looking the head or tail
        bool checkMove(direction d, ownArray<int> & snake, bool head=true);
        // Try to grow in directions given
        bool tryToGrow(direction * ds, int size);
        // Increse snake size
        void grow();
        // Add food to the board whre there is no snake
        void addFood();
        // Make a super simple hamiltonianCycle for a square layout with height dividable with 8
        // hamiltonianCycle shows a route around the playfield that can be used to make the moves
        // https://en.wikipedia.org/wiki/Hamiltonian_path_problem
        void makeHamiltonianCycle();
        // Find the shortest path to food without hitting snake body
        ownArray<direction> findPath();
        // Convert array index point to x and y coordinates 
        void convert(int point, int & x, int & y);
        // convert x, y coordinates to array index
        int convert(int x, int y);
        // Check whether value is in containers
        bool checkIfValueInContainer(ownArray<int> c, int v);
        // for clearing the board
        void clearBoard();
        // Playfield used in snake
        Playfield playfield_;
        // snake size 
        int length_;
        // Is game ongoing
        bool ongoing_;
        // Is food available
        bool food_;
        // cycle 
        byte * board_;
        ownArray<int> hamiltonianCycle_;
};


#endif