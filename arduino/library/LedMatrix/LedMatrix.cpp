#include "Arduino.h"
#include "LedMatrix.h"


LedMatrix::LedMatrix (byte rows [], byte cols [], int drawingDelay){
    for(byte i = 0; i < 8; ++i) {
        pinMode(rows[i], OUTPUT);        
        pinMode(cols[i], OUTPUT);        
    }
    cols_ = cols;
    rows_ = rows;
    drawingDelay_ = drawingDelay;
}


void LedMatrix::draw(){
    for (byte i = 0; i < 8; ++i) {
        digitalWrite(rows_[i], 1);
        for (byte j = 0; j < 8; ++j) {
            digitalWrite(cols_[j], (~shape_[i] >> j) & 0x01 );
            delayMicroseconds(drawingDelay_);
            digitalWrite(cols_[j], 1);
        }
        digitalWrite(rows_[i], 0);
    }
    return;
}


void LedMatrix::draw(char * word, int length, int timeBetween){
    for (byte i = 0; i < length; ++i) {
        setShape(word[i]);
        for (byte k = 0; k < timeBetween; ++k) {
            draw();
        }
    }
}
        

void LedMatrix::getShape(char shape, byte* shape_bytes){
    switch (shape) {
        case '+':
            shape_bytes[0] = B00011000;
            shape_bytes[1] = B00011000;
            shape_bytes[2] = B00011000;
            shape_bytes[3] = B11111111;
            shape_bytes[4] = B11111111;
            shape_bytes[5] = B00011000;
            shape_bytes[6] = B00011000;
            shape_bytes[7] = B00011000;
            return;
        case '-':
            shape_bytes[0] = B00000000;
            shape_bytes[1] = B00000000;
            shape_bytes[2] = B00000000;
            shape_bytes[3] = B11111111;
            shape_bytes[4] = B11111111;
            shape_bytes[5] = B00000000;
            shape_bytes[6] = B00000000;
            shape_bytes[7] = B00000000;
            return;
        case 's':
            shape_bytes[0] = B11111111;
            shape_bytes[1] = B11111111;
            shape_bytes[2] = B11000000;
            shape_bytes[3] = B11111111;
            shape_bytes[4] = B11111111;
            shape_bytes[5] = B00000011;
            shape_bytes[6] = B11111111;
            shape_bytes[7] = B11111111;
            return;
        case 'u':
            shape_bytes[0] = B11000011;
            shape_bytes[1] = B11000011;
            shape_bytes[2] = B11000011;
            shape_bytes[3] = B11000011;
            shape_bytes[4] = B11000011;
            shape_bytes[5] = B11000011;
            shape_bytes[6] = B11111111;
            shape_bytes[7] = B11111111;
            return;
        case 'l':
            shape_bytes[0] = B11000000;
            shape_bytes[1] = B11000000;
            shape_bytes[2] = B11000000;
            shape_bytes[3] = B11000000;
            shape_bytes[4] = B11000000;
            shape_bytes[5] = B11000000;
            shape_bytes[6] = B11111111;
            shape_bytes[7] = B11111111;
            return;
        case 'a':
            shape_bytes[0] = B00011000;
            shape_bytes[1] = B00111100;
            shape_bytes[2] = B01100110;
            shape_bytes[3] = B11000011;
            shape_bytes[4] = B11111111;
            shape_bytes[5] = B11111111;
            shape_bytes[6] = B11000111;
            shape_bytes[7] = B11000111;
            return;
        case 'r':
            shape_bytes[0] = B11111110;
            shape_bytes[1] = B11111111;
            shape_bytes[2] = B11000011;
            shape_bytes[3] = B11111111;
            shape_bytes[4] = B11111110;
            shape_bytes[5] = B11001100;
            shape_bytes[6] = B11000110;
            shape_bytes[7] = B11000011;
            return;
        case 'i':
            shape_bytes[0] = B00111100;
            shape_bytes[1] = B00111100;
            shape_bytes[2] = B00011000;
            shape_bytes[3] = B00011000;
            shape_bytes[4] = B00011000;
            shape_bytes[5] = B00011000;
            shape_bytes[6] = B00111100;
            shape_bytes[7] = B00111100;
            return;
        default:
            shape_bytes[0] = B00000000;
            shape_bytes[1] = B00000000;
            shape_bytes[2] = B00000000;
            shape_bytes[3] = B00000000;
            shape_bytes[4] = B00000000;
            shape_bytes[5] = B00000000;
            shape_bytes[6] = B00000000;
            shape_bytes[7] = B00000000;
            return;

    }
}

void LedMatrix::setShape(char shape){
    if (shape == oldShape_) {
        return;
    } else {
        getShape(shape, shape_);
        oldShape_ = shape;
    }
}


void LedMatrix::setShape(byte leds [8]){
    oldShape_ = ' ';
    shape_[0] = leds[0];
    shape_[1] = leds[1];
    shape_[2] = leds[2];
    shape_[3] = leds[3];
    shape_[4] = leds[4];
    shape_[5] = leds[5];
    shape_[6] = leds[6];
    shape_[7] = leds[7];
    return;
}


void LedMatrix::drawAnimation(int number, int delay) {
    switch(number){
        case 1:
            rowBasedAnimation();
            break;
        case 2:
            columnBasedAnimation();
            break;
        case 3:
            rainAnimation();
            break;
    }
    return;
}


void LedMatrix::rowBasedAnimation(){
    clear();
    for (int k = 0; k < 8; ++k) {
        shape_[k] = B00000001;
        for (int i = 0; i < 8;  ++i){
            draw();
            bitShift();
        }
        draw();
        bitMove();
    }
}

void LedMatrix::columnBasedAnimation() {
    clear();
    for (int k = 0; k < 8; ++k) {
        shape_[0] = B00000001 << k;
        for (int i = 0; i < 8;  ++i){
            draw();
            bitMove();
        }
    }

}


void LedMatrix::rainAnimation() {  
    shape_[0] = random(0,255);
    for (int i = 0; i < 8; ++i){
        draw();
    }
    bitMove();
}


void LedMatrix::clear(){
    shape_[0] = B00000000;
    shape_[1] = B00000000;
    shape_[2] = B00000000;
    shape_[3] = B00000000;
    shape_[4] = B00000000;
    shape_[5] = B00000000;
    shape_[6] = B00000000;
    shape_[7] = B00000000;
}


void LedMatrix::setDelay(int delay){
    if (delay == drawingDelay_){
        return;
    } else {
        drawingDelay_ = delay;
    }
}


void LedMatrix::bitMove(){
    for (byte i = 8; i--> 0;) {
        for (byte a = 0; a < 8; ++a){
            if (i != 7 ) {
                if (bitRead(shape_[i], a) == 1){
                    bitWrite(shape_[i+1], a, 1);
                    bitWrite(shape_[i], a, 0);
                } 
            } else if (i ==7){
                bitWrite(shape_[i], a, 0);
            }
        }
    }
}


void LedMatrix::bitShift(){
    for (int i = 0; i < 8; ++i){
        byte result = shape_[i] << 1;
        shape_[i] = result;
    }
}
