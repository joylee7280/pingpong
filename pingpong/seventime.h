#include <SoftwareSerial.h>
#include "TM1637.h"

#define CLK 6 //CLK pin，可自行決定接到哪個PIN
#define DIO 7 //DIO pin，可自行決定接到哪個PIN
TM1637 tm1637(CLK, DIO);
int point_flag = 0;
//unsigned long start_time=0;
//unsigned long total_time=20;
unsigned long extern t;
int extern left_time;
int first = 0;
int second = 0;
int third = 0;
int fourth = 0;


void time_setup()
{
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL); //亮度設定，BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  for (int i = 0; i < 4; i++)
  {
    tm1637.display(i, 0); //設定每一位燈號顯示的內容，參數1：燈號，參數2：顯示的數字
  }
}

void settime()
{
  if (left_time < 0)
  {
    tm1637.display(0, 0); //設定每一位燈號顯示的內容，參數1：燈號，參數2：顯示的數字
    tm1637.display(1, 0);
    tm1637.display(2, 0);
    tm1637.display(3, 0);
  }
  else
  {
    //Serial.println(time_now);
    first = left_time / 60 / 60;
    second = left_time / 60;
    third = (left_time % 60) / 10;
    fourth = left_time % 10;

    tm1637.display(0, first); //設定每一位燈號顯示的內容，參數1：燈號，參數2：顯示的數字
    tm1637.display(1, second);
    tm1637.display(2, third);
    tm1637.display(3, fourth);
  }

  //時和分之間的分號：，它是和時的個位數一起被更新的，要先設定下次更新時，要不要顯示
  //這裡的作法就是一秒顯示，一秒不顯示
}
void show_point()
{
  if (point_flag)
  {
    tm1637.point(POINT_OFF);
    point_flag = 0;
  }
  else
  {
    tm1637.point(POINT_ON);
    point_flag = 1;
  }
}

void counter()
{
  if (millis() > t + 1000)
  {
    t = millis();
    left_time--;
    Serial.print("Left time:");
    Serial.println(left_time);
  }
  show_point();
}