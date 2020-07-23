#include <deque>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <queue>

#include <math.h>

#include "Snake.h"
#include "../sort/sort.h"

Snake::Snake(int height, int width, int size){
    // playfield width height
    playfield_.height = height;
    playfield_.width = width;
    // Initialize playfield and snake
    length_ = size;
    playfield_.food = 0;
    ongoing_ = false;
    food_ = false;
}

Snake::~Snake(){
}


int random (int lower, int upper) {
    srand((unsigned) time(0));
    return rand() % upper + lower + 1;
}

bool Snake::startGame(){
    if (ongoing_){
        return false; // don't start game if it is still going
    }
    int a = 0;
    int b = 0;
    // Put food in a random position
    while (a == b) {
        //std::cout << "Generating random number ";
        a = random(0, playfield_.height * playfield_.width-1);
        //a = random(0, playfield_.height * playfield_.width);
        //b = random(0, height*width-1);
        b = (2 * playfield_.height)+ 3;
        //std::cout << a << " b:" << b  << std::endl;
    }
    // Add food to playfield
    playfield_.food = a;
    food_ = true;
    // Add snake body to the left side of the snake
    for (int i = 0; i < length_; ++i){
        //std::cout << "Snake " << i << "=" << b - i << std::endl;
        playfield_.snake.push_back(b - i);
    }
    return true;

}


void Snake::grow(){
    ++ length_;
    // TODO what if we can't grow in any direction
    // Tail grow up 
    std::cout << "sl " << playfield_.snake[playfield_.snake.size() -2 ] << " l " << playfield_.snake.back() << std::endl;
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
        for (std::deque<int>::const_iterator i = playfield_.snake.begin(); i != playfield_.snake.end(); ++i){
            if (a == *i) {
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


void Snake::move(direction d, std::deque<int> & snake) {
    switch (d) {
        case UP:
            snake.push_front(snake.front() - playfield_.height);
            snake.pop_back();
            break; 
        case DOWN:
            snake.push_front(snake.front() + playfield_.height);
            snake.pop_back();
            break;
        case LEFT:
            snake.push_front(snake.front() - 1);
            snake.pop_back();
            break;
        case RIGHT:
            snake.push_front(snake.front() + 1);
            snake.pop_back();
            break;
    };
}


bool Snake::checkMove(direction d, const std::deque<int> & snake, bool head) {
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
    for (std::deque<int>::const_iterator i = snake.begin(); i != snake.end(); ++i){
        if (newHead == *i) {
            return false;
        }
    }
    return true;
}
std::deque<int> Snake::getBoard() {
    std::deque<int> sorted =  playfield_.snake;
    std::deque<int> result (playfield_.height * playfield_.width, 0);
    sort::insertionSort(sorted);
    // std::cout << "Snake size " << sorted.size() << std::endl;
    int index = 0;
    int position;
    bool stop = false;
    for (int i = 0; i < playfield_.height; ++i){
        for (int k = 0; k < playfield_.width; ++k) {
            position = i * playfield_.width + k;
            if (sorted.at(index) == position) {
                //std::cout << "Snake pos " << sorted.at(index) << " " << position << std::endl;
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
//    if (size  != ((playfield_.height * playfield_.width) - 1)) {
//        return;
//    }
//    std::deque<int> sorted =  playfield_.snake;
//    sort::insertionSort(sorted);
//    int index = 0;
//    int position;
//    for (int i = 0; i < playfield_.height; ++i){
//        for (int k = 0; k < playfield_.width; ++k) {
//            position = i * playfield_.width + k;
//            if (sorted.at(index) == position) {
//                bitWrite(bytes[i], k, 1);
//            } else if (playfield_.food == position){
//                bitWrite(bytes[i], k, 1);
//            } else {
//                bitWrite(bytes[i], k, 0);
//            }
//        }
//    }
//}

void Snake::nextFrame(direction d) {
    //std::cout << "calling next frame";
    move(d, playfield_.snake);
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
        std::cout << "index: " << i << " direction " << ds[i] << std::endl;
        switch (ds[i]){
            case UP:
                if (checkMove(UP, playfield_.snake, false)){
                    playfield_.snake.push_back(playfield_.snake.back() - playfield_.height);
                    end = true;
                    break;
                }
            case DOWN:
                if (checkMove(DOWN, playfield_.snake, false)){
                    playfield_.snake.push_back(playfield_.snake.back() + playfield_.height);
                    end = true;
                    break;
                }
            case LEFT: 
                if (checkMove(LEFT, playfield_.snake, false)){
                    playfield_.snake.push_back(playfield_.snake.back() - 1);
                    end = true;
                    break;
                }
            case RIGHT:
                if (checkMove(RIGHT, playfield_.snake, false)){
                    playfield_.snake.push_back(playfield_.snake.back() + 1);
                    end = true;
                    break;
                }
        }
        if (end) {
            break;
        }
    }    
    return end;
}


bool Snake::findPath(direction * dA) {
    direction path [playfield_.height * playfield_.width];
        
}


int Snake::euclideanDistance(int point) {
    // convert to x, y coordinates
    int x, y;
    int x2, y2;
    convert(point, x, y);
    convert(playfield_.food, x2, y2);
    
    // https://en.wikipedia.org/wiki/Euclidean_distance
    return sqrt(pow(x - x2, 2) + pow(y -y2, 2));
}

void Snake::convert(int point, int & x, int & y) {
    x = (point % playfield_.width) + 1;
    y = int (point / playfield_.height) ;
    return;
}