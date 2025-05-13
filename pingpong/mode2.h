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
int extern bytecode;
int extern hit;
int extern next_target_list[];
int extern target_list[];
bool extern in_next;
bool extern after_finish;
int extern stage;
int hist_score2=0;

void game2over()
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
    in_next = false;
}

void get_next_target_list()
{ //""""""""""NEW"""""""""""""
    int seed1 = analogRead(6);
    seed1 = seed1 % 5 + 1;
    switch (seed1)
    {
    case 1:
        next_target_list[0] = 2;
        next_target_list[1] = 5;
        if (seed1 != 2 && seed1 != 5)
            next_target_list[2] = seed1;
        else
            next_target_list[2] = (seed1 + 2) % 6;
        break;
    case 2:
        next_target_list[0] = 1;
        if (seed1 % 2 == 1)
        {
            next_target_list[1] = 4;
            next_target_list[2] = 5;
        }
        else
        {
            next_target_list[1] = 2;
            next_target_list[2] = 4;
        }
        break;
    case 3:
        next_target_list[2] = 6;
        if (seed1 % 2 == 1)
        {
            next_target_list[0] = 3;
            next_target_list[1] = 5;
        }
        else
        {
            next_target_list[0] = 2;
            next_target_list[1] = 3;
        }
        break;
    case 4:
        next_target_list[0] = 1;
        next_target_list[2] = 6;
        if (seed1 % 2 == 1)
            next_target_list[1] = 2;
        else
            next_target_list[1] = 5;
        break;
    case 5:
        next_target_list[0] = 1;
        next_target_list[2] = 6;
        if (seed1 % 2 == 1)
            next_target_list[1] = 2;
        else
            next_target_list[1] = 5;
        break;
    }
    int m[3] = {6, 0, 0};
    for (int i = 0; i < 3; i++)
    {
        if (next_target_list[i] < m[0])
            m[0] = next_target_list[i];
        if (next_target_list[i] > m[2])
            m[2] = next_target_list[i];
    }
    for (int i = 0; i < 3; i++)
    {
        if (next_target_list[i] != m[0] && next_target_list[i] != m[2])
        {
            m[1] = next_target_list[i];
            break;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        next_target_list[i] = m[i];
    }
}
void update_bytecode(int hit)
{
    int near = 1;
    for (int i = 0; i < 3; i++)
    {
        if (next_target_list[i] != 0)
        {
            if (next_target_list[i] + 1 == hit)
            {
                near = 0;
            }
            else if (next_target_list[i] - 1 == hit)
            {
                near = 2;
            }
        }
    }
    if (near == 0)
    {
        if (hit < 4)
        {
            int minus = 1;
            for (int i = 0; i < hit; i++)
            {
                minus *= 2;
            }
            bytecode -= minus;
        }
        else
        {
            int minus = 1;
            for (int i = 0; i < hit + 1; i++)
            {
                minus *= 2;
            }
            bytecode -= minus;
        }
    }
    else if (near == 1)
    {
        int hitgrid[3] = {0};
        hitgrid[0] = hit;
        bytecode -= pairLED(hitgrid);
    }
    else
    {
        if (hit < 4)
        {
            int minus = 1;
            for (int i = 0; i < hit - 1; i++)
            {
                minus *= 2;
            }
            bytecode -= minus;
        }
        else
        {
            int minus = 1;
            for (int i = 0; i < hit; i++)
            {
                minus *= 2;
            }
            bytecode -= minus;
        }
    }
}

void mode2()
{
    counter();
    //settime();
    if (hit == 0)
    {
        //substitude this by iezoelectric material function
        hit=is_hit(0,mode);
    }
    else
    {
        if (stage == 1)
        {
            
            for (int i = 0; i < 3; i++)
            {
                if (hit == next_target_list[i])
                {
                    in_next = true;
                    next_target_list[i] = 0;
                    break;
                }
            }
        }
        if (!after_finish && in_next)
        {
            after_hit(bytecode, hit, mode);
        }
        else if (in_next)
        {
            
            set_score(mode, hit_score);
            score_display(hist_score2);
            in_next = false;
            update_bytecode(hit);
            shiftdata(bytecode);
            after_finish = false;
            hit = 0;
            if (bytecode == 0)
            {
                tone(buzPin, 600, 100);
                tone(buzPin, 400, 100);
                set_score(mode, 2 * hit_score);
                score_display(hist_score2);
                get_next_target_list();
                bytecode = pairLED(next_target_list);
                shiftdata(bytecode);
                for (int i = 0; i < 3; i++)
                {
                    target_list[i] = next_target_list[i];
                }
                hit = 0;
                after_finish = false;
            }
        }
        else if (!in_next)
        {
            //tone(buzPin, 60, 100);
            hit = 0;
        }
    }
    if (left_time <= 0)
    {
        game2over();
    }
}
