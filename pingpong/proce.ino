#include<SoftwareSerial.h>
SoftwareSerial mySerial(10,13);
//Pin connected to ST_CP of 74HC595
#define latchPin 8
//Pin connected to SH_CP of 74HC595
#define clockPin 12
//Pin connected to DS of 74HC595
#define dataPin 11
//Pin connected to buzzer
#define buzPin 3

#define button 2

#include "mode1.h"
#include "mode2.h"
#include "mode3.h"

int button_state = 0;
//compare previuous and now stete to count
int prev_button = 0; //""""""""""NEW"""""""""""""

//player's score and historistic high score
int now_score = 0;
int hit_score = 5;

//whether the correct grid is hit
int hit = 0; //""""""""UPDATE"""""""""

//which grid should player hit,replace "target" and "next_target"
//""""""""""NEW"""""""""""""
int target_list[3];
int next_target_list[3];
bool in_next = false;

//data send to 74hc595
int bytecode;

//use t as timer
unsigned long t = 0;
unsigned long cht = 0;
//timer that control after_hit
unsigned long after_t = 0; //""""""""""NEW"""""""""""""
int stage = 1;
bool after_finish = true;

int left_time;

//timer that control mode
unsigned long whole_t = 0; //""""""""""NEW"""""""""""""
//play mode
int mode; //""""""""""NEW"""""""""""""

//score
int extern hist_score1;
int extern hist_score2;
int extern hist_score3;


void setup()
{
    //set pins to output so you can control the shift register
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(buzPin, OUTPUT);
    pinMode(button, INPUT_PULLUP);
    pinMode(sensor1, INPUT);
    pinMode(sensor2, INPUT);
    pinMode(sensor3, INPUT);
    pinMode(sensor4, INPUT);
    pinMode(sensor5, INPUT);
    pinMode(sensor6, INPUT);
    Serial.begin(9600);
    mySerial.begin(9600);
   
   
    now_score = 0;
    startspec();
    time_setup();
    score_setup();
    button_state = digitalRead(button);
    stage = 1;
    after_finish = false;
    after_t = millis();
    mode = 0;           //""""""""""NEW"""""""""""""
    whole_t = millis(); //""""""""""NEW"""""""""""""
    in_next = false;
    left_time=-1;
}
 
void loop()
{

  

      
     
//////////////////bluetooth select//////////////////////
      settime();
      if(mode==0)
      {
    
         if(mySerial.available())
      
         {
      
            char receive=mySerial.read();
            Serial.println(receive);
            if(receive=='s')
            {
              play_first_mode();
            }
            else if(receive=='m')
            {
              play_second_mode();
            }
            else if(receive=='r')
            {
              play_third_mode();
            }
         }  
      }
      if (mode == 10)
      {
          mode1();
      }
      else if (mode == 20)
      {
          mode2();
      }
      else if (mode == 30)
      {
          mode3();
      }

////////////////button select////////////////////
//    settime();
//    if (mode < 10)
//    {
//
//        //give player 5s to decide
//        if (millis() > whole_t + 3000)
//        {
//            Serial.println("select time up!!");
//            whole_t = millis();
//            if (mode == 1)
//            {
//                play_first_mode();
//            }
//            else if (mode == 2)
//            {
//                play_second_mode();
//            }
//            else if (mode == 3)
//            {
//                play_third_mode();
//            }
//            else
//            {
//                play_mode_reset();
//            }
//        }
//        else
//        {
//            button_state = digitalRead(button);
//            if (button_state == LOW && prev_button == HIGH)
//            {
//                Serial.println("press once button!");
//                mode++;
//                tone(buzPin, 100, 100);
//            }
//            prev_button = button_state;
//        }
//    }
//    else if (mode == 10)
//    {
//        mode1();
//    }
//    else if (mode == 20)
//    {
//        mode2();
//    }
//    else if (mode == 30)
//    {
//        mode3();
//    }
//
}

//special effect when the power is on
void startspec()
{
    for (int i = 1; i < 256; i *= 2)
    {
        tone(buzPin, 150 + 80 * sqrt(i));
        shiftdata_delay(i, 500);
        
    }
    tone(buzPin, 600, 300);
    shiftdata_delay(255, 300);
    tone(buzPin, 150, 300);
    shiftdata_delay(0, 100);
    tone(buzPin, 600, 300);
    shiftdata_delay(255, 200);
    tone(buzPin, 150, 300);
    shiftdata_delay(0, 500);
    //first one
}

void play_first_mode()
{ //""""""""""NEW"""""""""""""
    Serial.println("play first mode!");
    mode = 10;
    next_target_list[0] = 2;
    next_target_list[1] = 0;
    next_target_list[2] = 0;
    bytecode = pairLED(next_target_list);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, bytecode);
    digitalWrite(latchPin, HIGH);
    tone(buzPin, 659, 500);
    delay(1000);
    tone(buzPin, 659, 500);
    delay(1000);
    tone(buzPin, 659, 500);
    delay(1000);
    for (int i = 0; i < 3; i++)
    {
        target_list[i] = next_target_list[i];
    }
    left_time=20;
    now_score=0;
    score_display(hist_score1);
}

void play_second_mode()
{ //""""""""""NEW"""""""""""""
    Serial.println("play second mode!");
    mode = 20;
    next_target_list[0] = 1;
    next_target_list[1] = 5;
    next_target_list[2] = 6;
    bytecode = pairLED(next_target_list);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, bytecode);
    digitalWrite(latchPin, HIGH);
    tone(buzPin, 659, 500);
    delay(1000);
    tone(buzPin, 659, 500);
    delay(1000);
    tone(buzPin, 659, 500);
    delay(1000);
    for (int i = 0; i < 3; i++)
    {
        target_list[i] = next_target_list[i];
    }
    left_time=20;
    now_score=0;
    score_display(hist_score2);
}
void play_third_mode()
{ //""""""""""NEW"""""""""""""
    Serial.println("play third mode!");
    mode = 30;
    next_target_list[0] = 5;
    next_target_list[1] = 0;
    next_target_list[2] = 0;
    bytecode = pairLED(next_target_list);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, bytecode);
    digitalWrite(latchPin, HIGH);
    tone(buzPin, 659, 500);
    delay(1000);
    tone(buzPin, 659, 500);
    delay(1000);
    tone(buzPin, 659, 500);
    delay(1000);
    for (int i = 0; i < 3; i++)
    {
        target_list[i] = next_target_list[i];
    }
    left_time=20;
    now_score=0;
    score_display(hist_score3);
}
void play_mode_reset()
{ //""""""""""NEW"""""""""""""
    tone(buzPin, 440, 100);
    delay(150);
    tone(buzPin, 440, 100);
    delay(150);

    mode = 0;
    Serial.println("select again");
}
