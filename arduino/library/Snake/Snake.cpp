#include <deque>
#include <cstdlib>
#include <iostream>
#include <ctime>

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
        std::cout << "Generating random number ";
        a = random(0, playfield_.height * playfield_.width-1);
        //a = random(0, playfield_.height * playfield_.width);
        //b = random(0, height*width-1);
        b = (2 * playfield_.height)+ 3;a:
        std::cout << a << " b:" << b  << std::endl;
    }
    // Add food to playfield
    playfield_.food = a;
    // Add snake body to the left side of the snake
    for (int i = 0; i < length_; ++i){
        std::cout << "Snake " << i << "=" << b - i << std::endl;
        playfield_.snake.push_back(b - i);
    }
    return true;

}


void Snake::grow(){
    ++ length_;
    if (checkMove(LEFT, playfield_.snake, false)){
        playfield_.snake.push_back(playfield_.snake.back() - 1);
    } else if (checkMove(RIGHT, playfield_.snake, false)){
        playfield_.snake.push_back(playfield_.snake.back() + 1);
    } else if (checkMove(UP, playfield_.snake, false)){
        playfield_.snake.push_back(playfield_.snake.back() - playfield_.height);
    } else if (checkMove(DOWN, playfield_.snake, false)){
        playfield_.snake.push_back(playfield_.snake.back() + playfield_.height);
    }
}


void Snake::addFood(){
    int a = 0;
    bool found = false;
    while (true) {
        a = random(0, playfield_.height * playfield_.width - 1);
        //a = rand() % playfield_.height * playfield_.width;
        found = false;
        for (std::deque<int>::const_iterator i = playfield_.snake.begin(); i != playfield_.snake.end(); ++i){
            if (a == *i) {
                found = true;
                break;
            }
        }
        if (not found) {
            playfield_.food = a;
            break;
        }
    }

    return;
}


void Snake::move(direction d, std::deque<int> & snake) {
    switch (d) {
        case UP:
            std::cout << "UP";
            snake.push_front(snake.front() - playfield_.height);
            snake.pop_back();
            std::cout << "\n" << snake[0] << " " << snake [1] << " " << snake [2] << std::endl;;
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
            if (begin > playfield_.height -1){
                newHead = begin - playfield_.height;
            } else {
                return false;
            }
            break; 
        case DOWN:
            if (begin < ((playfield_.height - 1) * playfield_.width)){
                newHead = begin + playfield_.height;
            } else {
                return false;
            }
            break;

        case LEFT:
            if (begin & playfield_.width != 0) {
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
    std::deque<int> result (playfield_.height * playfield_.width - 1, 0);
    sort::insertionSort(sorted);
    int index = 0;
    int position;
    bool stop = false;
    for (int i = 0; i < playfield_.height; ++i){
        for (int k = 0; k < playfield_.width; ++k) {
            position = i * playfield_.width + k;
            if (sorted.at(index) == position) {
                result[position] = 1;
                ++ index;
            } else if (playfield_.food == position){
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
    std::cout << "calling next frame";
    move(d, playfield_.snake);
}

Playfield Snake::getPlayfield() {
    return playfield_;
}

