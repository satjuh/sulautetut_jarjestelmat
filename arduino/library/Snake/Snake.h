#ifndef Snake_h 
#define Snake_h 


#include <deque>


struct Playfield {
    std::deque<int> snake;
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
        std::deque<int> getBoard();
        // Start a snake game
        bool startGame();
        // Return play field for debugging
        Playfield getPlayfield();
    private:
        // Increse snake size
        void grow();
        // Add food to the board whre there is no snake
        void addFood();
        // Moves snake in a given direction does not care wheter you 
        // can or cannot move to that direction. 
        void move(direction d, std::deque<int> & snake);
        // Check wheter moving to a given direction is possible
        // head parameter tells wheter we are looking the head or tail
        bool checkMove(direction d, const std::deque<int> & snake, bool head=true);
        // Playfield used in snake
        Playfield playfield_;
        // snake size 
        int length_;
        // Is game ongoing
        bool ongoing_;
};


#endif