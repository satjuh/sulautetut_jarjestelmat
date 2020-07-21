#include <Arduino.h>
#include <ArduinoSTL.h>
#include <deque>

#include "Snake.h"

Snake::Snake(int height, int width, int size){
    // playfield width height
    playfield_.height = height;
    playfield_.width = width;
    // Initialize playfield and snake
    playfield_.field = new int [height*width] {0};
    length_ = size;
    playfield_.food = 0;
    ongoing_ = false;
}

Snake::~Snake(){
    delete playfield_.field;
}


bool Snake::startGame(){
    if (!ongoing_){
        return; // don't start game if it is still going
    }
    int a, b = 0;
    // Put food in a random position
    while (a == b) {
        a = random(0, playfield_.height * playfield_.width-1);
        //b = random(0, height*width-1);
        b = 2 * playfield_.height+ 3;
    }
    // Add food to playfield
    playfield_.field[a] = 2;
    playfield_.food = a;
    // Add snake body to the left side of the snake
    for (int i = 0; i < length_; ++i){
        if (i == 0){
            playfield_.snake[i] = b;
        } else {
            playfield_.snake[i] = b - 1;
        }
    }
    playfield_.field[b] = 1;

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
        playfield_.snake.push_back(playfield_.snake.back() + playfield_.height)
    }
}


void Snake::addFood(){
    while (true) {
        int a = random(0, playfield_.height * playfield_.width);
        if (playfield_.field[a] != 1){
            playfield_.field[a] = 2;
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