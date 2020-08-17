#include <Arduino.h>

#include "Snake.h"
#include "Containers.h"
#include "../sort/sort.h"

using namespace containers;

Snake::Snake(int height, int width, int size){
    // playfield width height
    playfield_.height = height;
    playfield_.width = width;
    // Initialize playfield and snake
    length_ = size;
    playfield_.food = 0;
    ongoing_ = false;
    food_ = false;
    board_ = new byte [height];
    // Assuming 8 x 8 grid even though the whole program is modular to some extent
    clearBoard();
    ownArray<int> temp(width*height, 0);
    hamiltonianCycle_ = temp;
    makeHamiltonianCycle();
}

Snake::~Snake(){
}


bool Snake::startGame(){
    if (ongoing_){
        return false; // don't start game if it is still going
    }
    int a = 0;
    int b = 0;
    // Put food in a random position
    while (a == b) {
        a = random(0, playfield_.height * playfield_.width-1);
        //a = random(0, playfield_.height * playfield_.width);
        //b = random(0, height*width-1);
        b = (2 * playfield_.height)+ 3;
    }
    
    for (unsigned int i = 0; playfield_.snake.size(); ++i){
        playfield_.snake.pop_back();
    }
    length_ = 3;
    // Add food to playfield
    playfield_.food = a;
    food_ = true;
    // clear snake containers
    for (unsigned int i = 0; i < playfield_.snake.size(); ++i){
        playfield_.snake.pop_back();
    }
    // Add snake body to the left side of the snake
    for (int i = 0; i < length_; ++i){
        playfield_.snake.push_back(b - i);
    }
    ongoing_ = true;
    return true;

}


void Snake::grow(){
    ++ length_;
    // TODO what if we can't grow in any direction
    // Tail grow up 
    if (playfield_.snake[playfield_.snake.size() -2] - playfield_.height == playfield_.snake.back()) {
        direction ds [3] = {UP, LEFT, RIGHT};
        tryToGrow(ds, 3);
    // Tail grow up 
    } else if (playfield_.snake[playfield_.snake.size() -2] + playfield_.height == playfield_.snake.back()) {
        direction ds [3] = {DOWN, LEFT, RIGHT};
        tryToGrow(ds, 3);
    // Tail grow left
    } else if (playfield_.snake[playfield_.snake.size() -2] - 1 == playfield_.snake.back()) {
        direction ds [3] = {LEFT, UP, DOWN};
        tryToGrow(ds, 3);
    // Tail grow right
    } else if (playfield_.snake[playfield_.snake.size() -2] + 1 == playfield_.snake.back()) {
        direction ds [3] = {RIGHT, UP, DOWN};
        tryToGrow(ds, 3);
    }
}


void Snake::addFood(){
    int a = 0;
    // Flag to set if snake found in food position
    bool found = false;
    while (true) {
        a = random(0, playfield_.height * playfield_.width - 1);
        found = false;
        for (unsigned int i = 0; i < playfield_.snake.size(); ++i){
            if (playfield_.snake[i] == a){
                found = true;
                break;
            }
        }
        if (!found) {
            playfield_.food = a;
            break;
        }
    }

    return;
}

void Snake::makeHamiltonianCycle(){
    // hamiltonianCycle_ used as and map [x] = next position
    int x, y;
    x = playfield_.width - 1;
    y = 0;
    // Start from right top
    bool moveLeft = true;
    bool moveUp = false;
    for (unsigned int i = 0; i < playfield_.height * playfield_.width; ++i){
        // First line
        if (!moveUp){
            if (x == 0 && moveLeft){
                hamiltonianCycle_[convert(x,y)] = convert(x, y + 1);
                moveLeft = false; 
                ++y;
            } else if (x == playfield_.width - 2 && y > 0 && y < playfield_.height - 1 && !moveLeft){
                hamiltonianCycle_[convert(x,y)] = convert(x, y + 1);
                moveLeft = true;
                ++y;
            } else if (y == playfield_.height - 1 && x == playfield_.width - 1) {
                hamiltonianCycle_[convert(x,y)] = convert(x, y - 1);
                moveUp = true;
                --y;
            } else {
                if (moveLeft){
                    hamiltonianCycle_[convert(x,y)] = convert(x - 1, y);
                    --x;
                } else {
                    hamiltonianCycle_[convert(x, y)] = convert(x + 1, y);
                    ++x;
                }
            }    
        } else {
            if (y == 0) {
                hamiltonianCycle_[convert(x, y)] = convert(x - 1, y);
                break;
            } else {
                hamiltonianCycle_[convert(x, y)] = convert(x, y - 1);
                --y;
            }
        }
    }
}


void Snake::move(direction d, ownArray<int> & snake, bool forward) {

    switch (d) {
        case UP:
            if(forward){
                snake.push_front(snake.front() - playfield_.height);
                snake.pop_back();
            } else {
                snake.pop_front();
                snake.push_back(snake[snake.size()-1] - playfield_.height);
            }
            break; 
        case DOWN:
            if (forward){
                snake.push_front(snake.front() + playfield_.height);
                snake.pop_back();
            } else {
                snake.pop_front();
                snake.push_back(snake[snake.size()-1] + playfield_.height);
            }
            break;
        case LEFT:
            if (forward) {
                snake.push_front(snake.front() - 1);
                snake.pop_back();
            } else {
                snake.pop_front();
                snake.push_back(snake[snake.size()-1] - 1);
            }
            break;
        case RIGHT:
            if (forward){
                snake.push_front(snake.front() + 1);
                snake.pop_back();
            } else {
                snake.pop_front();
                snake.push_back(snake[snake.size()-1] + 1);
            }
            break;
    };
}


bool Snake::checkMove(direction d, ownArray<int> & snake, bool head) {
    int newHead = 0;
    int begin = head ? snake.front() : snake.back();
    switch (d) {
        case UP:
            if (begin > playfield_.width-1){
                newHead = begin - playfield_.width;
            } else {
                return false;
            }
            break; 
        case DOWN:
            if (begin < ((playfield_.width- 1) * playfield_.height)){
                newHead = begin + playfield_.height;
            } else {
                return false;
            }
            break;

        case LEFT:
            if (begin % playfield_.width != 0) {
                newHead = begin - 1;
            } else {
                return false;
            }
            break;
        case RIGHT:
            if ( (begin + 1) % playfield_.width != 0){
                newHead = begin + 1;
            } else {
                return false;
            }
            break;
    };
    for (unsigned int i = 0; i < snake.size(); ++i){
        if (newHead == snake[i]){
            return false;
        }
    }
    return true;
}

void Snake::getBoard() {
    ownArray<int> sorted =  playfield_.snake;
    sort::insertionSort(sorted);

    int index = 0;
    int position;
    bool stop = false;
    for (int i = 0; i < playfield_.height; ++i){
        for (unsigned int k = 0; k < playfield_.width; ++k) {
            position = i * playfield_.width + k;
            if (sorted[index] == position) {
                bitWrite(board_[i], k, 1);
                ++ index;
            }

            if (index == sorted.size()){
                stop = true;
                break;
            }
        }
        if (stop) {
            break;
        }
    }
    int x, y;
    convert(playfield_.food, x, y);
    bitWrite(board_[y], x, 1);
    return;
}

ownArray<int> Snake::getBoard(const ownArray<int>& snake) {
    ownArray<int> sorted =  snake;
    ownArray<int> result (playfield_.height * playfield_.width, 0);
    sort::insertionSort(sorted);

    int index = 0;
    int position;
    bool stop = false;
    for (int i = 0; i < playfield_.height; ++i){
        for (unsigned int k = 0; k < playfield_.width; ++k) {
            position = i * playfield_.width + k;
            if (sorted[index] == position) {
                result[position] = 1;
                ++ index;
            }

            if (index == sorted.size()){
                stop = true;
                break;
            }
        }
        if (stop) {
            break;
        }
    }
    result[playfield_.food] = 2;
    return result;
}

void Snake::nextFrame(void (*print)(byte arr[] )) {
    ownArray<direction> dirs = findPath();

    for (unsigned int i = 0; i < dirs.size(); ++i){
        clearBoard();
        getBoard();
        print(board_);
        move(dirs[i], playfield_.snake);
    }

    if (playfield_.snake.size() == playfield_.height * playfield_.width - 1){
        ongoing_=false;
        return;
    }
    if (playfield_.snake[0] == playfield_.food) {
        grow();
        addFood();
    }
}

Playfield Snake::getPlayfield() {
    return playfield_;
}


bool Snake::tryToGrow(direction * ds, int size) {
    bool end = false;
    for (int i = 0; i < size; ++i){
        switch (ds[i]){
            case UP:
                if (checkMove(UP, playfield_.snake, false)){
                    playfield_.snake.push_back(playfield_.snake.back() - playfield_.height);
                    end = true;
                }
                break;
            case DOWN:
                if (checkMove(DOWN, playfield_.snake, false)){
                    playfield_.snake.push_back(playfield_.snake.back() + playfield_.height);
                    end = true;
                }
                break;
            case LEFT:
                if (checkMove(LEFT, playfield_.snake, false)){
                    playfield_.snake.push_back(playfield_.snake.back() - 1);
                    end = true;
                }
                break;
            case RIGHT:
                if (checkMove(RIGHT, playfield_.snake, false)){
                    playfield_.snake.push_back(playfield_.snake.back() + 1);
                    end = true;
                }
                break;
            case LAST:
                break;
        }
        if (end) {
            break;
        }
    }    
    return end;
}


ownArray<direction> Snake::findPath() {
    ownArray<direction> result;
    int current = playfield_.snake.front();

    while (current != playfield_.food){
        int next = hamiltonianCycle_[current];
        // Up
        if (current - next == playfield_.height){
            result.push_back(UP);
        } else if (current + 1 == next) {
            result.push_back(RIGHT);
        } else if (current -1 == next) {
            result.push_back(LEFT);
        } else if (current - next == -playfield_.height){
            result.push_back(DOWN);
        }
        current = next;
    }
    return result;
}


void Snake::convert(int point, int & x, int & y) {
    x = (point % playfield_.width) + 1;
    y = int (point / playfield_.height) ;
    return;
}

int Snake::convert(int x, int y){
    return y * playfield_.height + x;
    
}

bool Snake::checkIfValueInContainer(ownArray<int> c, int v) {
    bool found = false;
    for (unsigned int i = 0; i < c.size(); ++i){
        if (c[i] == v){
            found = true;
            break;
        }
    }
    return found;
}


bool Snake::onGoing() {
    return ongoing_;
} 

void Snake::clearBoard(){
    for (unsigned int i = 0; i < 8; ++i) {
        board_[i] = B00000000;
    }
}