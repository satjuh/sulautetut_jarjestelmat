#ifndef Snake_h 
#define Snake_h 


#include "Containers.h" 

using namespace containers;


struct Playfield {
    ownArray<int> snake;
    int food;
    int height;
    int width;
};

enum direction {UP, DOWN, LEFT, RIGHT};

class Snake {
    public:
        // Height and width of the playfield
        Snake(int height, int width, int size = 3);
        // Destructor
        ~Snake();
        // Snake moves towards goal
        void nextFrame(direction d);
        // get board as byte array requires board size
        // void getBoard(byte * bytes, int size);
        ownArray<int> getBoard();
        // Start a snake game
        bool startGame();
        // Return play field for debugging
        Playfield getPlayfield();
    private:
        // Moves snake in a given direction does not care wheter you 
        // can or cannot move to that direction. 
        void move(direction d, ownArray<int> & snake);
        // Check wheter moving to a given direction is possible
        // head parameter tells wheter we are looking the head or tail
        bool checkMove(direction d, ownArray<int> & snake, bool head=true);
        // Try to grow in directions given
        bool tryToGrow(direction * ds, int size);
        // Increse snake size
        void grow();
        // Add food to the board whre there is no snake
        void addFood();
        // Find the shortest path to food without hitting snake body
        bool findPath(direction * dA);
        // Calculate the eukledian distance between food and a point in the board
        int euclideanDistance(int point); 
        // Convert array index point to x and y coordinates 
        void convert(int point, int & x, int & y);
        // Playfield used in snake
        Playfield playfield_;
        // snake size 
        int length_;
        // Is game ongoing
        bool ongoing_;
        // Is food available
        bool food_;
};


#endif