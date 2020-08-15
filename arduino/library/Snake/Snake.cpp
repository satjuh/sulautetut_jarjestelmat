#include <cstdlib>
#include <iostream>
#include <ctime>

#include <math.h>
#include <unistd.h>

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
    // Add snake body to the left side of the snake
    for (int i = 0; i < length_; ++i){
        playfield_.snake.push_back(b - i);
    }
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
ownArray<int> Snake::getBoard() {
    ownArray<int> sorted =  playfield_.snake;
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

void Snake::nextFrame(void (*print)(const ownArray<int>& arr)) {
    ownArray<direction> dirs = findPath();
    bool restart = false;
    // Couldn't find a path so lets start again.
    if (dirs.size() == 0){
        restart = true;
    }

    for (unsigned int i = 0; i < dirs.size(); ++i){
        if (!(checkMove(UP, playfield_.snake) || checkMove(DOWN, playfield_.snake) 
            || checkMove(LEFT, playfield_.snake) || checkMove(RIGHT, playfield_.snake))){
                restart=true;
                break;
            }
        if (checkMove(dirs[i], playfield_.snake)){
            ownArray<int> board = getBoard();
            print(board);
            move(dirs[i], playfield_.snake);
        }
    }
    if (restart){
        print(ownArray<int>(64,1));
        ongoing_ = false;
        startGame();
    }
    /*
    if (checkMove(d,playfield_.snake)){
        move(d, playfield_.snake);
    }
    */
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
    ownArray<int> tempSnake = playfield_.snake;
    ownArray<int> visited;
    depthFirst(result, LAST, visited, tempSnake);
    return result;
    /*
    ownArray<int> visited; 
    // A container parent child relations. Used similarly to std::map where
    // index = 1 means position 1 on the playfield. 
    ownArray<ParChild> map (64, ParChild());
    
    ParChild current (playfield_.snake.front(), -1, LAST);
    priorityQueue<ParChild> posDirections;
    bool notFound = true;

    // Push first position to visited
    visited.push_back(playfield_.snake.front());

    int counter = 0;
    while (notFound){
        ++counter;
        if (counter > playfield_.width * playfield_.height){
            break;
        }
        if (current.value == playfield_.food){
            break;
        }
        // Add possible moves from current position
        if (checkMove(LEFT, playfield_.snake) && !checkIfValueInContainer(visited, current.value - 1)) {
            ParChild t (current.value - 1, current.value, LEFT);
            posDirections.push(t, euclideanDistance(t.value));
            visited.push_back(t.value);
            map[t.value] = t;
        }
        if (checkMove(RIGHT, playfield_.snake) && !checkIfValueInContainer(visited, current.value + 1)){
            ParChild t (current.value + 1, current.value, RIGHT);
            posDirections.push(t, euclideanDistance(t.value));
            visited.push_back(t.value);
            map[t.value] = t;
        }
        if (checkMove(DOWN, playfield_.snake) && !checkIfValueInContainer(visited, current.value + playfield_.width)){
            ParChild t (current.value + playfield_.width, current.value, DOWN);
            posDirections.push(t, euclideanDistance(t.value));
            visited.push_back(t.value);
            map[t.value] = t;
        }
        if (checkMove(UP, playfield_.snake) && !checkIfValueInContainer(visited, current.value - playfield_.width)){
            ParChild t (current.value - playfield_.width, current.value, UP);
            posDirections.push(t, euclideanDistance(t.value));
            visited.push_back(t.value);
            map[t.value] = t;
        }

        current = posDirections.pop();
        

    }

    ownArray<direction> result;
    int index = current.value;

    while(index != -1){
        std::cout << "Index==" << index << std::endl;
        result.push_back(map[index].d);
        index = map[index].parent;
    }

    return result;
    */
}

bool Snake::depthFirst(ownArray<direction> & directions, direction current, ownArray<int>& visited, ownArray<int>& snake){
    ownArray<int> board = getBoard(snake);
    if (current != LAST) {
        directions.push_back(current);
        move(current, snake);
    }
    if (snake.front() == playfield_.food){
        return true;
    } else {
        visited.push_back(snake.front());
        bool found = false;
        if (checkMove(LEFT,snake) && !checkIfValueInContainer(visited, snake.front() -1)){
            found = depthFirst(directions, LEFT, visited, snake);
            if (!found){
                move(RIGHT, snake, false);
                directions.pop_back();
            }
        }
        if (checkMove(RIGHT, snake) && !found && !checkIfValueInContainer(visited, snake.front() +1 )){
            found = depthFirst(directions, RIGHT, visited, snake);
            if (!found){
                move(LEFT, snake, false);
                directions.pop_back();
            }
        }
        if (checkMove(UP, snake) && !found && !checkIfValueInContainer(visited, snake.front() - playfield_.width)){
            found = depthFirst(directions, UP, visited, snake);
            if (!found){
                move(DOWN, snake, false);
                directions.pop_back();
            }
        }
        if (checkMove(DOWN, snake) && !found && !checkIfValueInContainer(visited, snake.front() + playfield_.width)){
            found = depthFirst(directions, DOWN, visited, snake);
            if (!found){
                move(UP, snake, false);
                directions.pop_back();
            }
        }

        if (found){
            return true;
        }

    }
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
