#include <SoftwareSerial.h>
#include "TM1637.h"
#define CLK2 4 //CLK pin，可自行決定接到哪個PIN
#define DIO2 5 //DIO pin，可自行決定接到哪個PIN
TM1637 tm16372(CLK2, DIO2);
int point_flag2 = 0;
int extern now_score;
int extern hist_score1;
int extern hist_score2;
int extern hist_score3;

void score_setup()
{
    tm16372.init();
    tm16372.set(BRIGHT_TYPICAL); //亮度設定，BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
    for (int i = 0; i < 4; i++)
    {
        tm16372.display(i, 0); //設定每一位燈號顯示的內容，參數1：燈號，參數2：顯示的數字
    }
}

void score_display(int hist_score)//""""""""UPDATE"""""""""
{
    tm16372.display(0, hist_score / 10); //設定每一位燈號顯示的內容，參數1：燈號，參數2：顯示的數字
    tm16372.display(1, hist_score % 10);
    tm16372.display(2, now_score / 10);
    tm16372.display(3, now_score % 10);
    delay(100);
}
void set_score(int mode, int hit_score)
{
    now_score += hit_score;
    if (mode == 10)
    {
        if (now_score > hist_score1)
        {
            hist_score1 = now_score;
        }
    }
    else if (mode == 20)
    {
        if (now_score > hist_score2)
        {
            hist_score2 = now_score;
        }
    }
    else if (mode == 30)
    {
        if (now_score > hist_score3)
        {
            hist_score3 = now_score;
        }
    }
}
