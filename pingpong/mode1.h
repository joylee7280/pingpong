#ifndef shift
#define shift
#include "shiftdataLED.h"
#endif
#ifndef timer
#define timer
#include "seventime.h"
#endif
#ifndef updatescore
#define updatescore
#include "sevenscore.h"
#endif
#ifndef hitsensor
#define hitsensor
#include "piezoelectric_sensor.h"
#endif
#ifndef pitches
#define pitches
#include"pitches.h"
#endif
#define buzPin 3

int extern mode;
int extern hit;
bool extern after_finish;
int extern bytecode;
int extern now_score;
int extern hit_score;
int extern next_target_list[];
int extern target_list[];
int extern left_time;
int extern stage;
int hist_score1;
void game1over()
{
    tm1637.display(0, 0); //設定每一位燈號顯示的內容，參數1：燈號，參數2：顯示的數字
    tm1637.display(1, 0);
    tm1637.display(2, 0);
    tm1637.display(3, 0);
   
    for(int thisNote=0; thisNote<8;thisNote++){
    int noteDuration=1000/noteDurations[thisNote];
    tone(3,melody[thisNote],noteDuration);
    int pauseBetweenNotes=noteDuration*1.30;
    delay(pauseBetweenNotes);
    noTone(3);
    }
    shiftdata(0);
    mode = 0;
    stage = 1;
    after_finish = false;
    hit = 0;
    left_time = -1;
    now_score = 0;
    //Serial.println(mode);
}

void mode1()
{
    counter();
    //settime();
    if (!hit)
    {
        //substitude this by iezoelectric material function
        hit=is_hit(next_target_list[0],mode);
    }
    else
    {
        if (!after_finish) //""""""UPDATE"""""""""
        {
            after_hit(bytecode, hit, mode);
        }
        else
        {
            set_score(mode, hit_score);
            score_display(hist_score1);
            do
            {
                unsigned long int seed; //""""""""NEW"""""""""""from blob become local
                seed = millis();
                randomSeed(seed);
                next_target_list[0] = random(1, 7);          //""""""""""NEW"""""""""""""
            } while (next_target_list[0] == target_list[0]); //""""""""""NEW"""""""""""""
            bytecode = pairLED(next_target_list);            //""""""""""NEW"""""""""""""
            shiftdata(bytecode);
            target_list[0] = next_target_list[0]; //""""""""""NEW"""""""""""""
            Serial.print("This is new target grid:");
            Serial.println(target_list[0]); //""""""""""NEW"""""""""""""
            hit = false;
            after_finish = false;
        }
    }
    //put gameover here because only first mode end with time""""""""""NEW"""""""""""""
    if (left_time <= 0)
    {
        game1over();
        //remember to reset all variable
    }
}
