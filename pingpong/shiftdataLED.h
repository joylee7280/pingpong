#include <SoftwareSerial.h>
//Pin connected to ST_CP of 74HC595
#define latchPin 8
//Pin connected to SH_CP of 74HC595
#define clockPin 12
//Pin connected to DS of 74HC595
#define dataPin 11
//Pin connected to buzzer
#define buzPin 3

int extern stage;
unsigned long int extern after_t;
bool extern after_finish;
int *grid_to_LED(int grid[])
{
    static int output[6] = {0};
    if (grid[2] == 0)
    { //deal with first mode, no change
        if (grid[0] < 4)
        {
            output[0] = grid[0];
            output[1] = grid[0] + 1;
        }
        else
        {
            output[0] = grid[0] + 1;
            output[1] = grid[0] + 2;
        }
    }
    else
    { //deal with second mode, output should more than 2 digits
        for (int i = 0; i < 3; i++)
        {
            if (grid[i] < 4)
            {
                output[2 * i] = grid[i];
                output[2 * i + 1] = grid[i] + 1;
            }
            else
            {
                output[2 * i] = grid[i] + 1;
                output[2 * i + 1] = grid[i] + 2;
            }
        }
        if (output[1] == output[2])
            output[1] = 0;
        if (output[3] == output[4])
            output[3] = 0;
    }
    return output;
}
//encode LED in pairs
int pairLED(int grid[])
{
    int *paircode = grid_to_LED(grid);
    int code = 0; //""""""""""UPDATE"""""""""""""
    if (grid[2] == 0)
    { //deal with first mode, no change
        int first = 1;
        for (int i = 0; i < (*paircode) - 1; i++)
        {
            first *= 2;
        }
        int sec = 1;
        for (int i = 0; i < (*(paircode + 1)) - 1; i++)
        {
            sec *= 2;
        }
        code += first + sec;
        return code;
    }
    else
    { //deal with second mode, paircode has more than 2 digits
        for (int i = 0; i < 6; i++)
        {
            if (*(paircode + i) != 0)
            {
                int add = 1;
                for (int j = 0; j < *(paircode + i) - 1; j++)
                {
                    add *= 2;
                }
                code += add;
            }
        }
        return code;
    }
}

// send data to 74hc595 without delay
void shiftdata(int numberToDisplay)
{
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
    digitalWrite(latchPin, HIGH);
}
//send data to 74hc595 to light LED by delay()
void shiftdata_delay(int numberToDisplay, int delaytime)
{
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
    digitalWrite(latchPin, HIGH);
    delay(delaytime);
}
//shining the LED playing music after hit"""""""""""UPDATE"""""""""""""
void after_hit(int bytecode, int hit, int mode)
{
    if (mode == 10|| mode==30 )
    {
        if (stage % 2 == 1)
        {
            shiftdata(0);
            analogWrite(buzPin, 70);
            if (millis() > after_t + 200)
            {
                stage += 1;
                after_t = millis();
            }
        }
        else if (stage % 2 == 0)
        {
            shiftdata(bytecode);
            analogWrite(buzPin, 220);
            if (millis() > after_t + 200)
            {
                stage += 1;
                after_t = millis();
            }
            if (stage == 5)
            {
                stage = 1;
                after_finish = true;
                shiftdata(0);
                analogWrite(buzPin, 0);
            }
        }
    }
    else if (mode == 20)
    {
        int secbytecode = bytecode;
        int hitgrid[3] = {0};
        hitgrid[0] = hit;
        secbytecode -= pairLED(hitgrid);
        if (stage % 2 == 1)
        {
            shiftdata(secbytecode);
            analogWrite(buzPin, 70);
            if (millis() > after_t + 200)
            {
                stage += 1;
                after_t = millis();
            }
        }
        else if (stage % 2 == 0)
        {
            shiftdata(bytecode);
            analogWrite(buzPin, 220);
            if (millis() > after_t + 200)
            {
                stage += 1;
                after_t = millis();
            }
            if (stage == 5)
            {
                analogWrite(buzPin, 0);
                stage = 1;
                after_finish = true;
            }
        }
    }
}
