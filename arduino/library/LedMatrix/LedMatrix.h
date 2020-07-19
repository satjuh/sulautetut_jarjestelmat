#ifndef LedMatrix_h
#define LedMatrix_h


#include "Arduino.h"

// Made for 8x8 led matrix
class LedMatrix{
    public:
        // Pins for rows and cols
        LedMatrix(byte rows [], byte cols[], int drawingDelay= 200);
        // Shapes
        void draw();
        // Set the shape that is drawn when draw() is called
        void setShape(char shape);
        // Draws a word one character at a time.
        void drawWord(char * word, int length, int timeBetween);
        // Draws an animation to the led matfix
        void drawAnimation(int number, int delay = 200);
        // Clear the shape bytes 
        void clear();
        // Set delay used in turning the led on/off 
        void setDelay(int delay);
    private:
        // Used to acquire the shapes that are drawn
        void getShape(char shape, byte* shape_bytes);
        // A simple lighting effect moving based on row
        void rowBasedAnimation();
        // A simple lighting effect moving based on column
        void columnBasedAnimation(); 
        // Animation that resembles rain
        void rainAnimation();
        // Move a row of leds back and forth
        void backAndForthAnimation();
        // Move a bit from byte to the next
        void bitMove();
        // Shift bits in the shape_ array
        void bitShift();
        // previously drawn shape
        char oldShape_;
        // How much delay until the led is turned off
        int drawingDelay_;
        // Shape that is drawn
        byte shape_ [8];
        // Pins that are responsible for the rows of the led matrix
        byte * cols_;
        // Pins that are responsible for the colums of the led matrix
        byte * rows_;
};

#endif
